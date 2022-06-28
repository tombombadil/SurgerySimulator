// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleItemWidget.h"
#include <UnrealString.h>
#include <Materials/MaterialInstanceDynamic.h>
#include <WidgetTransform.h>
#include <Engine/Engine.h>
#include <WidgetTree.h>
#include <ConstructorHelpers.h>
#include <LogMacros.h>



void UCircleItemWidget::NativeConstruct()
{

	Super::NativeConstruct();
}



float UCircleItemWidget::degreeReturn()
{
	return degreeValue;
}

float UCircleItemWidget::startPointReturn()
{
	return startPoint;
}

UMaterialInstanceDynamic* UCircleItemWidget::matReturn()
{
	return MaterialInstance;
}

UButton* UCircleItemWidget::buttonReturn()
{
	return button_42;
}

UCanvasPanel* UCircleItemWidget::canvasReturn()
{
	return canvasPanel_221;
}

UCanvasPanel* UCircleItemWidget::txtcanvasReturn()
{
	return textCanvas;;
}

void UCircleItemWidget::Init(FString _ItemId)
{
	ItemId = _ItemId;
	MaterialInstance = UMaterialInstanceDynamic::Create(Mats, Mats);
	TArray<UWidget*> allW;
	WidgetTree->GetAllWidgets(allW);
	for (UWidget* w : allW)
	{
// 		UE_LOG(LogTemp, Display, TEXT("%s"), *("getname :  " + w->GetName()));
// 		UE_LOG(LogTemp, Display, TEXT("%s"), *("GetPathName :  " + w->GetPathName()));
// 		UE_LOG(LogTemp, Display, TEXT("%s"), *("GetDisplayLabel :  " + w->GetDisplayLabel()));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *("id :  " + _ItemId));

	}
	
	textCanvas = Cast<UCanvasPanel>(GetWidgetFromName("txtCanvas"));
	textBlock = Cast<UTextBlock>(GetWidgetFromName("textBlock_45"));
	if (textBlock)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, (TEXT("text")));
		//labelText=FText::AsCultureInvariant("asd");
		textBlock->SetText(labelText);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, (TEXT("%s"), labelText));

	}
	

	setSelected(0);
}





void UCircleItemWidget::createStyle()
{
	if (MaterialInstance)
	{
		//MaterialInstance->SetScalarParameterValue("Degree in Percent", degreeValue);
		//slate.ImageType.operator=*MaterialInstance;
	}
}

void UCircleItemWidget::setSelected(int Status)
{
	
	switch (Status)
	{
	case 0:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, (TEXT("kapali")));

		isActive = false;
		MaterialInstance->SetVectorParameterValue("Color",ItemBgColorNormal);
		break;
	case 1:
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, (TEXT("acik")));

		isActive = true;
		MaterialInstance->SetVectorParameterValue("Color", ItemBgColorHighlight);
		break;
	}
}



float UCircleItemWidget::getOrigin()
{
	float _result = (startPoint * 2) + (degreeValue);
	return _result;
}

FString UCircleItemWidget::GetId()
{
	FString _id;
	if (!dataStruct.id.IsEmpty())
	{
		_id = dataStruct.id;

	}
	return _id;
}

