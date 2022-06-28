// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleWidgetCode.h"
#include <Engine/Engine.h>
#include <UnrealMathUtility.h>
#include <WidgetTree.h>
#include "RadialMenuWidgetCode.h"
#include <Kismet/GameplayStatics.h>



void UCircleWidgetCode::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	/*

	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::MotionController_Left_Trigger))
	{
		if (isActive)
		{
			selecteds.Broadcast(CurrSelected, 1);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Select")));
	}
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Right))
	{
		if (isActive)
		{
			CurrSelectedIndex = CurrSelectedIndex - 1;
			setSelectedIndex(CurrSelectedIndex, true);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Right")));
	}
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->WasInputKeyJustPressed(EKeys::MotionController_Left_Thumbstick_Left))
	{
		if (isActive)
		{
			CurrSelectedIndex = CurrSelectedIndex + 1;
			setSelectedIndex(CurrSelectedIndex, true);
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Left")));
	}
*/

}

void UCircleWidgetCode::leftForRightCont()
{
	if (isActive)
	{
		CurrSelectedIndex = CurrSelectedIndex + 1;
		setSelectedIndex(CurrSelectedIndex, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Left")));
}
void UCircleWidgetCode::rightForRightCont()
{
	if (isActive)
	{
		CurrSelectedIndex = CurrSelectedIndex - 1;
		setSelectedIndex(CurrSelectedIndex, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Right")));
}
void UCircleWidgetCode::selectedForRightCont()
{
	if (isActive)
	{
		selecteds.Broadcast(CurrSelected, 1);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Select")));

}


void UCircleWidgetCode::leftForLeftCont()
{
	if (isActive)
	{
		CurrSelectedIndex = CurrSelectedIndex + 1;
		setSelectedIndex(CurrSelectedIndex, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Left")));
}
void UCircleWidgetCode::rightForLeftCont()
{
	if (isActive)
	{
		CurrSelectedIndex = CurrSelectedIndex - 1;
		setSelectedIndex(CurrSelectedIndex, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Right")));
}
void UCircleWidgetCode::selectedForLeftCont()
{
	if (isActive)
	{
		selecteds.Broadcast(CurrSelected, 1);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Select")));

}






void UCircleWidgetCode::Init(FString Id,float Degrees) 
{
	TotalAngle = Degrees;
	_Id = Id;
	CreateCircleItems(Degrees);

	

	//selectionWheel
	//OnInputThumbRight
	//OnInputThumbLeft
}

void UCircleWidgetCode::setActive(bool Value, bool DontHideifMainMenu)
{
	isActive = Value;
	//Open
	if (isActive)
	{
		setSelectedIndex((__menuitemDATA.Num() - 1), false);
		SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		//Close
		if (DontHideifMainMenu)
		{
			if (_Id=="main")
			{

			}
			else
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
		}
		else
		{
			SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

UCircleItemWidget* UCircleWidgetCode::GetItem(FString __Id)
{
	
	for  (UCircleItemWidget* var : ListItems)
	{
		if (__Id== var->ItemId)
		{
			return var;
		}
		
	}
	return 0;
}

void UCircleWidgetCode::setSelectedIndex(float newIndex, bool TryOpenSubMenu)
{
	HighlightItem(false, CurrSelected, true);
	float clampvalue = FMath::Clamp(newIndex, 0.0f, float((ListItems.Num()-1))) ;
	CurrSelectedIndex = clampvalue;
	CurrSelected= ListItems[CurrSelectedIndex];
	HighlightItem(true, CurrSelected, TryOpenSubMenu);
}

void UCircleWidgetCode::HighlightItem(bool Status, UCircleItemWidget* Item, bool TryOpenSubMenu)
{
	
	if (Item)
	{
		if (Status)
		{
			Item->setSelected(1);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("ss ")));

			if (TryOpenSubMenu)
			{
				selecteds.Broadcast(Item, 0);

			}
		}
		else
		{
			Item->setSelected(0);
		}
	}
}

void UCircleWidgetCode::CreateCircleItems(float _degree)
{
	UE_LOG(LogTemp, Display, TEXT("%f"), _degree);
	itemContnr = Cast<UCanvasPanel>(GetWidgetFromName("ItemContainer"));
	//if(itemContnr) UE_LOG(LogTemp, Display, TEXT("%s"), *("itemContnr :  " + itemContnr->GetName()));
	float i = 0;
	for (FMenuItemData var : __menuitemDATA)
	{
		UCircleItemWidget* _circleitemWidget = CreateWidget<UCircleItemWidget>(GetWorld(), ClassSubWidget);

		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("ss - %s"), __menuitemDATA[i].id));

		float _startpoint = (((i * (_degree / __menuitemDATA.Num())) / 2) + 0.002);
		float _degreevalue = (((_degree / __menuitemDATA.Num()) / 2) - 0.002);
		
		_circleitemWidget->startPoint = _startpoint;
		_circleitemWidget->degreeValue = _degreevalue;
		_circleitemWidget->dataStruct = var;
		_circleitemWidget->labelText= (FText::AsCultureInvariant(__menuitemDATA[i].label));
		FString __startpoint = FString::SanitizeFloat(_startpoint);
		if(itemContnr) itemContnr->AddChild(_circleitemWidget);

		ListItems.Add(_circleitemWidget);

		_circleitemWidget->Init(__menuitemDATA[i].id);

		for (int j=0; j<ListItems.Num();j++)
		{
			ListItems[j]->labelText = (FText::AsCultureInvariant(__menuitemDATA[i].label));
		}

		//UE_LOG(LogTemp, Display, TEXT("asas - sa %s"), *__startpoint);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("- %s",__startpoint)));


		//Set Text->label
		//_circleitemWidget->labelText = FText::AsCultureInvariant(__menuitemDATA[i].label);
			
		//_circleitemWidget->labelText->SetText(FText::AsCultureInvariant(__menuitemDATA[i].label));

		++i;
	}
}
/*
//listen input -> SelectionWheel
void UCircleWidgetCode::ListenSelectionWheel()
{
	if (isActive)
	{
		radialeventitem->OnSelectedItem(CurrSelected, 1);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Select")));
}

void UCircleWidgetCode::ListenleftThumbstickRight()
{
	if (isActive)
	{
		CurrSelectedIndex = CurrSelectedIndex-1 ;
		setSelectedIndex(CurrSelectedIndex, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Right")));

}

void UCircleWidgetCode::ListenleftThumbstickleft()
{
	if (isActive)
	{
		CurrSelectedIndex = CurrSelectedIndex + 1;
		setSelectedIndex(CurrSelectedIndex, true);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("Left")));

}*/

