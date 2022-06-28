#include "PlayerCont.h"
#include "OMCEM.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Controllers/UICont.h"
#include "OmEngine/Controllers/ToolsCont.h"
#include "OmEngine/Controllers/Observer.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Inputs/Inputs.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Controllers/TaskCont.h"
#include "CameraCont.h"
#include "OmHUD.h"
#include "../Components/ProcessComp.h"
#include "MoveCont.h"
#include "OmEngine/Ui/VrMenu/VrMenu.h"
#include "Player/VrMoveCont.h"


APlayerCont * APlayerCont::INS = NULL;



APlayerCont::APlayerCont()
{
	

	//MouseWheelAxis = 0;
	//KeyAxisWS = 0;
	//KeyAxisAD = 0;
	//KeyAxisUpDown = 0;

	APlayerCont::INS = this;
	//PrimaryActorTick.bCanEverTick = true;
	focusId = EFocus::GAME;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

}


void APlayerCont::Init()
{
	

	resetValues();
	
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	FInputModeGameOnly _inputGameOnly;
	MyController->SetInputMode(_inputGameOnly);
	AutoReceiveInput = EAutoReceiveInput::Player0;

	

	TArray<USceneComponent*> listAllChilds;
	GetComponents(listAllChilds, true);
	for (USceneComponent* c : listAllChilds) 
	{

		if (c->GetName() == "CamRoot")
			compCamRoot = c;
		//if (Cast<UOmComp>(c)) Cast<UOmComp>(c)->Init();
		//if(Cast<UOmComp>(c)) Cast<UOmComp>(c)->Init();
		

		
		
	}
	
	Cam = Cast<UCameraCont>( GetComponentByClass(UCameraCont::StaticClass()));

	//AVrMoveCont* VrL = Cast<AVrMoveCont>(registerMoveCont(Cast<AMoveCont>(GetChildActor("VrL")), true));
	//AVrMoveCont * VrR = Cast< AVrMoveCont>( UResource::INS->SpawnItem(ClassVrRight, FVector::ZeroVector, FRotator::ZeroRotator));

	


	
	
}


void APlayerCont::AfterInit()
{
	AVrMoveCont* VrR = 0;
	AVrMoveCont* VrL = 0;
	if (AMainCont::INS->IsVRMode)
	{
		 VrR = spawnVrMove(ClassVrRight);

		 VrL = spawnVrMove(ClassVrLeft);


		VrR->Init();
		VrR->SetEnable(true);
		VrR->SetEnableSelectWidget(true);

		VrL->Init();
		VrL->SetEnable(true);
		VrL->SetEnableSelectWidget(true);
	}
	

	//AVrMoveCont* VrR = Cast<AVrMoveCont>(registerMoveCont(Cast<AMoveCont>(GetChildActor("VrR")), true));
	AMoveCont* HapticR = registerMoveCont(Cast<AMoveCont>(GetChildActor("HapticR")));
	AMoveCont* HapticL = registerMoveCont(Cast<AMoveCont>(GetChildActor("HapticL")));
	AMoveCont* Mouse = registerMoveCont(Cast<AMoveCont>(GetChildActor("Mouse")));
	HapticR->SetEnable(false);
	HapticL->SetEnable(false);
	Mouse->SetEnable(false);


	if (AMainCont::INS->InputMode == 0) //MOUSE------> Right(Mouse), +VrR + VrL
	{
		Mouse->SetEnableSelectTool(true);
		Mouse->SetEnableProcessTrigger(true);
		Mouse->SetEnableSelectWidget(true);
		MoveR = Mouse;

	}

	else if (AMainCont::INS->InputMode == 1) //1 HAPTIC------>Right(HapticRight), +VrR + VrL
	{
		HapticR->SetEnableSelectTool(true);
		HapticR->SetEnableProcessTrigger(true);
		HapticR->SetEnableSelectWidget(true);
		HapticR->SetEnableForceFeedback(true);
		MoveR = HapticR;
	}
	else if (AMainCont::INS->InputMode == 2)//2 HAPTIC------>Right(HapticRight), Left(HapticLeft), +VrR + VrL
	{
		HapticR->SetEnableSelectTool(true);
		HapticR->SetEnableProcessTrigger(true);
		HapticR->SetEnableSelectWidget(true);
		HapticR->SetEnableForceFeedback(true);
		MoveR = HapticR;
		MoveL = HapticL;
	}
	if (AMainCont::INS->InputMode == 3) //VR------>Right(VrR), Left(VrL)
	{
		if (VrR)
		{
			VrR->SetEnableSelectTool(true);
			VrR->SetEnableProcessTrigger(true);
			MoveR = VrR;

			MoveL = VrL;
		}
		else
		{
			MoveR = Mouse;
		}
		

	}


	if (MoveR)
	{
		MoveR->SetEnable(true);
		MoveR->SetOperationHand(true);
	}
	if (MoveL)
	{
		MoveL->SetEnable(true);
		MoveL->SetOperationHand(true);
	}
}


AVrMoveCont* APlayerCont::spawnVrMove(TSubclassOf<AVrMoveCont> _class)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = this;
	AVrMoveCont* vrMove = AMainCont::INS->GetWorld()->SpawnActor<AVrMoveCont>(_class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	UResource::INS->RegisterItem(vrMove);
	vrMove->AttachToComponent(compCamRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	registerMoveCont(vrMove);
	return vrMove;
}
AMoveCont* APlayerCont::registerMoveCont(AMoveCont* _moveCont)
{
	listMoveConts.Add(_moveCont);
	return _moveCont;
}
void APlayerCont::Start()
{
	initializeTransparentMeshes();

	for(AMoveCont* move : listMoveConts)
	{
		if (move->GetEnable())
		{
			if (move->Input)
				move->Input->EventInput.AddDynamic(this, &APlayerCont::onInputEventHandler);
		}
		
	}
	
	
}


void APlayerCont::onInputEventHandler(FInputInfo _info)
{
	BpEventInput(_info);
}



void APlayerCont::TakeTool(FString _move, AToolItem* _tool, bool _releaseCurrent)
{
	AMoveCont* move = GetMoveCont(_move);
	
	if (move)
	{
		move->TakeTool(_tool, _releaseCurrent);
	}
}

void APlayerCont::TakeTool(FString _move, FString _id)
{

	AToolItem* tool = Cast<AToolItem>(UResource::INS->GetItem(_id, false));
	if (tool) TakeTool(_move, tool);

}

AToolItem* APlayerCont::PutTool(FString _move, EPutToolRule _putRule, FVector _releaseLoc )
{
	AToolItem* tool = 0;
	AMoveCont* move = GetMoveCont(_move);
	if (move)
	{
		if (move->CurrPart) tool = move->CurrPart;
		else if (move->CurrTool) tool = move->CurrTool;
			
		if(tool)	move->PutTool(tool, _putRule, _releaseLoc);

	}
	return tool;
}






void APlayerCont::SetProcess(bool _status, FString _move, AToolItem* _tool, UProcessComp* _processComp, FOmValue _params)
{
	AMoveCont* move = GetMoveCont(_move);
	if (!move) return;
	//Debug("#player.SetProcess -> @_processComp " + _processComp->Id + "  @_status :" + FString::FromInt((int)_status) + " @move->IsOnProcess : " , move->IsOnProcess);
	if (_status)
	{
		if (!move->IsOnProcess && move->CurrTool)
		{
			move->CurrTool->SetProcess(true, _processComp, _params);
			move->SetProcess(true,  _processComp, _params);

		}

	}
	else
	{
		if (move->IsOnProcess && move->CurrTool)
		{

			move->CurrTool->SetProcess(false, _processComp, _params);
			move->SetProcess(false,  _processComp, _params);
		}

	}
}


AMoveCont* APlayerCont::GetMoveCont(FString _move)
{
	AMoveCont* m = 0;


	if (_move == "MoveR")
		m = MoveR;
	else if (_move == "MoveL")
		m = MoveL;
	
	return m;
}


USceneComponent* APlayerCont::GetComp( FString _compName)
{
	USceneComponent* comp = 0;

	TArray<USceneComponent*> arrAll;
	this->GetComponents(arrAll, true);
	for (USceneComponent* c : arrAll)
	{
		if (c->GetName() == _compName) comp = c;
	}

	return comp;
}

void APlayerCont::Update(float DeltaSeconds)
{
	

	for (UOmComp* c : GetComps())
	{
		c->Render(DeltaSeconds);
	}


	
}

void APlayerCont::updateMotionCont()
{
	
}



void APlayerCont::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (AMoveCont* m : listMoveConts)
	{
		if (m->GetEnable())
			m->Destroy();
	}
	
	if (UHapticDevice::INS) 	UHapticDevice::INS->Destroy();
}



TArray<UOmComp*> APlayerCont::GetComps()
{
	TArray<UOmComp*> allComps;
	this->GetComponents(allComps, true);
	return allComps;
}


EFocus APlayerCont::GetFocus()
{
	return focusId;
}

void APlayerCont::SetFocus(EFocus _focusId)
{
	if ((int)focusId != (int)_focusId)
	{
		lastFocusId = focusId;
	}

	focusId = _focusId;
}

void APlayerCont::BackToPrevFocus()
{
	focusId = lastFocusId;
}

/*_______________ TRANSPARENT MESHES _________________ */

void APlayerCont::initializeTransparentMeshes()
{
	for (FTransparentMeshInfo meshInfo : TransparentMeshes)
	{
		UMeshComponent* meshComp = Cast<UMeshComponent>( UResource::INS->GetComp(meshInfo.MeshId, true));
		if (meshComp)
		{
			FTransparentMesh transMeshStrutct;
			transMeshStrutct.MeshComp = meshComp;
			transMeshStrutct.DefaultMats = meshComp->GetMaterials();
			transMeshStrutct.TransparentMat = meshInfo.TransparentMat;
			listTransparentMeshes.Add(transMeshStrutct);
			
		}
	}
}

void APlayerCont::ToogleTransparentMode()
{
	transparentMode = !transparentMode;

	for (FTransparentMesh transMeshStruct : listTransparentMeshes)
	{
		if (transMeshStruct.MeshComp)
		{
			for (int8 i = 0; i < transMeshStruct.DefaultMats.Num(); i++)
			{
				if (transparentMode)
					transMeshStruct.MeshComp->SetMaterial(i, transMeshStruct.TransparentMat);
				else
					transMeshStruct.MeshComp->SetMaterial(i, transMeshStruct.DefaultMats[i]);
			}
			
			
		}
	}

	
	for (FString idObj : TransparentModeHideObjects)
	{
		AItem* item = UResource::INS->GetItem(idObj,false);
		if (item)
		{
			item->SetVisible(!transparentMode,false);
		}
		else
		{
			USceneComponent* comp = UResource::INS->GetComp(idObj, false);
			if (comp)
			{
				comp->SetVisibility(!transparentMode, true);
			}
		}
		

		
	}
};


void APlayerCont::resetValues()
{
	//APlayerCont::MoveInputDelta = FVector::ZeroVector;
	//APlayerCont::LocalRotInput = FRotator::ZeroRotator;
	//APlayerCont::MouseWheelAxis = 0;
	//APlayerCont::IsMouseLeft = false;
	//APlayerCont::KeyZ = false;
	//APlayerCont::KeyX = false;
	//APlayerCont::KeyC = false;
	//APlayerCont::KeyCtrl = false;
	//APlayerCont::KeyShift = false;
	//APlayerCont::KeyAlt = false;
}




USceneComponent* APlayerCont::GetComponent(FString _insName)
{

	USceneComponent* _comp = 0;
	if (!_insName.IsEmpty())
	{
		TArray<USceneComponent*> allComponents;
		GetComponents(allComponents, true);
		for (USceneComponent* sceneComp : allComponents)
		{
			if (sceneComp->GetName() == _insName)
			{
				_comp = sceneComp;
			}

		}
	}
	return _comp;
}


AOmActor* APlayerCont::GetChildActor(FString _compName)
{
	UChildActorComponent* caComp = Cast<UChildActorComponent>(GetComponent(_compName));
	AOmActor* actor = 0;
	if (caComp) actor = Cast<AOmActor>(caComp->GetChildActor());
	return actor;
}



/* ---------------- INPUT RECEIVE (FROM UNREAL) ----------------- */
void APlayerCont::onInputReceive(bool _status, FString _key)
{
	//EventInputAction.Broadcast(_action, _key);

	if (!AMainCont::INS->IsAppStarted) return;
	


	//if (_key == "BtnFinishOperation")
	//{
	//	//UServer::INS->SendAllMetrics();

	//}

	EventKeyAndMouse.Broadcast(_status, _key);


	
}

//static
float APlayerCont::GetKeyAxis(FString _keyName)
{
	float r = 0;
	if (APlayerCont::INS)
		r = APlayerCont::INS->playerInputComponent->GetAxisValue(*_keyName);

	return r;
}
//static
bool APlayerCont::GetKeyAction(FString _keyName)
{
	bool r = false;
	if (APlayerCont::INS)
		r = APlayerCont::INS->InputListener->GetKeyAction(_keyName);
	return r;
}

FVector APlayerCont::GetKeyAxisVector(FString _axisName)
{
	FVector v = FVector::ZeroVector;
	if (APlayerCont::INS)
		v = APlayerCont::INS->playerInputComponent->GetVectorAxisValue(*_axisName);
	return v;
}

void APlayerCont::SetupPlayerInputComponent(class UInputComponent* _InputComp)
{
	Super::SetupPlayerInputComponent(_InputComp);
	playerInputComponent = _InputComp;


	InputListener = NewObject<UInputListener>();
	InputListener->EnableMouseAndKeyboardListener(playerInputComponent);
	InputListener->EventAction.AddDynamic(this, &APlayerCont::onInputReceive);




	
	//_InputComp->Priority = -100;

	//_InputComp->BindAxis("UPDOWN", this, &APlayerCont::InputKeyAxisUpDown);
	//_InputComp->BindAxis("LEFTRIGHT", this, &APlayerCont::InputKeyAxisLeftRight);
	//_InputComp->BindAxis("KeyWS", this, &APlayerCont::InputKeyAxisWS);
	//_InputComp->BindAxis("KeyAD", this, &APlayerCont::InputKeyAxisAD);
	//_InputComp->BindAxis("CamMoveX", this, &APlayerCont::InputAxisCamMoveX);
	//_InputComp->BindAxis("CamMoveY", this, &APlayerCont::InputAxisCamMoveY);
	//_InputComp->BindAxis("CamZoom", this, &APlayerCont::InputAxisCamZoom);
	//_InputComp->BindAxis("CamRotX", this, &APlayerCont::InputAxisCamRotX);
	//_InputComp->BindAxis("CamRotY", this, &APlayerCont::InputAxisCamRotY);
	//_InputComp->BindAxis("MouseX", this, &APlayerCont::InputAxis);
	//_InputComp->BindAxis("MouseY", this, &APlayerCont::InputAxis);
	//_InputComp->BindAxis("MouseZ", this, &APlayerCont::InputAxis);
	//_InputComp->BindAxis("CamMoveZ", this, &APlayerCont::InputAxis);


	//_InputComp->BindAction("Focus", IE_Pressed, this, &APlayerCont::InputKeyFocus);
	//_InputComp->BindAction("KeyCtrl", IE_Pressed, this, &APlayerCont::InputKeyDownCtrl);
	//_InputComp->BindAction("KeyCtrl", IE_Released, this, &APlayerCont::InputKeyUpCtrl);
	//_InputComp->BindAction("KeyShift", IE_Pressed, this, &APlayerCont::InputKeyDownShift);
	//_InputComp->BindAction("KeyShift", IE_Released, this, &APlayerCont::InputKeyUpShift);
	//_InputComp->BindAction("KeyAlt", IE_Pressed, this, &APlayerCont::InputKeyDownAlt);
	//_InputComp->BindAction("KeyAlt", IE_Released, this, &APlayerCont::InputKeyUpAlt);
	//_InputComp->BindAction("Tab", IE_Pressed, this, &APlayerCont::InputKeyDownTab);
	//_InputComp->BindAction("VrLeftTrigger", IE_Pressed, this, &APlayerCont::InputKeyVrLeftTrigger);
	//_InputComp->BindAction("VrLeftTrigger", IE_Released, this, &APlayerCont::InputKeyVrLeftTriggerDown);
	//_InputComp->BindAction("VrRightTrigger", IE_Pressed, this, &APlayerCont::InputKeyVrRightTrigger);
	//_InputComp->BindAction("VrRightTrigger", IE_Released, this, &APlayerCont::InputKeyVrRightTriggerDown);
	//_InputComp->BindAction("VrRightGrip", IE_Pressed, this, &APlayerCont::InputKeyVrRightGrip);
	//_InputComp->BindAction("VrRightGrip", IE_Released, this, &APlayerCont::InputKeyVrRightGripDown);
	//_InputComp->BindAction("ShowRightVrLaser", IE_Pressed, this, &APlayerCont::InputKeyShowRVrLaserUp);
	//_InputComp->BindAction("ShowRightVrLaser", IE_Released, this, &APlayerCont::InputKeyShowRVrLaserDown);
	//_InputComp->BindAction("ShowLeftVrLaser", IE_Pressed, this, &APlayerCont::InputKeyShowLVrLaserUp);
	//_InputComp->BindAction("ShowLeftVrLaser", IE_Released, this, &APlayerCont::InputKeyShowLVrLaserDown);
	//_InputComp->BindAction("TestSendAllMetrics", IE_Pressed, this, &APlayerCont::InputTestSendAllMetricsUp);
	//_InputComp->BindAction("KeyResetCam", IE_Pressed, this, &APlayerCont::InputKeyResetCam);
	//_InputComp->BindAction("KeyCloseTheMessageScreen", IE_Pressed, this, &APlayerCont::InputCloseTheMessageScreen);
	//_InputComp->BindAction("KeyToogleTransparentMode", IE_Pressed, this, &APlayerCont::InputKeyTransparentMode);
	//
	//_InputComp->BindAction("Space", IE_Pressed, this, &APlayerCont::InputKeyDownSpace);
	//_InputComp->BindAction("Space", IE_Released, this, &APlayerCont::InputKeyUpSpace);
	//_InputComp->BindAction("KeyNum0", IE_Pressed, this, &APlayerCont::InputKeyNum0);
	//_InputComp->BindAction("KeyNum1", IE_Pressed, this, &APlayerCont::InputKeyNum1);
	//_InputComp->BindAction("KeyNum2", IE_Pressed, this, &APlayerCont::InputKeyNum2);


	//_InputComp->BindAction("MouseLeftButton", IE_Pressed, this, &APlayerCont::InputMouseLeftDown);
	//_InputComp->BindAction("MouseLeftButton", IE_Released, this, &APlayerCont::InputMouseLeftUp);
	//_InputComp->BindAction("MouseRightButton", IE_Pressed, this, &APlayerCont::InputMouseRightDown);
	//_InputComp->BindAction("MouseRightButton", IE_Released, this, &APlayerCont::InputMouseRightUp);
	
	
}





/*_______________ MOUSE CLICKS _________________ */


//
//void APlayerCont::InputMouseLeftDown()
//{
//	KeyMouseLeft = true;
//	onInputKeyHandler("MouseLeft", true);
//}
//void APlayerCont::InputMouseLeftUp()
//{
//	onInputKeyHandler("MouseLeft", false);
//	KeyMouseLeft = false;
//
//}
//
//void APlayerCont::InputMouseRightDown()
//{
//	onInputKeyHandler("MouseRight", true);
//	KeyMouseRight = true;
//}
//
//void APlayerCont::InputMouseRightUp()
//{
//	onInputKeyHandler("MouseRight", false);
//	KeyMouseRight = false;
//}






/*_______________ KEY INPUTS _________________ */

//
//void APlayerCont::InputKeyAxisLeftRight(float AxisValue)
//{
//}
//void APlayerCont::InputKeyAxisUpDown(float AxisValue)
//{
//	KeyAxisUpDown = AxisValue;
//}
//void APlayerCont::InputKeyAxisWS(float AxisValue)
//{
//	KeyAxisWS = AxisValue;
//}
//
//void APlayerCont::InputKeyAxisAD(float AxisValue)
//{
//	KeyAxisAD = AxisValue;
//}
//
//
//
//
//void APlayerCont::InputKeyDownCtrl() { 	APlayerCont::KeyCtrl = true; }
//void APlayerCont::InputKeyFocus() {  }
//void APlayerCont::InputKeyUpCtrl() { APlayerCont::KeyCtrl = false; }
//void APlayerCont::InputKeyDownShift() { APlayerCont::KeyShift = true; }
//void APlayerCont::InputKeyUpShift() { APlayerCont::KeyShift = false; }
//void APlayerCont::InputKeyDownAlt() { APlayerCont::KeyAlt = true; }
//void APlayerCont::InputKeyUpAlt() { APlayerCont::KeyAlt = false; }
//void APlayerCont::InputKeyDownSpace() { APlayerCont::KeySpace = true; }
//void APlayerCont::InputKeyUpSpace() { APlayerCont::KeySpace = false; }
//void APlayerCont::InputKeyDownTab()
//{ 
//}
//
//void APlayerCont::InputKeyVrLeftTrigger() 
//{
//	//if (!VrMenuR->GetIsMenuOpen() && VrMenuL->GetIsMenuOpen())
//	if (!VrMenuR->GetIsMenuOpen() && !VrMenuL->GetIsMenuOpen())
//	{
//		onInputKeyHandler("VrLeftTrigger", true);
//	}
//	
//}
//void APlayerCont::InputKeyVrLeftTriggerDown() 
//{ 
//	if (!VrMenuR->GetIsMenuOpen() && !VrMenuL->GetIsMenuOpen())
//	{
//		onInputKeyHandler("VrLeftTrigger", false);
//	}
//}
//
//
//void APlayerCont::InputKeyVrRightTrigger()
//{
//	//if (!VrMenuR->GetIsMenuOpen() && VrMenuL->GetIsMenuOpen())
//	if (!VrMenuR->GetIsMenuOpen() && !VrMenuL->GetIsMenuOpen())
//	{
//		onInputKeyHandler("VrRightTrigger", true);
//	}
//
//}
//void APlayerCont::InputKeyVrRightTriggerDown()
//{
//	if (!VrMenuR->GetIsMenuOpen() && !VrMenuL->GetIsMenuOpen())
//	{
//		onInputKeyHandler("VrRightTrigger", false);
//	}
//}
//
//void APlayerCont::InputKeyVrRightGrip()
//{
//	if (!VrMenuR->GetIsMenuOpen() && !VrMenuL->GetIsMenuOpen())
//	{
//		onInputKeyHandler("VrRightGrip", true);
//	}
//
//}
//void APlayerCont::InputKeyVrRightGripDown()
//{
//	if (!VrMenuR->GetIsMenuOpen() && !VrMenuL->GetIsMenuOpen())
//	{
//		onInputKeyHandler("VrRightGrip", false);
//	}
//}
//
//void APlayerCont::InputTestSendAllMetricsUp()
//{
//	onInputKeyHandler("BtnFinishOperation", true);
//}
//
//void APlayerCont::InputCloseTheMessageScreen()
//{
//	AMainCont::INS->Hud->BpEventRun("func:(HideMessage)");
//}
//
//
//
//void APlayerCont::InputKeyShowRVrLaserUp()
//{
//	KeyShowRVrLaser = true;
//}
//void APlayerCont::InputKeyShowRVrLaserDown()
//{
//	KeyShowRVrLaser = false;
//}
//
//void APlayerCont::InputKeyShowLVrLaserUp()
//{
//	KeyShowLVrLaser = true;
//}
//void APlayerCont::InputKeyShowLVrLaserDown()
//{
//	KeyShowLVrLaser = false;
//}
//
//
//void APlayerCont::InputKeyResetCam()
//{
//	if(Cam) Cam->ResetCamPosition();
//}
//
//
//
//void APlayerCont::InputKeyTransparentMode()
//{
//	ToogleTransparentMode();
//}
//
//
//
//void APlayerCont::InputKeyNum0() { AUICont::INS->ToogleDebugHud(); }
//void APlayerCont::InputKeyNum1()
//{
//	if (AMainCont::INS->DebugMode > 0)
//	{
//		/*if (KeyBtnSubmit)
//		{
//			UTaskCont::INS->GoToTask(UTaskCont::INS->currTask->Index - 1);
//		}*/
//	}
//	else
//	{
//	}
//}
//
//void APlayerCont::InputKeyNum2()
//{
//	if (AMainCont::INS->DebugMode > 0)
//	{
//		/*if (KeyBtnSubmit)
//		{
//			UTaskCont::INS->GoToTask(UTaskCont::INS->currTask->Index + 1);
//		}*/
//	}
//	else
//	{
//	}
//}
//		

















//#####################################################################################
//									[ InputListener ]
//#####################################################################################


UInputListener::UInputListener()
{

}



void UInputListener::EnableMouseAndKeyboardListener(class UInputComponent* _inputComponent)
{
	inputComponent = _inputComponent;
	addActionMappings("Action");
	addActionMappings("Axis");
	//inputComponent->BindAction<FInputActionBindEvent>(FName("ActionTest1"), IE_Pressed, this, &UInputListener::OnInputActionListener, EInputEvent::IE_Pressed, FString("ActionTest1"));
}



void UInputListener::Update(float _deltaTime)
{
	if (inputComponent)
	{
		for (FString axisKeys : listAxisNames)
		{
			updateFloat(axisKeys, inputComponent->GetAxisValue(*axisKeys));
		}

	}
}



void UInputListener::OnInputAxisListener(float _axis)
{

}

void UInputListener::OnInputActionListener(EInputEvent _type, FString _keyName)
{
	if (_type == EInputEvent::IE_Pressed)
	{
		updateFloat(_keyName, 1);
		EventAction.Broadcast(true, _keyName);
	}
	if (_type == EInputEvent::IE_Released)
	{
		updateFloat(_keyName, 0);
		EventAction.Broadcast(false, _keyName);
	}


}

void UInputListener::addActionMappings(FString _type)
{
	TArray<FString> arr;
	FString iniKey = _type + "Mappings";
	FString iniPropertyName = _type + "Name";
	GConfig->GetArray(TEXT("/Script/Engine.InputSettings"), *iniKey, arr, GInputIni);
	for (FString s : arr)
	{
		TArray<FString> parse1 = UOmUtils::ParseString(s, "(" + iniPropertyName + "=\"");
	
		TArray<FString> parse2 = UOmUtils::ParseString(parse1[0], "\",");
		FString inputName = parse2[0];

		if (_type == "Action")
		{
			if (!listActionNames.Contains(inputName))
			{
				inputComponent->BindAction<FInputActionBindEvent>(FName(*inputName), IE_Pressed, this, &UInputListener::OnInputActionListener, EInputEvent::IE_Pressed, inputName);
				inputComponent->BindAction<FInputActionBindEvent>(FName(*inputName), IE_Released, this, &UInputListener::OnInputActionListener, EInputEvent::IE_Released, inputName);
				listActionNames.Add(inputName);
			}
			
		}
		if (_type == "Axis")
		{
			

			if (!listAxisNames.Contains(inputName))
			{
				inputComponent->BindAxis(FName(*inputName));
				listAxisNames.Add(inputName);
			}

		}



	}


}


//
//float UInputListener::GetKeyAxis(FString _keyName)
//{
//	float r = 0;
//	if (listFloats.Contains(_keyName))
//		r = listFloats[_keyName];
//	return r;
//}

bool UInputListener::GetKeyAction(FString _keyName)
{
	bool r = false;
	if (listFloats.Contains(_keyName))
		r = ((int)listFloats[_keyName] == 1);
	return r;
}

FVector UInputListener::GetVector(FString _keyName)
{
	FVector r = FVector::ZeroVector;
	return r;
}

void UInputListener::updateFloat(FString _key, float _value)
{
	if (listFloats.Contains(_key))
		listFloats[_key] = _value;
	else
		listFloats.Add(_key, _value);
}


