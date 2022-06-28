// Fill out your copyright notice in the Description page of Project Settings.

#include "VrMenu.h"
#include "RadialMenuWidgetCode.h"
#include <Components/StaticMeshComponent.h>
#include <MotionControllerComponent.h>
#include "GameFramework/Actor.h"
#include <ConstructorHelpers.h>
#include <Components/SceneComponent.h>
#include <GameFramework/Pawn.h>
#include <Engine/World.h>
#include <EngineUtils.h>
#include <Engine/StaticMesh.h>
#include <Engine/Engine.h>
#include <UnrealString.h>
#include <UserWidget.h>
#include <UObjectBase.h>
#include <Slate/SlateBrushAsset.h>
#include <Engine/EngineBaseTypes.h>
#include <Components/InputComponent.h>
#include "CircleWidgetCode.h"



// Sets default values
AVrMenu::AVrMenu()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	//root
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;

	//VROrigin
	/*VROrigin = CreateDefaultSubobject<USceneComponent>(TEXT("VROrigin"));
	VROrigin->SetupAttachment(RootSceneComponent);*/
	
	//LeftRightController
	/*LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetTrackingMotionSource(TEXT("Left"));
	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetTrackingMotionSource(TEXT("Right"));
	LeftMotionController->SetupAttachment(RootSceneComponent);
	RightMotionController->SetupAttachment(RootSceneComponent);*/

	//Camera
	/*Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(VROrigin);*/
	
	//SetClassWidget
	myWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("myWidget"));
	myWidget->SetupAttachment(RootSceneComponent); 
	//static ConstructorHelpers::FClassFinder<UUserWidget>classWidget(TEXT("WidgetBlueprintGeneratedClass'/Game/SPINE/Dev/UI/bp_RadialWidget.bp_RadialWidget_C'"));
	//myWidget->SetWidgetClass(classWidget.Class);
	/*FRotator NewRot =FRotator(0, 0, 0);
	myWidget->SetWorldRotation(NewRot);*/

}

// Called when the game starts or when spawned
void AVrMenu::BeginPlay()
{
	Super::BeginPlay();

// 	healt = 50.0f;
// 	FString healtString = FString::SanitizeFloat(healt);
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, (TEXT("enemy Healt "+healtString)));
// 
// 	//healtString = "enemy Healt " + healtString;
// 	UE_LOG(LogTemp, Warning, TEXT("enemy Health %s"), *healtString);
	RadialMenuData = dynamic_cast<URadialMenuWidgetCode*>(myWidget->GetUserWidgetObject());

	if (RadialMenuData) {

		RadialMenuData->EventSelectItem.AddDynamic(this, &AVrMenu::onEventSelectedItem);
		RadialMenuData->CreateMenus(menuDATA);

	}
	
}

// Called every frame
void AVrMenu::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
/*
void AVrMenu::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AVrMenu::OnOpenMenu);

}*/

void AVrMenu::onEventSelectedItem(FString _id)
{
	BpEventOnSelected(_id);
}
void AVrMenu::OnOpenMenu()
{
	if (RadialMenuData) {
		RadialMenuData->ToogleMenu();
	}
}

bool AVrMenu::GetIsMenuOpen()
{
	if (RadialMenuData) {
		return RadialMenuData->isOpenMenu;
	}
	return 0;
}