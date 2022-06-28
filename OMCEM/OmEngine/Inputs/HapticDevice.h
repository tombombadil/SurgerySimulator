#pragma once

#include "HapticDevice.generated.h"

class UInputs;




UENUM(BlueprintType)
enum class EForceType : uint8
{
	FORCE_COLLISION,
	FORCE_PROCESS_ENDUP,
	FORCE_VIBRATE
};


UCLASS()
class OMCEM_API UHapticDevice : public UObject
{
	GENERATED_BODY()
		friend class UInputs;
public:
	static UHapticDevice* INS;
	UHapticDevice();
	FVector GetLoc(int _idDevice);
	FVector GetRot(int _idDevice);
	bool GetIsEnableDevice(int _idDevice);
	bool UpdatePivotOfHaptic(int _idDevice, FVector _startPositions);
	void Init(bool _isMultipleDevice);
	void SetEnable(bool _enable);

	UPROPERTY() FVector PositionUnitScale;
	FVector PivotOfHaptic;
	UPROPERTY()bool IsEnable;
	
	void Update(float DeltaSeconds) ;
	void SetJoint(int _idDevice, bool _isEnable, FVector _jointPos, FVector _boxExtends, float _influence, float _stiffness);
	FVector ConvertHapticToUnreal(int8 _idDevice, FVector _hapticWorldLoc);
	FVector ConvertUnrealToHaptic(int8 _idDevice, FVector _unrealWorldLoc);
	void Destroy();
	bool importMethod(FString _methodNameFromDll);
	/* call dll funcs */
	int dll_init(bool _isMultipleDevice);
	TArray<float> dll_GetArray(int _device, FString _charValueName, int _sizeOfArr);
	int dll_ApplyForce(int _device, int _index, FVector _force);
	int dll_SetEnableVib(int _device, bool _isEnable, float _amplitude, float _freq);
	TArray<int> dll_GetStatus(int _device);
	FString dll_GetStr(int _device, FString _valueName);
	FString dll_GetString(int _device, FString _charValueName);
	int dll_destroy();
	static bool importDLL(FString folder, FString name);
	static void freeDLL();


protected:
	TMap<int, FVector> startPositions;
	bool isEnableDll;
	TArray<float> toTArray(float* _arr, int8 _sizeOfArray);
	TArray<int> toTArray(int* _arr, int8 _sizeOfArray);
	FVector convertToFVector(TArray<float> _array);
	float* convertToCArray(FVector _fvector);

	


};


