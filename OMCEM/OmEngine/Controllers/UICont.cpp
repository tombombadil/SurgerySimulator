#include "UICont.h"
#include "OMCEM.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Ui/UIXRay.h"
#include "UObjectGlobals.h"
#include "OmEngine/Utils/Resource.h"
#include "OmEngine/Ui/Widget/OmWidgetComp.h"
#include "MainCont.h"
#include "OmEngine/Ui/Widget/HudWidgetComp.h"
#include "XRayRender.h"
#include "Observer.h"
#include "TaskCont.h"
#include "ToolsCont.h"
#include "PlayerCont.h"


void FOmWidget::SetActive(bool _status, bool _visibilty)
{
	_isActive = _status;
	if (Widget) Widget->SetVisible(_visibilty);
}


AUICont* AUICont::INS = NULL;

AUICont::AUICont()
{
}

void AUICont::Init()
{
	AUICont::INS = this;

	UTaskCont::INS->EventStatus.AddDynamic( this,&AUICont::onTaskProcessHandler);

	RootTool = Cast<USceneComponent>(GetComponent("ToolRoot"));
	RootHud = Cast<USceneComponent>(GetComponent("HudRoot"));
	WidgetToolAngleX = Cast<UToolWidgetProgress>(GetComponent("ToolAngleX"));
	listWidgets.Add(FOmWidget(EWidgetId::TOOL_ANGLE_X, 0, WidgetToolAngleX));

	WidgetToolAngleY = Cast<UToolWidgetProgress>(GetComponent("ToolAngleY"));
	listWidgets.Add(FOmWidget(EWidgetId::TOOL_ANGLE_Y, 0, WidgetToolAngleY));

	WidgetToolProgress = Cast<UToolWidgetProgress>(GetComponent("ToolProgress"));
	listWidgets.Add(FOmWidget(EWidgetId::TOOL_PROGRESS, 0, WidgetToolProgress));

	WidgetPopup = Cast<UHudWidgetComp>(GetComponent("PopupWidget"));
	listWidgets.Add(FOmWidget(EWidgetId::POPUP, 2, WidgetPopup));
	WidgetPopup->SetVisible(true);

	WidgetDebug = Cast<UOmWidgetComp>(GetComponent("HudDebug"));
	listWidgets.Add(FOmWidget(EWidgetId::DEBUG, 10, WidgetDebug));

	if (!RootTool || !RootHud || !WidgetToolProgress) Debug::Warning("UiCont Blueprint Component isimleri degismis!");
	if (AMainCont::INS->Toolset) SurToolSetDefaultTrans = AMainCont::INS->Toolset->GetActorTransform();

	//if (UResource::INS->ITEM_XRAY)
	{
		WidgetXRay = Cast<UOmWidgetComp>(GetComponent("WCarmLeft"));
		listWidgets.Add(FOmWidget(EWidgetId::XRAY, 3, WidgetXRay));
		WidgetXRay->SetVisible(true);
		UiXray = Cast<UUIXRay>(WidgetXRay->GetUserWidgetObject());

		//WidgetDiscCam = Cast<UOmWidgetComp>(UResource::INS->ITEM_XRAY->GetComponent("DiscCamWidget"));
		//listWidgets.Add(FOmWidget(EWidgetId::STATUS_MONITOR, 10, WidgetDiscCam));
	}




	WidgetTaskMonitor = Cast<UOmWidgetComp>(GetComponent("TaskMonitorWidget"));
	listWidgets.Add(FOmWidget(EWidgetId::STATUS_MONITOR, 11, WidgetTaskMonitor));








	listWidgets.Add(FOmWidget(EWidgetId::TOOL_MENU, 1));

	TooltipToolname = GetChildActor("CaToolname");
	TooltipXRay = GetChildActor("CaTooltipXray");
	if (TooltipXRay)
	{
		TooltipXRay->SetActorLocation(AMainCont::INS->TooltipXrayLoc);
		TooltipXRay->BpEventRun("func:(EnableErr)");
	}

	SetTooltipActivity(EWidgetId::TOOLTIP_TOOLNAME, false);
	SetTooltipActivity(EWidgetId::TOOLTIP_XRAY, false);



	TArray< UOmWidgetComp*> listOmWidgetComps;;
	GetComponents(listOmWidgetComps, true);

	for (UOmWidgetComp* widgetComp : listOmWidgetComps)
	{

		widgetComp->Init();
	}

}



void AUICont::SetActiveWidget(EWidgetId _idWidget, bool _activity, bool  _visiblty)
{
	
		
	

	if (_activity)
	{
		//activeWidget = _idWidget;
		for (FOmWidget wid : listWidgets)
		{
			if (wid.Id == _idWidget)
			{
				activeWidget = wid;
				activeWidget.IsValid = true;
				activeWidget.SetActive(true, _visiblty);
			}
			
		}

		for (FOmWidget wid : listWidgets)
		{
			if (activeWidget.IsValid)
			{
				if (wid.IdGroup == activeWidget.IdGroup)
				{
					if (wid.Id != activeWidget.Id) wid.SetActive(false, _visiblty);
					
				}
				
			}

		}

	}
	else
	{
		if (activeWidget.IsValid)
		{
			activeWidget.SetActive(false, _visiblty);
			activeWidget.IsValid = false;
		}
	}



}





void AUICont::SetToolWidgetRootLoc(FTransform _trans)
{
	//if (UMoveAgent::INS && RootTool)
	{
		//if (UMoveAgent::INS->RefAgentCompMain)
		{
				//RootTool->SetWorldLocation(UMoveAgent::INS->RefAgentCompMain->GetComponentLocation());
				RootTool->SetWorldLocation(_trans.GetLocation());

				FRotator r = RootTool->GetComponentRotation();
				//r.Yaw = UMoveAgent::INS->RefAgentCompMain->GetComponentRotation().Yaw;
				FRotator rot = _trans.GetRotation().Rotator();
				//rot.Yaw = 0;
				
				USceneComponent* angleWidgets = Cast<USceneComponent>(GetComponent("AngleWidgets"));
				angleWidgets->SetWorldRotation(rot);

		}
	}
}
void AUICont::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	

	if(WidgetDiscCam) WidgetDiscCam->SetVisible(true);

	if (WidgetDebug)
	{
		if (!WidgetDebug->GetUserWidgetObject()) return;
		UTextBlock* uiDebugTxt = Cast<UTextBlock>(WidgetDebug->GetUserWidgetObject()->GetWidgetFromName("Msg"));
		FString _msg = UConfig::INS->PrintDebugParams();
		uiDebugTxt->SetText(FText::FromString(_msg));
	}

}



void AUICont::SetTooltipActivity(EWidgetId _idTooltipActor, bool _visible, FString _msg, int8 _status)
{
	AOmActor* actor = 0;
	if (_idTooltipActor == EWidgetId::TOOLTIP_TOOLNAME) actor = TooltipToolname;
	else if (_idTooltipActor == EWidgetId::TOOLTIP_XRAY) actor = TooltipXRay;
	if (actor)
	{
		actor->SetActorHiddenInGame(!_visible);
		if(_visible) actor->BpEventRun("func:(SetText),text:(" + _msg + "),status:("+FString::FromInt(_status)+")");
	}
}



void AUICont::UpdateTaskMonitor(FString _currTaskName, FString _nextTaskName)
{
	UUIStatusMonitor* uiStatus  = Cast<UUIStatusMonitor>(WidgetTaskMonitor->GetUserWidgetObject());
	if(uiStatus) uiStatus->SetTexts(_currTaskName, _nextTaskName);
}


void AUICont::ShowMessage( FString _msg, int8 _msgStatus, float _autoCloseTimer)
{
	if (!WidgetPopup->uiPopupMsgTxt) return;
	WidgetPopup->uiPopupMsgTxt->SetText(FText::FromString(_msg));
	
	if (_autoCloseTimer > 0)
	{
		printMsgtimerDel.BindUFunction(this, FName("onPrintMsgTimer"));
		GetWorldTimerManager().SetTimer(printMsgtimer, printMsgtimerDel, _autoCloseTimer, false);
	}
	BpEventRun("func:(ShowMsg)");

}

void AUICont::HideMessage()
{
	GetWorld()->GetTimerManager().ClearTimer(printMsgtimer);
	BpEventRun("func:(HideMsg)");
}


void AUICont::onPrintMsgTimer()
{
	HideMessage();
}



void AUICont::PrintOnStatusBar(FString _msg, int8 _type)
{
	//if (UIApp->TxtStatusBar)
		//UIApp->TxtStatusBar->SetText(FText::FromString(_msg));


}


FOmWidget AUICont::GetWidget(EWidgetId _id)
{
	FOmWidget w;
	for (FOmWidget wid : listWidgets)
	{
		if (wid.Id == _id)
		{
			w = wid;
		}

	}
	return w;
}

bool AUICont::GetWidgetIsActive(EWidgetId _id)
{
	bool r = 0;
	FOmWidget wid = GetWidget(_id);
	//if (wid.IsValid)
		r = wid.GetActive();
	return r;
}


/* TOOL MENU */
void AUICont::SetSelectToolByRay(AToolItem* _toolItem)
{
	

	if (currSelectToolByRay)
	{
		if (currSelectToolByRay != _toolItem)
		{
			currSelectToolByRay->ResetMaterial();
			currSelectToolByRay = nullptr;
		}
	
	}

	if (_toolItem)
	{
	
			currSelectToolByRay = _toolItem;
			currSelectToolByRay->SetMaterial(AMainCont::INS->INS->MAT_HIGHLIGHTTOOL);

	}

	
	
}




void AUICont::OpenToolMenu(bool _status,  int8 _toolMenuOpenKey , bool _selectTool )
{

	FTransform _trans;
	if ( !AMainCont::INS->Toolset) return;
	if (!Cast<AItem>(AMainCont::INS->Toolset)->IsEnable) return;


	 
	if (_status == IsOpenToolMenu) return;
	 if (_toolMenuOpenKey < toolMenuOpenKey) return;


	if (_status)
	{

		for (AToolItem* tool : UToolsCont::INS->GetToolItems())
		{
			//tool->ResetMaterial();
		}
		_trans.SetLocation( AMainCont::INS->LocToolsetToPlayer);
		//AMainCont::INS->Toolset->BpEventRun("cmd:(OpenAnimStart)");
		IsOpenToolMenu = true;

		if (_selectTool)
		{
			IsActiveToolSet = true;
			SetFocus(true);
		}
		toolMenuOpenKey = _toolMenuOpenKey;

		/*if (currOperationsTool)
		{
			UToolsCont::INS->SetHightlight(true, currOperationsTool->Id);
		}*/
	}
	else
	{
		_trans.SetLocation( SurToolSetDefaultTrans.GetLocation());
		//AMainCont::INS->Toolset->BpEventRun("cmd:(CloseAnimStart)");
		IsOpenToolMenu = false;
		IsActiveToolSet = false;
		APlayerCont::INS->SetFocus(EFocus::GAME);
		toolMenuOpenKey = 0;
		
		/*if (currSelectEqByRay)
		{
			currSelectEqByRay->ResetMaterial();
			currSelectEqByRay = nullptr;
		}*/

		if (currSelectToolByRay)
		{
			//currSelectToolByRay->ResetMaterial();
			currSelectToolByRay = nullptr;
		}
			UToolsCont::INS->SetHightlight(false);
			if (UConfig::INS->LevelMode == 0)
			{
				AUICont::INS->SetTooltipActivity(EWidgetId::TOOLTIP_TOOLNAME, false);
			}
	}
	//twSurToolset = UTween::INS->AddActor(AMainCont::INS->Toolset, _trans, 0.02f);

	isToolMenuOpened = _status;
	SetActiveWidget(EWidgetId::TOOL_MENU, _status, _status);
	
}





void AUICont::ToogleFocus()
{
	SetFocus(isFocus ? false : true);

	
}

void AUICont::SetFocus(bool _status)
{
	if (_status)
		APlayerCont::INS->SetFocus(EFocus::UI_CONTROLLER);
	else
		APlayerCont::INS->BackToPrevFocus();

}





void AUICont::onTaskProcessHandler(EEventType _eventType, FProcessResult _processresult)
{


	//if (_eventType == EEventType::START)
	//{
	//	if (_process)
	//	{
	//		currOperationsTool = Cast<AToolItem>(UOmUtils::GetItemFromOmValues(_process->StartCommand.ToString(), "item"));
	//	}

	//}
	//else if (_eventType == EEventType::COMPLETE)
	//{
	//	currOperationsTool = nullptr;
	//}



}




void AUICont::ToogleDebugHud()
{
	isViewDebugHud = isViewDebugHud?false:true;
	WidgetDebug->SetVisible(isViewDebugHud);

}

