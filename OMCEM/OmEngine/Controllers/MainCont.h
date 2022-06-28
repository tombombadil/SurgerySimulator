#pragma once

#include "GameFramework/Pawn.h"
#include "OmEngine/Utils/OmStructs.h"
#include "OmEngine/Utils/Server.h"
#include "OmEngine/Utils/OmValue.h"
#include <Materials/Material.h>
#include "MainCont.generated.h"

class AToolItem;
class AXRayRender;
class UToolsCont;
class UTaskCont;
class UToolsCont;
class UResource;
class UConfig;
class UToolsCont;
class AUICont;
class APlayerCont;
class UResource;
class AToolItem;
class UBleedingCont;
class UServer;
class UTween;
class AOmHUD;



/*
Kisaltmalar :
- Om : Omcem
- Cont : Controller Classlarý
- Comp : Component
- FEvent -> delegate(event) ler
*/


UCLASS()
//class OMCEM_API AMainCont : public APawn
class OMCEM_API AMainCont : public AActor
{
	GENERATED_BODY()

public:
	/* params */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) TArray<FString> AuthorizedMacAddresses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) bool IsVRMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) bool IsEnable = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) bool IsEnableMetric = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) bool IsSecureMacAddress = false;
	UPROPERTY(EditAnywhere, Category = OMCEM) FString Version;
	/*InputMode -1(vr) 0(mouse) 1(single haptic) 2(multiple haptic) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) int InputMode = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) AItem* OperationRoot;
	/* containercomponent id in tray on table */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) FString IdTrayInToolset;
	//UPROPERTY(EditAnywhere, Category = OMCEM) TArray<AToolItem*> ArrToolItems;
	UPROPERTY(EditAnywhere, Category = OMCEM) AXRayRender* XRay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Haptic") FVector HapticRightLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Haptic") FVector HapticLeftLoc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Config") TSubclassOf<class AUICont> ClassUIController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Config") TSubclassOf<class UTaskCont> ClassTaskController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Config") UMaterial* MAT_HIGHLIGHTTOOL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM|Config") int DebugLogLevel = 0;


	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UResource* resource;

	UPROPERTY(BlueprintReadWrite, Category = OMCEM) AOmHUD* Hud;
	UPROPERTY(EditAnywhere, Category = OMCEM, Meta = (MakeEditWidget = true)) FVector TooltipXrayLoc;
	UPROPERTY(EditAnywhere, Category = OMCEM) int StartTask = 0;
	UPROPERTY(EditAnywhere, Category = OMCEM) FString StartSelectTool = "Burr";
	UPROPERTY(EditAnywhere, Category = OMCEM) AItem* Toolset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM, Meta = (MakeEditWidget = true)) FVector LocToolsetToPlayer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM, Meta = (MakeEditWidget = true)) FTransform TransCArmToPatient;
	/* tool realign position size of away from cam*/
	UPROPERTY(EditAnywhere, Category = OMCEM) TArray<FString> ListToolsKeyShortcut;
	/* 
	Debug Mode :
	[0] -> Release! No Debug.
	[1] -> Sounds Active
	[2] -> All Sounds Deactive, +Debug Lines Active.
	[3] -> Skip some task (like control hole, depth etx..) 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) int DebugMode = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) TArray<FString> BleedingData;
	/* blood values/second */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) float BleedingPerBloodValue = 0.1;
	UPROPERTY(EditAnywhere, Category = OMCEM) TArray<float> BleedingLevelTimes;
	/* variables */
	static AMainCont* INS;
	UPROPERTY() bool IsAppInited;
	UPROPERTY() bool IsAppStarted;
	UPROPERTY(BlueprintReadOnly) bool IsOperationStarted;;
	UPROPERTY() FString LoginToken;
	UPROPERTY(BlueprintReadOnly) AUICont* UiCont;
	UPROPERTY(BlueprintReadOnly) UTween* Tween;
	UPROPERTY(BlueprintReadOnly, Category = "OM3") bool TransparentMode;
	//UPROPERTY(BlueprintReadOnly) FOperation Operation;
	/* func's */	
	AMainCont();
	virtual void BeginPlay() override;
	
	virtual void StartOperation();
	void FinishOperation();
	
	UFUNCTION(BlueprintCallable, Category = OMCEM) void Exit();
	UFUNCTION(BlueprintCallable, Category = OMCEM) void Restart();
	void CompletedOperation();
	
	virtual void Tick(float DeltaSeconds) override;
	
	
	void Run(FOmValue cmd);
	//void PrintErr(FString _errMsg);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString &cmd);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventInit();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventStart(); 
	UFUNCTION() void onResourcesLoaded(EEventType _eventType);
	 void SetTransparentMode(bool _status, TArray<FString> _items, FString _transparentCompTag);
protected:
	UFUNCTION() virtual void Start();
	virtual void createControllers();

	FTimerHandle UnusedHandle;
	UPROPERTY() UToolsCont* toolCont;
	UPROPERTY() UTaskCont* taskCont;
	
	UPROPERTY() UBleedingCont* bleedingCont;
	UFUNCTION() void onServerAllMetricResponse(struct FAllMetrics_Response _response);
	virtual void delayedBeginPlay();


private:
	UFUNCTION() void onLeftInputKeyHandler(FInputInfo _info);
	
	
};
