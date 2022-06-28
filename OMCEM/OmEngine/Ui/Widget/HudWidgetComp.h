// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OmEngine/Ui/Widget/OmWidgetComp.h"
#include "HudWidgetComp.generated.h"


UCLASS(ClassGroup = (OMCEM), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UHudWidgetComp : public UOmWidgetComp
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void Init();
	virtual void Render(float DeltaTime) override;
	
	void Reset(bool _resetVisiblity = true) override;

	UPROPERTY() UTextBlock* uiPopupTitleTxt;
	UPROPERTY() UTextBlock* uiPopupMsgTxt;
private:
	
		bool isEnableAutoClose;
		float autoCloseTimer;
};
