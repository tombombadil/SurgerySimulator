#include "MainCont.h"
#include "OMCEM.h"
#include "OmEngine/Controllers/TaskCont.h"
#include "OmEngine/Controllers/XRayRender.h"
#include "OmEngine/Items/Item.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Utils/OmValue.h"
#include "OmEngine/Controllers/ToolsCont.h"
#include "OmEngine/Controllers/OmHud.h"
#include "OmEngine/Inputs/Inputs.h"
#include "OmEngine/Utils/Config.h"
#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "CameraCont.h"
#include "PlayerCont.h"
#include "OMCEMGameModeBase.h"
#include "BleedingCont.h"
#include "../Utils/CoreServer.h"
#include "../Components/TransparentContComp.h"


AMainCont * AMainCont::INS = NULL;


AMainCont::AMainCont()
{
	PrimaryActorTick.bCanEverTick = true;
	AMainCont::INS = this;
	IsAppStarted = false;
	IsAppInited = false;
	TooltipXrayLoc = FVector(-34, -177, 163);
	for (int8 i = 0; i < 10; i++)
	{
		ListToolsKeyShortcut.Add("none");
	}

	//ClassResource = UResource::StaticClass();
	//ClassTaskCont = UTaskCont::StaticClass();
	
}

void AMainCont::BeginPlay() 
{ 
	//UGameUserSettings* gameUserSettings = GEngine->GameUserSettings;
	//gameUserSettings->SetFullscreenMode(EWindowMode::Windowed);
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenMode(ESpectatorScreenMode::SingleEyeCroppedToFill);


	createControllers();

	Hud = Cast<AOmHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	if (!Hud)
	{
		Debug::Warning("Hud Class not delivered from AOmHud class");
		return;
	}

	//GetWorld()->Exec(GetWorld(), TEXT("stat gpu"));
	
	if (!IsEnable) return;
	
	UConfig::OBSERVER_PVALUE_TOL = 1;
	
	bool authMacResult = false;
	if (AuthorizedMacAddresses.Num() > 0 && IsSecureMacAddress)
	{
		for (FString authMac : AuthorizedMacAddresses)
		{
			if (authMac.ToLower() == FGenericPlatformMisc::GetMacAddressString().ToLower())
			{
				authMacResult = true;
			}
				
		}
	}
	else
	{
		authMacResult = true;
	}
	
	Debug("macAddress GetMacAddressString : " + FGenericPlatformMisc::GetMacAddressString() + " authMacResult: ", authMacResult);
	
	if (InputMode == 3)
		StartSelectTool = "";
	
	FTimerHandle timerHandle;
	if (authMacResult) GetWorldTimerManager().SetTimer(timerHandle, this, &AMainCont::delayedBeginPlay, 0.4, false);
	//if(authMacResult) delayedBeginPlay();
	else 	Debug::Warning("You are not allowed to run this software");
}







void AMainCont::delayedBeginPlay()
{
	/* init & load resource */
	//resource = NewObject<UResource>();
	
	resource->EventOnStatus.AddDynamic(this, &AMainCont::onResourcesLoaded);
	resource->Load();



	taskCont->Init();
}


void AMainCont::onResourcesLoaded(EEventType _eventType)
{
	if (_eventType != EEventType::COMPLETE) return;
	
	/* set configs */
	{
		int confDebugMode =1, confLevelMode =-1, confStartTaskIndex = -1;
		int confIsVr = 1, confInputMode = -1, confEnableMetric = -1;
		FString confToken;


		//if (GetWorld()->WorldType == EWorldType::Game) 
		//{
		//	GConfig->GetInt(TEXT("Config"), TEXT("LevelMode"), confLevelMode, GGameIni);
		//	//GConfig->GetInt(TEXT("Config"), TEXT("StartTaskIndex"), confStartTaskIndex, GGameIni);
		//	GConfig->GetInt(TEXT("Config"), TEXT("EnableMetric"), confEnableMetric, GGameIni);

		//}


		confStartTaskIndex = UResource::INS->GetJsonInt(UResource::INS->GetOpJsonObj("Config"), "StartTask");
		confLevelMode = UResource::INS->GetJsonInt(UResource::INS->GetOpJsonObj("Config"), "LevelMode");
		confDebugMode = UResource::INS->GetJsonInt(UResource::INS->GetOpJsonObj("Config"), "DebugMode");
		confEnableMetric = UResource::INS->GetJsonInt(UResource::INS->GetOpJsonObj("Config"), "EnableMetric");
		confIsVr = UResource::INS->GetJsonInt(UResource::INS->GetOpJsonObj("Config"), "EnableVr");
		confInputMode = UResource::INS->GetJsonInt(UResource::INS->GetOpJsonObj("Config"), "InputMode");
		Version = UResource::INS->GetJsonString(UResource::INS->GetOpJsonObj("Config"), "Version");
		//confLevelMode = UResource::INS->GetJsonInt(UResource::INS->GetJsonObject("Config"), "LevelMode");
		FVector confStartPos = UResource::INS->GetJsonVector(UResource::INS->GetOpJsonObj("Config"), "PlayerPosition");
		FVector confRightMovePos = UResource::INS->GetJsonVector(UResource::INS->GetOpJsonObj("Config"), "RightMovePosition");
	
		if (GetWorld()->WorldType == EWorldType::Game) 
		{
			if (confStartTaskIndex > -1) StartTask = confStartTaskIndex;
			if (confDebugMode > -1) DebugMode = confDebugMode;
			if (confLevelMode > -1) UConfig::INS->LevelMode = confLevelMode;
			if (confIsVr > -1) IsVRMode = (confIsVr>0);
			if (confInputMode > -1) InputMode = confInputMode;
			if (confEnableMetric > -1) IsEnableMetric = (confEnableMetric >0);
			APlayerCont::INS->SetActorLocation(confStartPos);
			HapticRightLoc = confRightMovePos;
			//AMoveCont* moveRight = APlayerCont::INS->GetMoveCont("MoveR");
			//if(moveRight) moveRight->SetActorLocation( confRightMovePos);
			
		}

		 GConfig->GetString(TEXT("Config"), TEXT("LoginToken"), LoginToken, GGameIni);
		Debug("LoginToken " + LoginToken);

		UConfig::INS->AddDebugParam("OMCEM.conf.LevelMode ", FString::FromInt(UConfig::INS->LevelMode));
		UConfig::INS->AddDebugParam("OMCEM.conf.confStartTaskIndex ", FString::FromInt(StartTask));
		UConfig::INS->AddDebugParam("Main::DebugMode", FString::FromInt(DebugMode));
		UConfig::INS->AddDebugParam("Main::IsVRMode", FString::FromInt(IsVRMode));
		UConfig::INS->AddDebugParam("Main::InputMode", FString::FromInt(InputMode));
		UConfig::INS->AddDebugParam("Main::PlayerPosition" , confStartPos.ToString());
	}
	
	
	if (InputMode == 1 || InputMode == 2) //haptic
	{
		UHapticDevice::INS = NewObject<UHapticDevice>();
		
		if (GetWorld()->WorldType == EWorldType::Game)
		{
			FVector confHapticUnitScale = UResource::INS->GetJsonVector(UResource::INS->GetOpJsonObj("Config"), "HapticUnitScale");
			UHapticDevice::INS->PositionUnitScale = confHapticUnitScale;
		}
		
		UHapticDevice::INS->Init(InputMode > 1);

	}

	APlayerCont::INS->Init();

	

	//if (GetWorld()->WorldType == EWorldType::Game)
	{
		//FVector confRightMovePos = UResource::INS->GetJsonVector(UResource::INS->GetOpJsonObj("Config"), "RightMovePosition");
		//AMoveCont* moveRight = APlayerCont::INS->GetMoveCont("MoveR");
		//if (moveRight) 
			//moveRight->GetRootComponent()->GetAttachParent()->SetWorldLocation(confRightMovePos);



	}

	

	

	//UResource::INS->MOVE_AGENT = NewObject<UMoveAgent>();
	//UResource::INS->MOVE_AGENT->Init();




	toolCont = NewObject<UToolsCont>();
	Tween = NewObject<UTween>();
	Tween->Init();


	resource->Init();
	UiCont->Init();


	


	toolCont->Init();

	

	for (TActorIterator<AActor> act(AMainCont::INS->GetWorld()); act; ++act)
	{
		AActor* actor = Cast<AActor>(*act);

		if (actor)
		{
			TArray <UBoxComponent*> listallBoxes;
			actor->GetComponents(listallBoxes);
			for (UBoxComponent* b : listallBoxes)
			{
				if (DebugMode < 1) b->SetHiddenInGame(true);
			}

		}



	}


	{
		UCameraCont* camCont = Cast<UCameraCont >(UResource::INS->GetComp(APlayerCont::INS, "CameraCont"));
		if (camCont)
		{
			camCont->Init();
			camCont->SetRelativeLocation(FVector::ZeroVector);
		}
		else
		{
			Debug::Warning("CameraCont PlayerCont icinde bulunamadi!");
		}

	}


	APlayerCont::INS->AfterInit();
	
	Super::BeginPlay();


	GetWorldTimerManager().SetTimer(UnusedHandle, this, &AMainCont::Start, 1, false);


}





void AMainCont::Start()
{
	if (!IsEnable) return;
	IsAppInited = true;
	Hud->BpEventRun("func:(HideDisable)");
	AOMCEMGameModeBase* gameMode = Cast<AOMCEMGameModeBase>(GetWorld()->GetAuthGameMode());
	UiCont->WidgetTaskMonitor->SetVisible(true);
	if(bleedingCont) bleedingCont->Init(BleedingData);
	AUICont::INS->ShowMessage(UConfig::GET_MSG("WELCOME"), 1, 0);
	APlayerCont::INS->MoveR->Input->EventInput.AddDynamic(this, &AMainCont::onLeftInputKeyHandler);
	APlayerCont::INS->TakeTool("MoveR", AMainCont::INS->StartSelectTool);
	BpEventInit();
	IsAppStarted = true;

	for (AItem* item : UResource::INS->GetItems())
	{
		item->Start();
	}
	
	AXRayRender::INS->RefreshCaptures();
}

void AMainCont::StartOperation()
{
	Debug("StartOperation");
	if (!IsOperationStarted)
	{
		UTaskCont::INS->GenerateOperationId();


		UServer::INS->SendMetric(FMetricJson::CreateStartOperation());

		for (AItem* item : UResource::INS->GetItems())
		{
			item->StartOperation();
		}
		
		
		

	
		taskCont->GoToTask(StartTask);
		
		IsOperationStarted = true;

		BpEventStart();

		AUICont::INS->HideMessage();

		APlayerCont::INS->Start();


		
	

		
	}
}

void AMainCont::FinishOperation() //click  by user
{
	if (IsOperationStarted)
	{
		FGenericPlatformMisc::RequestExit(false);
	}
}


void AMainCont::Exit()
{
	if(GetWorld()->WorldType == EWorldType::Game)
		FGenericPlatformMisc::RequestExit(true);
}

void AMainCont::Restart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


void AMainCont::createControllers()
{
	UiCont = GetWorld()->SpawnActor<AUICont>(ClassUIController, FVector::ZeroVector, FRotator::ZeroRotator);
	taskCont = NewObject<UTaskCont>(this, ClassTaskController);
	resource = NewObject<UResource>(UResource::StaticClass());
	bleedingCont = NewObject<UBleedingCont>();
}


void AMainCont::Tick(float DeltaTime)
{
	if (!IsAppInited) return;
	Super::Tick(DeltaTime);

	if (Tween) Tween->Render(DeltaTime);


	toolCont->Render(DeltaTime);
	if(bleedingCont) bleedingCont->Render(DeltaTime);


	if (APlayerCont::INS) APlayerCont::INS->Update(DeltaTime);
	//if (UResource::INS->MOVE_AGENT) 	UResource::INS->MOVE_AGENT->Update(DeltaTime);

	

}


void AMainCont::onServerAllMetricResponse(FAllMetrics_Response _response)
{
	FinishOperation();
}



void AMainCont::Run(FOmValue cmd)
{
	FString strCmd = cmd.ToString();
	BpEventRun(strCmd); 
	FString func = cmd.GetValue("func").GetString(0);
}






void AMainCont::CompletedOperation()
{
	AUICont::INS->ShowMessage(UConfig::GET_MSG("operation_completed"), 1, 0);
}



void AMainCont::onLeftInputKeyHandler(FInputInfo _info)
{
	if (_info.Value == 1)
	{
		if (_info.KeyPreset == "KeyProcess")
		{
			if (!IsOperationStarted)
				StartOperation();
		}
	}
	
}




void AMainCont::SetTransparentMode(bool _status, TArray<FString> _items, FString _transparentCompTag)
{
	TransparentMode = _status;
	for (FString itemId : _items)
	{
		AItem* item = UResource::INS->GetItem(itemId);
		if (item)
		{
			FString cmd = _status ? "EnableTransparent" : "DisableTransparent";
			item->BpEventRun("func:(" + cmd + ")");
			item->BpEventRun("func:(" + cmd + ")");
		}
	}



	for (AItem* item : UResource::INS->GetItems())
	{
		UTransparentContComp* comp = Cast< UTransparentContComp>(item->GetComponentByClass(UTransparentContComp::StaticClass()));
		if (comp)
		{
			comp->SetEnableTransparent(_status, _transparentCompTag);
		}
	}


}