// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include <Camera/CameraComponent.h>
#include <Components/WidgetComponent.h>
#include <UnrealString.h>
#include "OmEngine/Items/Item.h"
#include "VrMenu.generated.h"


class UMotionControllerComponent;


USTRUCT(BlueprintType)
struct FMenuItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere) FString id;
	UPROPERTY(EditAnywhere) FString label;
};

USTRUCT(BlueprintType)
struct FMenuData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere) FMenuItemData Item;
	UPROPERTY(EditAnywhere) TArray<struct FMenuItemData> Subs;
};

UCLASS()
class OMCEM_API AVrMenu : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVrMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere) USceneComponent* VROrigin;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Components") class UMotionControllerComponent* LeftMotionController;
	UPROPERTY(EditDefaultsOnly, Category = "Components") class UMotionControllerComponent* RightMotionController;
	UPROPERTY(EditAnywhere) USceneComponent* RootSceneComponent;
	UPROPERTY(EditAnywhere) UCameraComponent* Cam;
	UPROPERTY(EditAnywhere) UWidgetComponent* myWidget;
	UPROPERTY(EditAnywhere) class URadialMenuWidgetCode* RadialMenuData;
	UPROPERTY(EditAnywhere) TArray<struct FMenuData> menuDATA;
//	UPROPERTY(EditAnywhere) float healt;
	UFUNCTION(BlueprintImplementableEvent) void BpEventOnSelected(const FString& SelectedItemId);
	TArray<struct FMenuItemData> menuitemDATA;
    class UCircleWidgetCode* circlewidgetCode;
	UFUNCTION(BlueprintCallable) bool GetIsMenuOpen();
protected:
	UFUNCTION(BlueprintCallable) void OnOpenMenu();
	
	UFUNCTION() void onEventSelectedItem(FString _id);
	void OnInputSelectionWheel();
	void OnInputThumbRight();
	void OnInputThumbLeft();
};
