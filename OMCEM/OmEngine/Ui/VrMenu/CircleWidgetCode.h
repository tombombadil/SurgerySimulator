// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CircleItemWidget.h"
#include "VrMenu.h"
#include <CanvasPanel.h>
#include "CircleWidgetCode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectedItems, class  UCircleItemWidget*, selected, int, status);

/**
 * 
 */
UCLASS()
class OMCEM_API UCircleWidgetCode : public UUserWidget
{
	GENERATED_BODY()
public:


	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	void Init(FString, float);
		void setActive(bool, bool);
		UCircleItemWidget* GetItem(FString);
		void setSelectedIndex(float,bool);
		void HighlightItem(bool, UCircleItemWidget*, bool);
		void CreateCircleItems(float);

		UFUNCTION(BlueprintCallable)void leftForRightCont();
		UFUNCTION(BlueprintCallable)void rightForRightCont();
		UFUNCTION(BlueprintCallable)void selectedForRightCont();
		UFUNCTION(BlueprintCallable)void leftForLeftCont();
		UFUNCTION(BlueprintCallable)void rightForLeftCont();
		UFUNCTION(BlueprintCallable)void selectedForLeftCont();

		
		UCanvasPanel* itemContnr;

		UPROPERTY()
		FOnSelectedItems selecteds;

		bool isActive;

		UPROPERTY(EditAnywhere)
			float TotalAngle;
		UPROPERTY(EditAnywhere)
			FString _Id;

		UPROPERTY(EditAnywhere)
		TArray<struct FMenuItemData> __menuitemDATA;
		

		TArray<class UCircleItemWidget*> ListItems;

		UPROPERTY(EditAnywhere)
			UCircleItemWidget *CurrSelected;
		UPROPERTY(EditAnywhere)
			int CurrSelectedIndex;

		class URadialMenuWidgetCode* radialeventitem;

		UPROPERTY(EditAnywhere)
			TSubclassOf< UCircleItemWidget> ClassSubWidget;
		
	
};
