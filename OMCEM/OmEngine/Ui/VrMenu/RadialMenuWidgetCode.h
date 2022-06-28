// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "VrMenu.h"
#include "CircleWidgetCode.h"
#include "Blueprint/UserWidget.h"
#include <UnrealString.h>
#include "Components/CanvasPanel.h"
#include <WidgetTransform.h>
#include "RadialMenuWidgetCode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventSelectItem, FString, SelectedItemId);

/**
 * 
 */
UCLASS()
class OMCEM_API URadialMenuWidgetCode : public UUserWidget
{
	GENERATED_BODY()

public:
	FEventSelectItem EventSelectItem;
	void CreateMenus(TArray<struct FMenuData>);
	TArray<struct FMenuItemData> GetMenuItems(int32);
	UCircleWidgetCode* _CreateMenu(TArray<struct FMenuItemData>, FString, UCanvasPanel*, float);
	void ListenMenuEvents(UCircleWidgetCode* _CircleWidget);
	UFUNCTION() void OnSelectedItem(UCircleItemWidget* Selected, int _status);
	UCircleWidgetCode* GetCircleWidget(FString);
	void ToogleMenu();
	void Open();
	void Close();
	void SetCurrentCircleMenu(FString);
	void highlightcurrentselectedsubmenu(FString);
	UFUNCTION(BlueprintCallable) UCanvasPanel* returnsubcanvas();
	UFUNCTION(BlueprintCallable) float returnAngle();
	UFUNCTION(BlueprintImplementableEvent) void OnSelectedItemBPEvent(const FString& SelectedItemId);

	TArray<struct FMenuData> _menuData;
	UCanvasPanel* __canvasReturn;
	float _angleReturn;
	UPROPERTY(EditAnywhere) bool isOpenMenu = false;
	UPROPERTY(EditAnywhere) TMap<FString, UCircleWidgetCode*> ListCircleWidget;
	UPROPERTY(EditAnywhere) UCircleWidgetCode* CurrentCircleMenu;
	UPROPERTY(EditAnywhere) FString CurrentCircleMenuId="none";
	UPROPERTY(EditAnywhere)	UCircleWidgetCode* CurrentHighlightSubMenu;
	UPROPERTY(EditAnywhere) TSubclassOf< UCircleWidgetCode> ClassMenuWidget;
	UPROPERTY(EditAnyWhere) FWidgetTransform widgettransform;

};

