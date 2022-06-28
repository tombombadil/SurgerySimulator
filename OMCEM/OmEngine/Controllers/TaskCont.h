#pragma once
#include <vector>
#include "Object.h"
#include "Process.h"
#include "OmEngine/Controllers/Observer.h"
#include "OmEngine/Controllers/Process.h"
#include "OmEngine/Utils/CoreServer.h"
#include "OmEngine/Utils/Server.h"
#include "OmEngine/Core/OmObject.h"
#include "TaskCont.generated.h"

class  UObserver;
class UProcess;
class UTaskCont;
//class UOmComp;
class UResource;

using namespace std;





/* TASK */
UCLASS(BlueprintType)
class OMCEM_API UTask : public UOmObject
{
	GENERATED_BODY()
private:
	UPROPERTY() TArray<UProcess*> listProcess;
public:
	static FString IDPREFIX;

	
	UPROPERTY() FString Label;
	UPROPERTY() bool isAutoNext;
	UPROPERTY() int Index;
	UPROPERTY() int8 CountTotalProcess;
	UPROPERTY() bool IsCompleted;
	UPROPERTY() bool IsShowInTaskMenu;


	UTask()
	{ 
		//ebug(">task("+FString::FromInt(Index)+") created -> " + Name ); 
	};
	static UTask* Create(FString _id, FString _label="", bool _autoNext=true);
	//static UTask* Create(FString _id, FMetric _metric, FString _label="", bool _autoNext=true);
	//void AddTaskProcess(UProcess* _process, int8 _index = -1);
	void AddTaskProcess(UProcess* _process, int8 _index);
	UProcess* GetProcess(int _index);
	TArray<UProcess*> GetProcesses();
	void Reset();
	//void UpdateProcesses();

	

};



/* #TASKCONTROLLER */
UCLASS(BlueprintType, Blueprintable)
class OMCEM_API UTaskCont : public UOmObject
{
	GENERATED_BODY()
public:
	UTaskCont();
	static UTaskCont* INS;
	 FEventProcessStatus EventStatus;
	UPROPERTY() UTask* currTask;
	//UPROPERTY() UProcess* currProcess;
	UPROPERTY() UProcess* currProcess;
	UPROPERTY() UObserver* observer;
	UPROPERTY() FString IdOperation; //generic
	virtual void Init();
	
	/*---------  base funcs ------ */
	//void AddTask(UTask* _task, bool isShowTaskMenu = true, int8 _insertIndex = -1);
	void AddTask(UTask* _task, bool isShowTaskMenu, int8 _insertIndex = -1); //oncesinde en son nvagriliyordu. simdi processler eklenmeden cagriliyor.
	void AddTaskToNext(UTask* _task, FString _targetPRevTaskId, bool isShowTaskMenu);
	UFUNCTION(BlueprintCallable, Category = "OM3") UTask* GetTask(FString _id);
	UTask* GetTask(int8 _index);
	FString GetTaskLabel(FString _id);
	UFUNCTION(BlueprintCallable, Category = "OM3")  URule* GetRule(FString _id);
	//UPModel* GetModel(FString _id);
	void GenerateOperationId();
	void PrintMsg(bool _type, int8 _status, FString _msg);
	
	//UProcess* AddProcess(UTask* _task, UProcess* _process, int8 _index =-1);
	UProcess* AddProcess(UTask* _task, UProcess* _process, int8 _index = -1);
	//UProcess* AddProcess(UTask* _task, FString _cmd , int8 _index =-1);
	//UProcess* AddProcess(UTask* _task, vector<FString> _commands);
	//UPModel* AddModel(FOmValue _cmd);
	//URule* CreateRule(UPModel* _model, FString _comparison, FOmValues _command, FString _msg = "", bool _isSkippabble = true, bool _isPrint = true, int8 _printAfterCounter = 0);
	
	URule* CreateRule(FString _comparison, FOmValues _command, FString _msg, bool _isSkippabble);
	void SetTask(FString _index);
	void GoToTask(int8 _task);
	void onTaskStatus(EStatus _status);
	void SetProces(int _process);
	void onProcessStatus(EStatus _status);
	UFUNCTION() void onEventProcess(EEventType _eventType, FProcessResult _result);
	void GoBackTask(FString _idTask);
	void RunCommands(FOmValues _commands);
	UFUNCTION(BlueprintCallable, Category = "OM3")  class AMainCont* GetMainCont();
	UPROPERTY() bool IsAIMode = false;
protected:

	UPROPERTY() TArray<UTask*> listTasks;
	UPROPERTY() TMap<FString, UProcess*> listProcesses;
	//UPROPERTY() TMap<FString, UPModel*> listModels;
	UPROPERTY() TMap<FString, URule*> listRules;
	virtual void InitTasks();
	//static void onPopupClosed(UEvent &e);
	void valitadeAllTasks();
	URule* getProcessResult(UProcess* _process);
	void onTaskResult(bool _status, FString _msg = "");



	/* @RayFom ve @RayTo tool'larin idCat'leri */
	UFUNCTION(BlueprintCallable, Category="OM3")  UProcess* CreateRayProcess(UTask* Task, FString RayFrom, FString RayTo);
	
	UFUNCTION(BlueprintCallable, Category = "OM3")  UProcess* CreateFieldProcess(UTask* Task, FString IdTool, FString IdProcessComp,  FString StartParams, FString SuccComparison,  bool IsTakeTool = true, int MetricAction = 2);
	//UProcess* HelperCreateHitProcess(UPModel* _model);
	/* @_metricAction : 0(none) 1(just onstarted) 2(just onCompleted) 3(bothstart ve completed) */
	//UProcess* HelperCreatePawnProcess(UTask* &_task, UPModel* _model, FString _succComparison, FString _errComparison, FString _errMsg, FString _params, bool _callStartCmdOnProcessComp = true, int8 _metricAction = 2);
	/* bir @_task'a  bir @_eq eklenmesini saglayan processleri ekleyen template */ 
	void HelperMakeInsertEquipment(UTask* &_task, AToolItem* _tool, AToolItem* _eq, class UOmComp* _collisionComp, UProcessComp* _pawn,  FString _pawnSuccComparison="", FString _addProcessParam="");
	/* bir collision model yaratir */
	//UPModel* HelperCreateCollisionModel(FString _from, FString _to);
	//UPModel* HelperCreateHitModel(FString _from, FString _to, FString _hitTime);
	void  HelperTakePart(bool _isTake, UTask* &_task, FPartFieldInfo _partField, FMetric _metric);
	//void HelperBrowseXRay(UTask* &_task, URule* _checkRull=0, UTask* _goBackTaskOnErr=0);
	//void setUi();

	void runCommand(FOmValue _cmd);



	
	void dispatchErr(FString _id);




	// BLUEPRINT

public:
	UFUNCTION (BlueprintCallable, Category="OM3") UTask* CreateTask(FString TaskId, FString TaskLabel, bool AutoNextTask=true);
	UFUNCTION(BlueprintCallable, Category = "OM3") UProcess* CreateProcess(UTask* Task);
	//UFUNCTION (BlueprintCallable, Category="OM3") UTask* CreateRule(FString TaskId, FString TaskLabel, bool AutoNextTask);
	//UFUNCTION (BlueprintCallable, Category = "OM3")  UProcess* CreateProcess(UTask* Task, FString Comparison, FString StartCommand, FString SuccedCommand, FString FailComman);
	UFUNCTION (BlueprintCallable, Category = "OM3") URule* CreateRule(UTask* Task, UProcess* Process=0, FString Comparison="", FString SuccedCommand="", int DefaultReturnValue = 0, bool IsOfflineRule = false);
	UFUNCTION (BlueprintCallable, Category = "OM3")  UTask* CreateXrayTask(FString TaskId, FString TaskLabel, UTask* GoBackTaskWhenHasError, FString IdTool, FString IdMeshList);
	UFUNCTION(BlueprintCallable, Category = "OM3")  	void CreateInsertToolPartProcess(UTask* Task, FString IdTool, FString IdToolPart, FString IdVolumeComp, FString IdProcessComp, FString StartParams, FString SuccComparison, int MetricAction);

	UFUNCTION(BlueprintCallable, Category = "OM3")  UTask* CreateTakeFieldTask(FString TaskId, FString TaskLabel, FString IdTool, FString IdField, FString IdCatPart, FString IdVolumeComp, FString IdProcessComp, FString StartParams, FString SuccComparison, int MetricAction);




	
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventCreateTasks();
};
