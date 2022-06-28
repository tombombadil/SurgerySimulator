// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "OmEngine/Controllers/UICont.h"
#include "OmWidgetComp.generated.h"

UCLASS(ClassGroup = (OMCEM), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UOmWidgetComp : public UWidgetComponent
{
	GENERATED_BODY()
public:
		virtual void BeginPlay() override;
		virtual void Init() ;
		virtual void Reset(bool _resetVisiblity=true) ;
		virtual void Render(float DeltaTime);
		void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
		void SetVisible(bool _visible);
		//UUserWidget* GetUI() { return GetUserWidgetObject(); };
protected:
	//UPROPERTY() UUserWidget* ui;
	
private:
	bool isBeginPlay = false;
	bool isVisible = false;
	
};
