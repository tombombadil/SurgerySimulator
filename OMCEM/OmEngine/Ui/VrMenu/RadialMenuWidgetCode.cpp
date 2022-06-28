// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialMenuWidgetCode.h"
#include <Engine/Engine.h>
#include "VrMenu.h"
#include "CircleWidgetCode.h"
#include <CanvasPanelSlot.h>
#include <LogMacros.h>

//Create All Menu
void URadialMenuWidgetCode::CreateMenus(TArray<struct FMenuData> menuStruct)
{
	 
	_menuData = menuStruct;
	//Create MainMenu
	TArray<struct FMenuItemData> _mainmenuitem = GetMenuItems(-1);
	UCanvasPanel* mainMenuCanvas = Cast<UCanvasPanel>(GetWidgetFromName("MainMenuCanvas"));
	UCircleWidgetCode* MainMenuWidget = _CreateMenu(_mainmenuitem,"main", mainMenuCanvas,1.0);

	ListenMenuEvents(MainMenuWidget);

	//Create SubMenu
	int32 i = 0;
	for (FMenuData var : menuStruct)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("ss - %s"),var.Item.id));
		TArray<struct FMenuItemData> _subMenuitems=  GetMenuItems(i);
		FString __id= var.Item.id;

		UCanvasPanel* subMenuCanvas = Cast<UCanvasPanel>(GetWidgetFromName("SubMenuCanvas"));
		__canvasReturn = subMenuCanvas;
		UCircleWidgetCode* SubMenuWidget = _CreateMenu(_subMenuitems, __id, subMenuCanvas, 0.5);

		ListenMenuEvents(SubMenuWidget);

		//calculate circleWidget origin from parent circlewidget(main menu item)
		if (SubMenuWidget) 
		{
			//get parent(main) menu item origin angle(point)
			UCircleItemWidget* mainMenuItemWidget = GetCircleWidget("main")->GetItem(__id);
			
			if (mainMenuItemWidget)
			{
				float _angle = ((mainMenuItemWidget->getOrigin())-((SubMenuWidget->TotalAngle)/2))*(-180);
				//returnAngle();
				//returnsubcanvas();
				//update sub menu rotation(angle)
				widgettransform.Translation = SubMenuWidget->RenderTransform.Translation;
				widgettransform.Scale = SubMenuWidget->RenderTransform.Scale;
				widgettransform.Shear = SubMenuWidget->RenderTransform.Shear;
				widgettransform.Angle = _angle;
				SubMenuWidget->SetRenderTransform(widgettransform);
			}
		}
		++i;
	}
}

TArray<struct FMenuItemData> URadialMenuWidgetCode::GetMenuItems(int32 Index)
{
	TArray<struct FMenuItemData> _result;
	
	if (Index>=0)
	{
		//get subs array in current index
		for(FMenuItemData var : _menuData[Index].Subs)
		{
			_result.Add(var);
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, (TEXT("ss - %s"), var.id));
		}
	}
	else 
	{
		//get all items in menudata
		for(FMenuData var : _menuData)
		{
			_result.Add(var.Item);
		}
	}
	return _result;
}

//create menu
UCircleWidgetCode* URadialMenuWidgetCode::_CreateMenu(TArray<struct FMenuItemData> menuStruct, FString _Id, UCanvasPanel* _Canvas, float Degree)
{
	if (menuStruct.IsValidIndex(0))
	{
		if (_Canvas) UE_LOG(LogTemp, Display, TEXT("%s"), *("_Canvas :  " + _Canvas->GetName()));

		UCircleWidgetCode *_circleWidget = CreateWidget<UCircleWidgetCode>(GetWorld(), ClassMenuWidget);

		UPanelSlot* _createdChild = _Canvas->AddChild(_circleWidget);

		//set canvas slot params
		UCanvasPanelSlot* _ChildSlot = dynamic_cast<UCanvasPanelSlot*>(_createdChild);

		_ChildSlot->SetPosition(FVector2D(0.0, 0.0));

		FAnchors _makeAnchor = FAnchors(0.5, 0.5, 0.5, 0.5);

		_ChildSlot->SetAnchors(_makeAnchor);
		
		_ChildSlot->SetAlignment(FVector2D(0.5, 0.5));

		_circleWidget->__menuitemDATA = menuStruct;

		_circleWidget->Init(_Id,Degree);
		
		ListCircleWidget.Add(_Id, _circleWidget);

		_circleWidget->setActive(false,false);

		return _circleWidget;
	}
	return 0;
}



void URadialMenuWidgetCode::ListenMenuEvents(UCircleWidgetCode* _CircleWidget)
{
	if (_CircleWidget)
	{ 
		_CircleWidget->selecteds.AddDynamic(this, &URadialMenuWidgetCode::OnSelectedItem);
	}
}

//onEvent -> selected item
void URadialMenuWidgetCode::OnSelectedItem(UCircleItemWidget* selected,int _status)
{
	switch (_status)
	{
	case 0:
		//higlight
		if (GetCircleWidget(selected->GetId()))
		{
			//for main menu
			highlightcurrentselectedsubmenu(selected->GetId());
		}
		else
		{
			if (CurrentHighlightSubMenu)
			{
				//for sub
				CurrentHighlightSubMenu->SetVisibility(ESlateVisibility::Hidden);
			}
		}
		break;
	case 1:
		//selected
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("onselected ")));

		if (GetCircleWidget(selected->GetId()))
		{
			SetCurrentCircleMenu(selected->GetId());
			CurrentHighlightSubMenu = nullptr;
		}
		else
		{

			//call OnEventSelected
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT(" SSSSSSSSSSS ")));
			OnSelectedItemBPEvent(selected->GetId());
			EventSelectItem.Broadcast(selected->GetId());
			Close();
			selected->setSelected(0);
		}
		break;
	}
}


UCircleWidgetCode* URadialMenuWidgetCode::GetCircleWidget(FString _id)
{
	if (ListCircleWidget.FindRef(_id)) 
	{
		return ListCircleWidget.FindRef(_id);
	}
	else 
	{
		return 0;
	}
}

void URadialMenuWidgetCode::ToogleMenu() 
{

	if (!isOpenMenu) {
		isOpenMenu = true;
		Open();
	}
	else {
		isOpenMenu = false;
		Close();
	}
}

void URadialMenuWidgetCode::Open()
{
	SetCurrentCircleMenu("main");

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("open")));
}

void URadialMenuWidgetCode::Close()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("close %s"), ListCircleWidget.FindKey("main")));

	for (const TPair<FString,UCircleWidgetCode*>& pair : ListCircleWidget)
	{
		//UE_LOG(LogTemp, Display, TEXT("%s"),(pair.Key) );
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("%s"), pair.Key));
		pair.Value->setActive(false,false);
		CurrentCircleMenu = nullptr;
		CurrentCircleMenuId = nullptr;
		if (pair.Value->CurrSelected)
		{
			pair.Value->CurrSelected->setSelected(0);
		}
	}
}

//set current circle menu
void URadialMenuWidgetCode::SetCurrentCircleMenu(FString Id)
{

	if (CurrentCircleMenuId!=Id)
	{
		if (CurrentCircleMenu)
		{
			//deactive prev current& if its not main hide it
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("ASD")));
			CurrentCircleMenu->setActive(false, true);
		}
		
		if (GetCircleWidget(Id))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, (TEXT("ASDx")));

			CurrentCircleMenu = GetCircleWidget(Id);
			CurrentCircleMenuId = CurrentCircleMenu->_Id;
			CurrentCircleMenu->setActive(true, true);
		}
		
		
	}
}

//highlight current selected -> sub menu
void URadialMenuWidgetCode::highlightcurrentselectedsubmenu(FString Id)
{
	if (CurrentHighlightSubMenu)
	{
		CurrentHighlightSubMenu->SetVisibility(ESlateVisibility::Hidden);
		CurrentHighlightSubMenu = NULL;
	}
	if (GetCircleWidget(Id))
	{
		CurrentHighlightSubMenu = GetCircleWidget(Id);
		CurrentHighlightSubMenu->SetVisibility(ESlateVisibility::Visible);
	}
}

UCanvasPanel* URadialMenuWidgetCode::returnsubcanvas()
{
	return __canvasReturn;
}

float URadialMenuWidgetCode::returnAngle()
{
	return _angleReturn;
}


