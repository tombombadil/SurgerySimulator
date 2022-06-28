// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include <Materials/Material.h>
#include <Button.h>
#include <CanvasPanel.h>
#include "VrMenu.h"
#include <SlateTypes.h>
#include <TextBlock.h>
#include "CircleItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class OMCEM_API UCircleItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(FString);
	void setSelected(int);
	float getOrigin();
	void createStyle();


	FString GetId();

	UFUNCTION(BlueprintCallable)
		float degreeReturn();

	UFUNCTION(BlueprintCallable)
		float startPointReturn();

	UFUNCTION(BlueprintCallable)
		UMaterialInstanceDynamic* matReturn();

	UFUNCTION(BlueprintCallable)
		UButton* buttonReturn();

	UFUNCTION(BlueprintCallable)
		UCanvasPanel* canvasReturn();

	UFUNCTION(BlueprintCallable)
		UCanvasPanel* txtcanvasReturn();


	UPROPERTY(EditAnywhere)
		FMenuItemData dataStruct;

	UPROPERTY(EditAnywhere)
		float startPoint = 0;
	UPROPERTY(EditAnywhere)
		float degreeValue = 0;

	UPROPERTY(EditAnywhere)
		FText labelText;

	UPROPERTY(EditAnywhere)
		UTextBlock *textBlock;
	
	UPROPERTY(EditAnywhere)
		FString ItemId;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Mats;

	UPROPERTY(EditAnywhere)
		UMaterialInstanceDynamic* MaterialInstance;


	UPROPERTY(EditAnywhere)
		UButton* button_42;

	UPROPERTY(EditAnywhere)
		UCanvasPanel* textCanvas;

	UPROPERTY(EditAnywhere)
		UCanvasPanel* canvasPanel_221;
	
	UPROPERTY(EditAnywhere)
		bool isActive;

	FButtonStyle buttonstyle;
	FSlateBrush slate;
	 void NativeConstruct() override;
	 UPROPERTY(EditAnywhere) FLinearColor ItemBgColorNormal;
	 UPROPERTY(EditAnywhere) FLinearColor ItemBgColorHighlight;

};
