#pragma once

#include "GameFramework/HUD.h"
#include "OmEngine/Ui/UIPopup.h"
#include "OmEngine/Ui/UIToolMenu.h"
#include "UserWidget.h"
#include "WidgetComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "OmEngine/Items/OmActor.h"
#include "OmEngine/Utils/Tween.h"
#include "UICont.generated.h"


class UUIToolMenu;
class UToolWidgetProgress;
class UHudWidgetComp;
class UUIPopup;
class UUITaskBar;
class UUIXRay;
class UOmWidgetComp;
class AToolItem;


UENUM()
enum class EWidgetId : uint8
{
	NONE,
	TOOL_MENU  UMETA(DisplayName = "TOOL_MENU"),
	TOOL_ANGLE_X  UMETA(DisplayName = "TOOL_ANGLE_X"),
	TOOL_ANGLE_Y  UMETA(DisplayName = "TOOL_ANGLE_Y"),
	TOOL_PROGRESS  UMETA(DisplayName = "TOOL_PROGRESS"),
	POPUP  UMETA(DisplayName = "POPUP"),
	XRAY,
	STATUS_MONITOR,
	TOOLTIP_TOOLNAME,
	TOOLTIP_XRAY,
	DEBUG
};



USTRUCT()
struct OMCEM_API FOmWidget
{
	GENERATED_USTRUCT_BODY()

	class UOmWidgetComp* Widget;
	int8 IdGroup = -1;
	EWidgetId Id;
	bool _isActive;
	bool IsValid = false;
	FOmWidget() { IsValid = true; };
	FOmWidget(EWidgetId _id, int8 _idGroup, UOmWidgetComp* _w = 0)
	{
		FOmWidget();
		Id = _id;
		IdGroup = _idGroup;
		Widget = _w;
	};

	
	void SetActive(bool _status, bool _visibilty);
	bool GetActive() {
		return _isActive;
	}

	

};



UCLASS()
class OMCEM_API AUICont : public AOmActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) USceneComponent* RootTool;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) USceneComponent* RootHud;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UToolWidgetProgress* WidgetToolAngleX;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UToolWidgetProgress* WidgetToolAngleY;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UToolWidgetProgress* WidgetToolProgress;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UHudWidgetComp* WidgetPopup;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UOmWidgetComp* WidgetDebug;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UOmWidgetComp* WidgetXRay;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UOmWidgetComp* WidgetXRayTooltip;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UOmWidgetComp* WidgetTaskMonitor;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) UOmWidgetComp* WidgetDiscCam;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) AOmActor* TooltipToolname;
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) AOmActor* TooltipXRay;

	UPROPERTY() UUIPopup* uiPopup;
	UPROPERTY() UUIXRay* UiXray;
	UPROPERTY() UUIToolMenu* uiToolMenu;
	UPROPERTY() UUIBase * currUI;
	//UPROPERTY() AToolItem* currOperationsTool;



	bool isFocus;
	bool IsOpenToolMenu;
	bool IsActiveToolSet;
	bool isXRayOpened;
	static AUICont* INS;

	AUICont(); 
	virtual void Init();
	void SetTooltipActivity(EWidgetId _idTooltipActor, bool _visible, FString _msg="", int8 _status=0);
	void SetToolWidgetRootLoc(FTransform _trans);
	virtual void Tick(float DeltaSeconds) override;
	void SetActiveWidget(EWidgetId _idWidget, bool _activity, bool _visiblty);
	
	FOmWidget GetWidget(EWidgetId _id);
	bool GetWidgetIsActive(EWidgetId _id);
	void UpdateTaskMonitor(FString _currTaskName, FString _nextTaskName);
	void ShowMessage(FString _msg, int8 _msgStatus, float _autoCloseTimer);
	void HideMessage();
	void PrintOnStatusBar(FString _msg, int8 _type);
	void SetSelectToolByRay(AToolItem* _toolItem);
	void OpenToolMenu(bool _status, int8 _toolMenuOpenKey = 0, bool _selectTool = true);
	void ToogleFocus();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventOnFocus(bool _focus);
	//AToolItem* GetCurrSelectedEqOnDesk() { return (AToolItem*) currSelectEqByRay;  };
	void SetFocus(bool _status);
	
	void ToogleDebugHud();
private:
	/* 0'dan buyuk degerlerde key uyusmazligi varsa open ve close komutlari yok sayilir. TaskCont un actigi menuyu baska bir yerden yanlislikla kapanmamasi icin bir pattern.. */
	int8 toolMenuOpenKey = 0;
	FActorTween twSurToolset;
	FOmWidget activeWidget;
	UPROPERTY() TArray<FOmWidget> listWidgets;

	
	/* vr */
	UPROPERTY() AToolItem* currSelectToolByRay;
	UPROPERTY() FTransform SurToolSetDefaultTrans;
	UFUNCTION() void onTaskProcessHandler(EEventType _eventType, FProcessResult _processresult);
	bool isViewDebugHud;
	bool isToolMenuOpened; //gecici
	/* timer */
	FTimerDelegate printMsgtimerDel;
	FTimerHandle printMsgtimer;
	UFUNCTION()	void onPrintMsgTimer();
	
};
