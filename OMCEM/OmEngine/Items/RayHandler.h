// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OmActor.h"
#include "RayHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventRaySelected, bool, _selectedStatus, UPrimitiveComponent*, _comp);



UCLASS()
class OMCEM_API ARayHandler : public AOmActor
{
	GENERATED_BODY()
		friend class URayHandlerComp;
	
public:
	/* */
	UPROPERTY(EditAnywhere, Category = "OMCEM") EShowRayLineRule ShowLineRule = EShowRayLineRule::ALWAYS_HIDE;
	UPROPERTY() bool IsMultiCast = true;
	UPROPERTY(EditAnywhere, Category = "OMCEM")	 EChannels RayChannel;
	UPROPERTY(EditAnywhere, Category = "OMCEM") float RayLen;
	UPROPERTY(EditAnywhere, Category = "OMCEM") float Thickness = 1;
	void BeginPlay() override;
	
	void Init(EChannels _rayChannel, float _rayLenght);
	
	void ShowLine(bool _status, bool _force = false);
	void ShowMark(bool _status);
	void SetEnable(bool _isEnable);
	//int8 SearchCompInList(UOmComp* _comp); /* return : array index, if has not contains then return -1 */
	void Tick(float DeltaSeconds) override;
	bool getContainsInList(TArray< FContactResult> _list, USceneComponent* _comp);
	TArray<UOmComp*> GetHitComps();
	TArray<FContactResult> GetHitResult();
	FEventRaySelected EventSelected;
private:
	UPROPERTY() bool isEnableRayHandler;
	UPROPERTY() USceneComponent* compLine;
	UPROPERTY() USceneComponent* compHitMark;
	UPROPERTY() bool isShowLineAlways;
	bool checkComp(UPrimitiveComponent* _primComp);
	TArray<FContactResult> listHitResult;
	
	void updateShowLinesStatus();
	void addSelected(FContactResult _result);
	void removeSelected(FContactResult _result);
	void updateSelected(FContactResult _result);
};


