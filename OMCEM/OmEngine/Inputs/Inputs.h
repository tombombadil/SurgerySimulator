#pragma once

#include "OmEngine/Utils/OmValue.h"
#include "OmEngine/Inputs/HapticDevice.h"
#include "OmEngine/Utils/OmStructs.h"
#include <MotionControllerComponent.h>
#include "Inputs.generated.h"

class UHapticDevice;




DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventInput2, FInputInfo, _info);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventInput2, bool, _action, FString, _key);





UCLASS()
class OMCEM_API UInputManager : public UObject
{
	GENERATED_BODY()
public:
	//UEventDispatcher * eventDisp;
	UPROPERTY() FVector MoveInputDelta;
	UPROPERTY() FVector MoveInput;
	UPROPERTY() FVector HapticCenterPos;
	UPROPERTY() FVector RawLoc;
	UPROPERTY() FRotator RotInputDelta;
	UPROPERTY() FRotator RotInput;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) float StickRotation;
	virtual void Update(float DeltaSeconds) {};
	virtual void AddForce(EForceType _type, FVector _direction, float magnitude) {};
	virtual void AddForce(EForceType _type, FVector _force) {};
	virtual FVector getDeltaLoc() { return FVector::ZeroVector; }
	virtual FRotator getDeltaRot() { return FRotator::ZeroRotator; }
	//UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Broadcast")
		//int32 OnBroadcastReceived(const FString& Message);
};



UCLASS()
class OMCEM_API UInputs : public UInputManager
{
	friend class AMoveCont;
	GENERATED_BODY()
public:
	bool BtnHaptic1;
	bool BtnHaptic2;

	UPROPERTY() UHapticDevice* device;
	UPROPERTY() int IdHapticDevice;
	UPROPERTY() UMotionControllerComponent* VrMotinComp;
	
	UInputs();
	bool IsEnableHaptic;
	//bool IsEnable;
	void SetEnable(bool _enable);
	FEventInput2 EventInput;
	UFUNCTION() void onKeyMouseHandler(bool _status, FString _key);
	bool IsEnable;

	void Run(FOmValue cmd);
	
	void Init();
	
	void Update(float DeltaSeconds) override;
	//void UpdatePivotOfHaptic();
	void AddForce(EForceType _type, FVector _direction, float magnitude) override;
	void AddForce(EForceType _type, FVector _force) override;
	void SetJointForceArea(bool _isEnable, FVector _jointPos, FVector _boxExtends, float _influence, float _stiffness);
	void SetVibrate(bool _isEnable, float _amplitude = 0.6, float _freq = 600);
	bool IsHaptic();
	bool IsVr();
	void SetSource(EInputSource _source);
	EInputSource GetSource();
	/* Unreal loc = PositionUnitScale * HapticPos */
	bool GetKeyPreset(FString _keyPreset);

	FString GetKeyPresetString(FString _keyPreset);
	TMap<FString, bool> GetAllKeyPreset();
private:
	bool isInited;
	FVector getLoc();
	FVector getDeltaLoc() override;
	FRotator getRot();
	FRotator getDeltaRot() override;
	void updateMouse(float DeltaTime);
	void updateHaptic(float DeltaTime);
	void updateVr(float DeltaTime);
	void onButtonHandler(FString btnId, bool status);
	
	void UpdateKeyPreset(FString _keyPreset, bool _status);
	
	UPROPERTY() EInputSource inputSource;
	UPROPERTY() TMap<FString, bool> listKeyPresets;
};




UCLASS()
class OMCEM_API UJointForce : public UObject
{
		GENERATED_BODY()
public:
	FString Id;
	FVector point;
	float	stopIfCloseThePointOffset;
	float	stifness;
	float	forceArea;
	bool	IsEnable;
	bool	IsStarted;
	UPROPERTY() UInputs*	Input;

	UJointForce() {};
	void Init(UInputs* _input, FString _id)
	{
		Id = _id;
		Input = _input;
		IsEnable = true;
		IsStarted = false;

	}
	/*  @stopIfCloseThePointOffset should > to -1 for enable  */
	void SetJoint(FVector _point, float _stifness, float _forceArea, float _stopWhenCloseDistance = -1)
	{
		if (!IsEnable  ) return;
		IsStarted = true;
		stifness = _stifness;
		forceArea = _forceArea;
		//point = _point;
		point = UHapticDevice::INS->ConvertUnrealToHaptic(Input->IdHapticDevice, _point);
		stopIfCloseThePointOffset = _stopWhenCloseDistance;
		//Input->SetJointForceArea(true, point, FVector(50, 50, 50), 0, _stifness);
	}

	void Update(FVector _currPos)
	{
		if (!IsEnable || !IsStarted) return;

		float dist = FVector::Distance(_currPos, point);
		FVector distVec = point - _currPos;
		if (stopIfCloseThePointOffset > -1)
		{
			if (dist <= stopIfCloseThePointOffset)
			{
				//Stop();
			}
		}
		FVector distanceValue = distVec / forceArea;
		distanceValue.X = FMath::Clamp<float>(distanceValue.X, -1, 1);
		distanceValue.Y = FMath::Clamp<float>(distanceValue.Y, -1, 1);
		distanceValue.Z = FMath::Clamp<float>(distanceValue.Z, -1, 1);
		FVector force = distanceValue * stifness;
		if(Input) Input->AddForce(EForceType::FORCE_PROCESS_ENDUP, force); 
		
	}


	void Stop()
	{
		//Input->SetJointForceArea(false, FVector::ZeroVector, FVector::ZeroVector, 0, 0);
		IsStarted = false;
	}

};
