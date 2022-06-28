// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OmEngine/Controllers/MoveCont.h"
#include "../../Ui/VrMenu/VrMenu.h"
#include "VrMoveCont.generated.h"

/**
 * 
 */
UCLASS()
class OMCEM_API AVrMoveCont : public AMoveCont
{
	GENERATED_BODY()
public:
	void Init() override;

	void OnInputHandler(FInputInfo _info) override;
	void Tick(float DeltaSeconds) override;
	float GetAxisYaw() override;
	 void setEnableToolSelectDisplays(bool _enable, class AToolItem* _selected, bool _isErr = false) override;
	UPROPERTY() class AVrMenu* VrMenu;
	UPROPERTY() UMotionControllerComponent* MotionComp;

	 FVector GetProcessInputLoc() override;
	 FRotator GetProcessInputRot() override;
	 void UpdateProcessInputOrigin() override;

		void SetOperationHand(bool _isOperation) override;
		 void Start() override;
		 void TakeTool(class AToolItem* _tool, bool _releaseCurr) override;
		 void PutTool(AToolItem* _tool, EPutToolRule _putRule, FVector _releaseLoc = FVector::ZeroVector)override ;
		 UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventHandAnim(const FString& AnimId, int Status);

protected:
	UPROPERTY() FRotator VrDeltaRotStart;
	UPROPERTY()USkeletalMeshComponent* HandRigComp;
	UPROPERTY()UStaticMeshComponent* OculusMeshComp;
	UFUNCTION() void onInputReceive(bool _status, FString _key);
};
