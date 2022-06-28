#include "MoveCont.h"
#include "OMCEM.h"
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Inputs/Inputs.h"
#include "OmEngine/Utils/Resource.h"
#include "OmEngine/Components/ProcessComp.h"
#include "OmEngine/Controllers/PlayerCont.h"
#include "OmEngine/Items/FieldItem.h"
#include "OmEngine/Controllers/PlayerCont.h"
#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Controllers/Observer.h"
#include "ToolsCont.h"
#include "Process.h"
#include "../Components/FxComp.h"
#include "../Utils/OmUtils.h"
#include "../Utils/OmStructs.h"
#include "TaskCont.h"



AMoveCont::AMoveCont()
{
}




void AMoveCont::Init()
{
	


	RootComp = Cast<UOmComp>(GetComponent("Root"));
	CompToolContainer = Cast<UOmComp>(GetComponent("ToolContainer"));
	if (CompToolContainer) compToolContainerDefaultTrans = CompToolContainer->GetRelativeTransform();
	Input = NewObject<UInputs>();
	Input->SetSource(InputSource);
	Input->EventInput.AddDynamic(this, &AMoveCont::OnInputHandler);
	Input->Init();

	HandMesh = Cast<USceneComponent>(GetComponent("Hand"));
	RaySelectTool = Cast<ARayHandler>(GetChildActor("SelectTool"));
	RayHit = Cast<ARayHandler>(GetChildActor("Hit"));
	RayCollProcess = Cast<ARayHandler>(GetChildActor("SelectProcess"));
	RaySelectWidget = Cast<ARayHandler>(GetChildActor("SelectWidget"));
	hitForce = FVector::ZeroVector;

	jointForce = NewObject<UJointForce>();
	
	

	Super::Init();
}

void AMoveCont::SetEnable(bool _isEnable)
{


	IsEnable = _isEnable;
	

	

	if (IsEnable)
	{


		if (InputSource == EInputSource::HAPTIC_LEFT || InputSource == EInputSource::HAPTIC_RIGHT)
		{
			FVector hapticPivot = (Input->IdHapticDevice == 0) ? AMainCont::INS->HapticRightLoc : AMainCont::INS->HapticLeftLoc;
			UHapticDevice::INS->UpdatePivotOfHaptic(Input->IdHapticDevice, hapticPivot);
			jointForce->Init(Input, Id);
		}
		else
		{
			if (InputSource == EInputSource::MOUSE)
			{
				Input->MoveInput = GetActorLocation();
			}
		}
		Input->SetEnable(true);
	}

	if (HandMesh) HandMesh->SetVisibility(false);

}

void AMoveCont::SetEnableSelectTool(bool _enable)
{
	enableSelectTool = _enable;
	if (RaySelectTool)
		RaySelectTool->SetEnable(enableSelectTool);
}

void AMoveCont::SetEnableForceFeedback(bool _enable)
{
	enableForceFeedback = _enable;
	if (RayHit) RayHit->SetEnable(enableForceFeedback);
}

void AMoveCont::SetEnableProcessTrigger(bool _enable)
{
	enableProcessTrigger = _enable;
	if (RayCollProcess)	
		RayCollProcess->SetEnable(enableProcessTrigger);
}

void AMoveCont::SetEnableSelectWidget(bool _enable)
{
	enableSelectWidget = _enable;
	if (RaySelectWidget)	
		RaySelectWidget->SetEnable(enableSelectWidget);
}


void AMoveCont::SetOperationHand(bool _isOperation)
{
	isOperationHand = _isOperation;
}


void AMoveCont::Tick(float DeltaTime)
{
	if (!Input || !IsEnable || !IsInited) return;

	Input->Update(DeltaTime);
	
	

	if (currProcessComp)
	{
		
		currProcessComp->UpdateTransform();

			if ( AMainCont::INS->IsOperationStarted )
			{
				FOmValue obsParams = currProcessComp->GetObserverParams();
				obsParams.UpdateValue("TimeTurnOn", FString::SanitizeFloat(TimerFromTurnOn), true);
				SendParams(obsParams);
			}
	}


	if (Input->IsEnable && !Input->IsVr())
	{
		updateTransform();
	}


	




	if(enableForceFeedback)
	{

		FVector f = FVector::ZeroVector;
		if (!IsOnProcess)
		{
			if(RayHit) f = getHitRayResult(RayHit->RayLen);
		}
		else
		{
			f = FVector::ZeroVector;
		}
		

		Input->AddForce(EForceType::FORCE_COLLISION, f);

		//UOmUtils::DrawLine(GetActorLocation(), GetActorLocation() + (f.GetSafeNormal() * 10), FColor::Yellow, false);
	}
	
		
	if (enableSelectTool && !IsOnProcess)
	{
		setEnableToolSelectDisplays(false, 0); //todo : bunu her seferinde cagirmali mi? 
		//if (AUICont::INS->IsActiveToolSet && UResource::INS->TOOLSET->IsEnable)
		{
			getSelectToolResult();
		}
		
	}


	

	
	if (jointForce->IsEnable)
	{
		jointForce->Update(Input->RawLoc);
	}
	if (IsOnProcess)
	{

		if (Input->GetKeyPreset("KeyProcess") )
			TimerFromTurnOn += DeltaTime;
		else 
			TimerFromTurnOn = 0;


		FVector f = FVector::ZeroVector;
		if (currProcessComp)
		{
			jointForce->stifness = currProcessComp->JointForceStifness + (currProcessComp->ForceFeedbackRatio * currProcessComp->EndForceStifness);
		}
		else
		{
			f = FVector::ZeroVector;
		}

		if (enableForceFeedback )
			Input->AddForce(EForceType::FORCE_COLLISION, f);

		

	}
	if (enableProcessTrigger && !IsOnProcess)
	{
		sendProcessRayResult();
	}
	

	if (isEnableVibrateEff)
	{
		vibrateEffTimer += DeltaTime;
		if (vibrateEffTimer < vibrateEffTimeout)
		{
			
		}
		else
		{
			isEnableVibrateEff = false;
			Input->SetVibrate(false,0,0);
		}
	}




	Super::Tick(DeltaTime);

}



/* SetProcess */
void AMoveCont::SetProcess(bool _status, UProcessComp* _processComp, FOmValue _params)
{
	if (!IsEnable || !CurrTool) return;
	IsOnProcess = _status;
	if (IsOnProcess)
	{
		
		SaveTrans("BeforeProcess");

		CurrTool->SetEnable(false);
		UToolsCont::INS->AttachActor(CurrTool, _processComp->Id, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FOmValue());

		_processComp->SetProcessEnable(true, this, CurrTool, _params);
		jointForce->SetJoint(_processComp->GetStartPoint(), _processComp->JointForceStifness, _processComp->JointForceArea);
		currProcessComp = _processComp;

		FVector uiToolWidgetRootLoc = _processComp->GetOwner()->GetActorLocation() + _processComp->ToolWidgetPosOffset;

		FRotator uiToolWidgetRootRot = FRotator::ZeroRotator;
		uiToolWidgetRootRot.Yaw = _processComp->GetOwner()->GetActorRotation().Yaw;
		uiToolWidgetRootRot.Pitch = _processComp->GetOwner()->GetActorRotation().Pitch;
		uiToolWidgetRootRot.Roll = _processComp->GetOwner()->GetActorRotation().Roll;

		AUICont::INS->SetToolWidgetRootLoc(FTransform(uiToolWidgetRootRot, uiToolWidgetRootLoc));

		if (CurrPart) CurrPart->Run(FOmValue("func:(StartProcess)"));
			
	}
	else 
	{

		if (CurrPart) CurrPart->Run(FOmValue("func:(EndProcess)"));
		if (InputSource == EInputSource::MOUSE)
		{
			Input->MoveInput = GetActorLocation();
			Input->RotInput = FRotator::ZeroRotator;
			
		}
		else
		{
			updateTransform();
		}


		_processComp->SetProcessEnable(false, this, CurrTool, _params);
		FOmValue getObserverParams = _processComp->GetObserverParams();
		bool isAutoCompByTaskCont = getObserverParams.GetValue("IsByTaskCont").IsValid ? getObserverParams.GetValue("IsByTaskCont").GetBool(0) : false;


		jointForce->Stop();
		
		Input->AddForce(EForceType::FORCE_COLLISION, FVector::ZeroVector, 0);
		Input->AddForce(EForceType::FORCE_VIBRATE, FVector::ZeroVector, 0);
		Input->AddForce(EForceType::FORCE_PROCESS_ENDUP, FVector::ZeroVector, 0);


		if (CurrPart)
		{
			{//#metric : hole(kemik)'e yeni bir toolpart eklendi


				UTaskCont::INS->currProcess->MetricEnd.AddDataParam("tool_name", CurrPart->IdCat);
				FVector toolSize = CurrTool->CurrPart->ToolSize;
				UTaskCont::INS->currProcess->MetricEnd.AddDataParam("tool_width", FString::SanitizeFloat(toolSize.X));
				UTaskCont::INS->currProcess->MetricEnd.AddDataParam("tool_length", FString::SanitizeFloat(toolSize.Z));

			}
		}


		if (!_processComp->IsReActiveTool)
		{
			if (CurrTool->CurrPart)
			{
				CurrTool->CurrPart->SetEnable(false);
				APlayerCont::INS->PutTool("MoveR", CurrTool->CurrPart->RuleOfPutToToolset); //release curr item, take a empty tool
				
			}
			else
			{
				CurrTool->SetEnable(false);
				APlayerCont::INS->PutTool("MoveR", EPutToolRule::PUT_KEEP_TRANSFORM); //release curr item, take a empty tool
				//TakeTool(0, false);
			}
			
			updateToolContainerRelative(0);
		}
		else
		{
		
			CurrTool->SetEnable(true);

			if (UTaskCont::INS->IsAIMode)
			{
				APlayerCont::INS->PutTool("MoveR", EPutToolRule::PUT_TOOLSET_RESTORE_INIT_TRANSFORM);


			}
			else
			{
				UToolsCont::INS->AttachActor(CurrTool, CompToolContainer->Id, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FOmValue());
				
			}
			
			updateToolContainerRelative(CurrTool);
			

		}

		currProcessComp = nullptr;

		if (AMainCont::INS->IsOperationStarted)
		{
			{//#metric process end!

				UServer::INS->SendMetric(UTaskCont::INS->currProcess->MetricEnd); //process bitis metrici gonderiliyor..
			}
		}
	
	

		RestoreTrans("BeforeProcess");


		
	}


	if (enableSelectWidget)
	{
		if (RaySelectWidget)
		{
			RaySelectWidget->SetEnable(!_status); //process aktifken widget secmeyi iptal edelim
		}
	}

}


/* haptic'deki kalemin yaw donme acisi */
float  AMoveCont::GetAxisYaw()
{
	float r = 0;
	if (Input->IsHaptic())
		r = Input->StickRotation;

	return r;
}

/* key presets from UInputs */
void AMoveCont::OnInputHandler(FInputInfo _info)
{
	FString _keyPreset = _info.KeyPreset;
	bool _status = (_info.Value != 0);

	if (_keyPreset == "KeyProcess")
	{
		if (CurrTool)
			if (CurrTool->IsVibrationTool)
			{
				if (_status)
				{
					CurrTool->IsTurnOn = true;
					CurrTool->Run(FOmValue("func:(TurnOn)"));

					Input->SetVibrate(true);
				}
				else
				{
					CurrTool->IsTurnOn = false;
					Input->SetVibrate(false, 0, 0);
					CurrTool->Run(FOmValue("func:(TurnOff)"));
				}
			}

		if (!_status)
		{
			Input->SetVibrate(false, 0, 0);
		}


		if (RaySelectWidget)
		{
			for (UOmComp* comp : RaySelectWidget->GetHitComps())
			{
				if (_status)
				{
					AOmActor* item = Cast<AOmActor>(comp->GetOwner());
					//item->OnSelect(FSelectHandler(ESelectHandlerAction::CLICK, InputSource, comp));
					item->OnSelect(FSelectHandler(ESelectHandlerAction::CLICK, RaySelectWidget->RayChannel, comp, this));
				}
			}
		}


	}





	if (_status)
	{
		if (currProcessComp)
		{
			currProcessComp->OnKeyPress(_keyPreset);


		}
		if (_keyPreset == "KeySelectTool" && enableSelectTool && !IsOnProcess)
		{
				takeAndPutTool();





		}






	}
	else
	{

	}



	/* enable/disable effs */
	if (_keyPreset == "KeyProcess")
	{
		if (CurrTool)
		{
			for (UOmComp* c : CurrTool->GetComps())
			{
				class UFxComp* fx = Cast<UFxComp>(c);
				if (fx)
				{
					c->SetEnable(_status);
				}
			}
		}
	}

	if (IsEnable && enableSelectTool)
	{//sent to observer
		FOmValue sendParams;
		sendParams.Add(FOmValue(_keyPreset + ":(" + FString::FromInt((int)_status) + ")"), true);

		UObserver::INS->Send(sendParams, true);
	}


}







void AMoveCont::updateTransform()
{


	FTransform tworld, trel;
	tworld = this->GetActorTransform();
	trel = FTransform(FRotator::ZeroRotator, FVector::ZeroVector);
	if (CurrTool) trel = CurrTool->GetRootComponent()->GetRelativeTransform();

	FVector locw = Input->getLoc();
	


	
	FRotator deltaRot = Input->getRot();
	FRotator rotw = tworld.GetRotation().Rotator();
	rotw.Pitch = deltaRot.Pitch + baseRot.Pitch;
	rotw.Roll = deltaRot.Roll + baseRot.Roll;
	rotw.Yaw = 0;// baseRot.Yaw;


	tworld.SetLocation(locw);
	tworld.SetRotation(rotw.Quaternion());







	this->SetActorTransform(tworld);
	if (CurrTool) CurrTool->GetRootComponent()->SetRelativeTransform(trel);


}








void AMoveCont::TakeTool(AToolItem* _tool, bool _releaseCurr)
{
	if (_releaseCurr)
	{
		if (_tool->IsPartOfTool) //want to take a new part
		{
			if (CurrPart)
			if (CurrPart != _tool) //put CurrPart
			{
				PutTool(CurrPart, CurrTool->CurrPart->RuleOfPutToToolset);
				setCurrentPart(0);
			}
			
		}
		else 
		{
			if(CurrTool)
			if (CurrTool !=_tool) //put CurrTool
			{
				PutTool(CurrTool, EPutToolRule::PUT_TOOLSET_RESTORE_INIT_TRANSFORM);
				CurrTool = 0;
			}
			
		}

	}

	if (_tool)
	
	{
		FString parentId;
		if (CurrTool)
		{
			if (_tool != CurrTool)
			{
				setCurrentPart(_tool);
				parentId = CurrTool->Id;
			}
			
			
		}
		else
		{
			CurrTool = _tool;
			parentId = CompToolContainer->Id;
		}
		
		//UToolsCont::INS->ConnectItems(_tool->Id, parentId, FOmValue("IsSnapTarget:(1),ConnectPoint:(HoldPoint)"));
		UToolsCont::INS->ConnectItems(_tool->Id, parentId, FOmValue("IsSnapTarget:(1)"));
		_tool->SetActiveTool(true, false);

		//if (CurrTool)
		{
			updateToolContainerRelative(CurrTool);
		}
		
		
		{//#metric selecttool

			if (AMainCont::INS->IsOperationStarted)
			{
				FMetricJson m = FMetricJson::Create();
				m.Json->SetStringField("category", "selectTool");
				m.Json->SetStringField("action", "select");
				m.Json->SetStringField("value", _tool->Id);
				m.AddDataParam("tool_name", _tool->IdCat);
				FVector toolSize = _tool->ToolSize;
				m.AddDataParam("tool_width", FString::SanitizeFloat(toolSize.X));
				m.AddDataParam("tool_length", FString::SanitizeFloat(toolSize.Z));
				UServer::INS->SendMetric(m);
			}

		}
		
	}

	if (HandMesh) HandMesh->SetVisibility((bool)!CurrTool, true);
	
}
void AMoveCont::PutTool(AToolItem* _tool, EPutToolRule _putRule, FVector _releaseLoc)
{
	_tool->Run(FOmValue("func:(TurnOff)"));
	_tool->IsTurnOn = false;
	Input->SetVibrate(false, 0, 0);

	
	UToolsCont::INS->DetachActor(_tool, FDetachmentTransformRules::KeepWorldTransform);
	_tool->SetActiveTool(false, true);
	if (_tool == CurrTool)
	{
		CurrTool = 0;
	}
	else if (_tool == CurrPart)
	{

		setCurrentPart(0);
	}

	if (_putRule != EPutToolRule::PUT_KEEP_TRANSFORM)
	{
		UToolsCont::INS->AttachActor(_tool, UResource::INS->TOOLS->Id, FAttachmentTransformRules::KeepWorldTransform, FOmValue());


		if (_putRule == EPutToolRule::PUT_TOOLSET_RAY_POINT)
			_tool->SetItemLoc(_releaseLoc, true, "ConnectPoint");
		else if (_putRule == EPutToolRule::PUT_IN_TRAY)
		{
			UOmComp* trayContainer = UResource::INS->GetOmComp(AMainCont::INS->IdTrayInToolset);
			if (trayContainer)
			{
				FVector loc = trayContainer->GetRandomPositionInBounds();
				_tool->SetItemLoc(loc, true);
			}
		}

		else if (_putRule == EPutToolRule::PUT_TOOLSET_RESTORE_INIT_TRANSFORM)
			_tool->RestoreRelTrans("InitRel");

	}

	if (HandMesh) HandMesh->SetVisibility(!CurrTool);

	if(InputSource == EInputSource::VR_LEFT || InputSource == EInputSource::VR_RIGHT) updateToolContainerRelative(0);
	

	SendParams(FOmValue("PutTool:(" + _tool->Id +"),PutToolCat:(" + _tool->IdCat + ")"));
}



void AMoveCont::updateToolContainerRelative(AToolItem* _tool)
{
	if (InputSource != EInputSource::VR_LEFT && InputSource != EInputSource::VR_RIGHT) return;
	if (_tool)
	{
		//UOmComp* compHoldTrans = _tool->GetCompByName("HoldPoint");
		UOmComp* compHoldTrans = Cast<UOmComp>( GetChildActor("HandRigChildActor")->GetComponent(_tool->Id));
		if (compHoldTrans)
		{
			//CompToolContainer->SetRelativeLocation(CompToolContainer->GetRelativeTransform().GetLocation() - compHoldTrans->GetRelativeTransform().GetLocation());
			CompToolContainer->SetRelativeLocation(compHoldTrans->GetRelativeTransform().GetLocation());
			//CompToolContainer->SetRelativeRotation(CompToolContainer->GetRelativeTransform().GetRotation().Rotator() + compHoldTrans->GetRelativeTransform().Rotator());
			CompToolContainer->SetRelativeRotation( compHoldTrans->GetRelativeTransform().Rotator());
		}


	}
	else
	{
		if (!CurrTool) //birakilan tool bir parttool degilse
		{
			if (CompToolContainer)
			{
				CompToolContainer->SetRelativeTransform(compToolContainerDefaultTrans);
			}
		}
	
	}


	

	
}
void AMoveCont::setCurrentPart(AToolItem* _newCurrPart)
{
	CurrPart = _newCurrPart;
	if (CurrTool)
	{
		CurrTool->SetCurrPart(CurrPart);
	}
}


/* ________________________ Select Tool Ray _________________________ */
void AMoveCont::setEnableToolSelectDisplays(bool _enable, AToolItem* _selected, bool _isErr)
{
	
	if (_enable)
	{
		FString msg = _isErr ? UConfig::GET_MSG("err_wrong_eq") : _selected->Label;
		if(!_isErr) AUICont::INS->SetSelectToolByRay(_selected);
		if ( UConfig::INS->LevelMode == 0)
		{
			AUICont::INS->SetTooltipActivity(EWidgetId::TOOLTIP_TOOLNAME, true, msg, _isErr?1:0);
		}
			
	}
	else
	{
		AUICont::INS->SetSelectToolByRay(0);
		if ( UConfig::INS->LevelMode == 0)
		{
			AUICont::INS->SetTooltipActivity(EWidgetId::TOOLTIP_TOOLNAME, false);
		}

	}

	
}


/* ________________________ Process Ray _________________________ */
void AMoveCont::sendProcessRayResult()
{

	for (FContactResult c : RayCollProcess->GetHitResult())
	{
	
		
			
			UOmComp* to = Cast<UOmComp>(c.toComp);
			FString fromId;
		
			if (to)
			{
				if (CurrTool)
				{
					fromId = CurrPart ? CurrPart->IdCat : CurrTool->IdCat;
				}
				else
				{
					fromId = "Hand";
				}
				UpdateParams(FOmValue("RayFrom:(" + fromId + "),RayTo:(" + to->Id + ")"), true);
			}
		


	}

}




void AMoveCont::takeAndPutTool()
{



	for (FContactResult contact : getSelectToolResult())
	{
		AToolItem* selected = Cast<AToolItem>(contact.fromActor);
		if (contact.toActor == this)
		{
			TakeTool(selected, true);
		}
		else
		{
			if (selected == CurrPart)
			{
				PutTool(CurrPart, CurrPart->RuleOfPutToToolset,contact.HitLoc);
				setCurrentPart(0);
			}
			else if (selected == CurrTool)
			{
				PutTool(CurrTool, EPutToolRule::PUT_TOOLSET_RESTORE_INIT_TRANSFORM);
				CurrTool = 0;
			}
		}
	}


}

TArray<FContactResult> AMoveCont::getSelectToolResult()
{
	TArray<FContactResult>  resultList;
	AItem* selected = 0;
	FVector selectedRayLoc = FVector::ZeroVector;


	TArray<FContactResult>  listSelectedToolsByRay = RaySelectTool->GetHitResult();
	AToolItem* takeTool = 0;
	//once neleri alabiliriz ona bakalim
	for (FContactResult hit : listSelectedToolsByRay)
	{
		bool isCanTake = false;
		if (Cast<AToolItem>(hit.toActor))
		{
			if (!(CurrTool && hit.toActor == CurrTool) && !(CurrPart && hit.toActor == CurrPart))
			{
				AToolItem* selected = Cast<AToolItem>(hit.toActor);


			
				UContainerComp* compContainer = Cast<UContainerComp>(hit.toComp);
				if (compContainer)
				{
					
					if (selected->IsPartOfTool)
					{
						if (CurrTool)
						{
							if (CurrTool->CompContainer->CanPutItem(selected->IdCat)) //CurrTool, secili Part'i alabiliyor mu?
							{
								isCanTake = true;
								
							}
							else
							{
								
									setEnableToolSelectDisplays(true, selected, true);
									AUICont::INS->TooltipToolname->SetActorLocation(hit.HitLoc);
								

							}
						}
							
					}
					else //secilen part degilse, bir tooldur. sartsiz alinabilir.
					{
						isCanTake = true;

				}
				
				}



			}

			
		}

		if (isCanTake)
		{
			FContactResult newContact = hit;
			newContact.fromActor = hit.toActor;
			newContact.toActor = this;
			takeTool = Cast<AToolItem>( hit.toActor);
			resultList.Add(newContact);


			if (takeTool)
			{
				setEnableToolSelectDisplays(true, Cast<AToolItem>(takeTool), false);
				AUICont::INS->TooltipToolname->SetActorLocation(hit.HitLoc);

			}
			

			
		}
	}



	//neleri birakabiliriz
	for (FContactResult hit : listSelectedToolsByRay)
	{
		bool isCanPut = true;
		if (Cast<AItem>(hit.toActor))
		{
			if (!(CurrTool && hit.toActor == CurrTool) && !(CurrPart && hit.toActor == CurrPart))
			{
				//AItem* selected = Cast<AItem>(hit.toActor);

				UContainerComp* compContainer = Cast<UContainerComp>(hit.toComp);
				AToolItem* putTool = 0;
				if (compContainer)
				{
					if (CurrPart)
					{
						putTool = CurrPart;
						
						isCanPut = compContainer->CanPutItem(CurrPart->IdCat);
						


					}

					else if (CurrTool)
					{
						putTool = CurrTool;
						//alinan takeTool bir part ise CurrTool'i geri birakmamamliyiz!
						if(takeTool)
						if (takeTool->IsPartOfTool)
						{
							isCanPut = false;
						}


						//bir takeTool = null ise CurrPart'i biraktik zaten. CurrTool'i geri birakmamamliyiz!
						if (!takeTool && CurrPart ) 
						{
							isCanPut = false;
						}

						
						
					}

					if (isCanPut && putTool)
					{
						FContactResult newContact = hit;
						newContact.fromActor = putTool;
						newContact.toActor = hit.toActor;
						resultList.Insert(newContact, 0);
					}

				}



			}


		}

		
	}
			


	return resultList;



}





FVector AMoveCont::getHitRayResult(float _length)
{
	
	FVector result = FVector::ZeroVector;
	RayHit->RayLen = _length;
	TArray<FContactResult> listAll = RayHit->GetHitResult();
		
	if (listAll.Num() > 0)
	{
		FContactResult c = listAll[0];
		float stifness = 0;
		if (c.GetToItem())
		{
			stifness = c.toComp->ComponentHasTag("softbody") ? c.GetToItem()->CollisionForcStiffness / 2 : c.GetToItem()->CollisionForcStiffness; 
			{
				FVector f = c.RayHitNormal * c.RayHitRatio *-1 * stifness;
				result = f;

			}
		}
				
	}


	return result;
}



void AMoveCont::UpdateParams(FOmValue _value, bool sendToObserver)
{
	if (_value.IsValid)
	{
		params.Add(_value, true);
	}
	if (sendToObserver)
		SendParams(_value);
}

void AMoveCont::SendParams(FOmValue _value)
{


	/* new */
	for (auto& elem : Input->GetAllKeyPreset())
	{
		FString keyName = elem.Key;
		bool keyStatus = elem.Value;
		_value.Add(keyName+":(" + FString::FromInt((int)keyStatus) + ")", true);
	}
	




	if(currProcessComp) _value.Add(",ProcessStep:(" + FString::FromInt((int)currProcessComp->GetProcessStep()) + ")", true);
	
	UObserver::INS->Send(_value);
}


//void AMoveCont::UpdateMetrics(FOmValue _value)
//{
//	if (_value.IsValid)
//	{
//		  paramsMetrics.Add(_value, true);
//	}
//}


void AMoveCont::UpdateProcessInputOrigin()
{
	processInputOrigin = GetActorTransform();
}

FVector AMoveCont::GetProcessInputLoc()
{
	FVector r = GetActorLocation() - processInputOrigin.GetLocation();
	return r;
}


FRotator AMoveCont::GetProcessInputRot()
{

	FRotator r = CompToolContainer->GetComponentRotation() - processInputOrigin.GetRotation().Rotator();

	//if (_isDelta)
		//r = GetActorTransform().GetRotation().Rotator() - VrDeltaRotStart;
	//else




	return r;
}







void AMoveCont::Run(FOmValue cmd)
{
	if (!IsEnable) return;

	FString strCmd = cmd.ToString();

	BpEventRun(strCmd);

	FString func = cmd.GetValue("func").GetString(0);



}




void AMoveCont::setBaseRot(FRotator _rotator)
{
	baseRot = _rotator;
	//SetWorldRotation(_rotator);
}








void AMoveCont::Destroy()
{
	

}






UProcessComp* AMoveCont::GetCurrProcessComp()
{
	return currProcessComp;
}

//
//void AMoveCont::SetVibrate(bool _status, float _timeout, float _amplitude, float _freq)
//{
//	if (!IsEnable) return;
//	isEnableVibrateEff = _status;
//	if (isEnableVibrateEff)
//	{
//		vibrateEffTimeout = FMath::Min<float>(_timeout, 10);
//		Input->SetVibrate(true, _amplitude, _freq);
//	}
//
//}

