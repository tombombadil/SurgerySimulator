#include "TaskCont.h"
#include "OMCEM.h"
#include<stdarg.h>
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Utils/Server.h"
#include "ToolsCont.h"
#include "UICont.h"
#include "OmEngine/Ui/UIXRay.h"
#include "OmEngine/Utils/OmValue.h"
#include "MainCont.h"
#include "OmEngine/Items/FieldItem.h"
#include "OmEngine/Inputs/Inputs.h"
#include "OmEngine/Components/ProcessComp.h"
#include "OmEngine/Ui/Widget/HudWidgetComp.h"
#include "XRayRender.h"
#include "OmEngine/Utils/CoreServer.h"
#include "PlayerCont.h"


UTaskCont* UTaskCont::INS = NULL;
FString UTask::IDPREFIX = "task";




/*___________________ TASK CONTROLLER ________________ 
#Process key patterns :
{ int }		-> regular key. value[0] == key[0]
{ int,int }	-> array keys.  value[0] == key[0] || value[1] == key[1] || value[n] == key[n] || ....
{ int:int }	-> clamps keys. value[0] >= key[0] && value[0] <= key[1]
*/
 UTaskCont::UTaskCont()
{
	UTaskCont::INS = this;
}
 void UTaskCont::Init()
 {
	 observer = NewObject<UObserver>();
	 observer->Init();
	 InitTasks();

	 //setUi();

	 valitadeAllTasks();


	 BpEventStart();

	 for (UTask* t : listTasks)
	 {
		 for (UProcess* p : t->GetProcesses())
		 {

			 Debug(Id + " - printprocess " +p->ToString(),1);
			 for (URule* r : p->listRules)
			 {
				 Debug(Id + "	 - printprocess rule " + r->ToString(),1);
			 }

		 }
	 }
 }

 void UTaskCont::valitadeAllTasks()
 {
	/* for (UTask* task : listTasks)
	 {
		 for (UProcess* process : task->GetProcesses())
		 {
			 bool isDetectAnyComparision = false;
			 for (URule* rule : process->listRules)
			 {
				 if (rule->comparison.IsValidIndex(0)) isDetectAnyComparision = true;
			 }

			
			 if (!isDetectAnyComparision)
			 {
				 URule* lastRule = process->listRules.Last();
				 lastRule->commands.Add("func:(ProcessCompleted)");
			 }

			
			 
		 }
	 }*/

	

 }
 void UTaskCont::InitTasks()
 {
	 BpEventCreateTasks();
 }




 //void UTaskCont::GenScrewApply(FHoleInfo _hole, FOmValue _makeHoleModel)
 //{

	// UTask* t = 0;
	// UProcess* p = 0;
	// UTask* tStart = 0;
	// UPModel* mHoleAngle = 0;


	// {

	//	 /*--------->(0) : "ENTRY POINT" */
	//	 t = CreateTask("EntryPoint" + _hole.id, "Prepare the entry point with burr " + _hole.label);
	//	 CreateRayProcess(t, _hole.toolEntry->IdCat, _hole.volume->Id);
	//	 CreateFieldProcess(t,  _hole.toolEntry->Id, _hole.processEntry->Id, "", "(TimeTurnOn>=1)");
	//	 CreateRule(t, 0, "", "func:(SetDecal),idDecal:(HoleEntry),field:(" + _hole.hole->Id + "),isEnable:(1)");
	// }
	// {

	//	 //--------->(1) : "MAKE HOLE" 
	//	 //CreateCollisionProcess(t, _hole.toolMakeHole->IdGroup, _hole.volume->Id);
	//	 //_makeHoleModel.Add( FOmValue("BtnTrigger:(1),ProgressStatus:(0/99)"), true);
	//	 //mHoleAngle = AddModel(_makeHoleModel);
	//	 t = CreateTask("Pedicle" + _hole.id, "Prepare the pedicle pathway with pedicle finder " + _hole.label);
	//	 p = CreateProcess(t);
	//	 CreateRule(t, p, "", "func:(resetItem),item:(" + _hole.hole->Id + ")");
	//	 CreateRule(t, p, "", "func:(PutToToolset),tool:(" + _hole.toolProbe->Id + ")");
	//	 CreateRayProcess(t, _hole.toolMakeHole->IdCat, _hole.volume->Id);
	//	 p = CreateFieldProcess(t, _hole.toolMakeHole->Id, _hole.pawnMakeHole->Id, "RotMin:(-10,0,0),RotMax:(5,0,0)", "(VrTrigger==1)&&(ProgressStatus==1)");
	//	 CreateRule(t, p, "", "func:(SetDecal),idDecal:(HoleCompleted),field:(" + _hole.hole->Id + "),isEnable:(1)");
	//	 tStart = t;
	// }

	// //if (AMainCont::INS->DebugMode < 3)
	// //{
	//	// //--------->(2) : "CONFIRM HOLE"
	//	// UPModel* m2_0 = HelperCreateCollisionModel(_hole.toolProbe->IdGroup, _hole.volume->Id);
	//	// UPModel* m2_1 = AddModel(FOmValue("Progress:(1)"));

	//	// t = UTask::Create("Probe" + _hole.id, "Confirm the walls of the pedicle have not been violated with probe");
	//	// AddProcess(t, HelperCreateCollisionProcess(m2_0));
	//	// HelperCreatePawnProcess(t, m2_1, "Progress=1", "", "", "item:(" + _hole.toolProbe->Id + "),comp:(" + _hole.pawnProbe->Id + "),IsReActive:(0))", true);
	//	// AddProcess(t, { "func:(Run),cmd:(SetDisable),item:(" + _hole.toolProbe->Id + ")" });



	//	// AddTask(t);
	// //}

	// //{

	//	// URule* ruleProgressRot = CreateRule(mHoleAngle, "ProgressRot=1", FOmValues(), UConfig::GET_MSG("err_hole_angle"), false, false);
	//	// URule* ruleProgressLine = CreateRule(mHoleAngle, "ProgressLen=1", FOmValues(), UConfig::GET_MSG("err_hole_len"), false, false);
	//	// t = GenBrowseXRay("CarmProbe" + _hole.id, tStart, ruleProgressRot, ruleProgressLine, _hole.pawnProbe->Id, "Check probe position with the C-Arm");
	//	// AddProcess(t, { "func:(PutToToolset),tool:(" + _hole.toolProbe->Id + ")", "func:(Run),cmd:(SetEnable),item:(" + _hole.toolProbe->Id + ")" });

	// //}





	// //{
	//	// //--------->(4) : "INSERT SCREW"  
	//	// t = UTask::Create("InsertScrew" + _hole.id, "Insert the appropriate screw into " + _hole.label);

	//	// HelperMakeInsertEquipment(t, _hole.toolScrew, _hole.screw, _hole.volume, _hole.pawnScrew, "Progress=1", "ForceToFit:(1)");
	//	// AddProcess(t, "func:(SetDecal),idDecal:(HoleCompleted),field:(" + _hole.hole->Id + "),isEnable:(0)");
	//	// AddTask(t);
	// //}

	// //{
	//	// t = GenBrowseXRay("CarmScrew"+_hole.id, 0, 0, 0, _hole.pawnScrew->Id, "Check screw position with the C-Arm");
	// //}

 //}





UTask* UTaskCont::CreateXrayTask(FString TaskId, FString TaskLabel, UTask* GoBackTaskWhenHasError , FString IdTool, FString IdMeshList )
{
	UTask* t = CreateTask(TaskId, TaskLabel);

	//FString metricId = "";
	//FString metricLabel = "";
	//{
	//	 metricId = "CarmFor"+ _prevTaskId;
	//	 metricLabel = "Look at c-arm for "+ _prevTaskId;
	//
	//}

	{ 
		UProcess* p = CreateProcess(t);
		CreateRule(t, p, "", "func:(OpenPopup),msg:(" + TaskLabel + "),time:(0)");



		URule* ruleEnableCarm =  CreateRule(t, p, "KeyProcessSubmit==1", "func:(EnableCarm),IdTool:(" + IdTool + "),IdMeshList:(" + IdMeshList + ")");
		ruleEnableCarm->commands.Add("func:(ClosePopup)");

		

		//p->Metric = FMetric::CreateStartProcess(metricId, metricLabel, true, false);
	}
	

	if(AMainCont::INS->DebugMode < 2 || AMainCont::INS->IsVRMode)
	{ /* lookat */
		//UProcess* p = CreateProcess(t);
		//URule* ruleEnableCarm = CreateRule(t, p, "(RayFrom==Cam)&&(RayTo==LookAtXRay)", "");
		 //CreateRule(t, p, "(BtnSubmit==1)", "func:(ClosePopup)");

	}




		UProcess* p = CreateProcess(t);
		
		CreateRule(t, p, "KeyProcessSubmit==1", "func:(CarmResult)");
		if (GoBackTaskWhenHasError)
		{
			CreateRule(t, p, "(XRayResultFail==1)", "func:(GoBackTask),task:(" + GoBackTaskWhenHasError->Id + ")");
			CreateRule(t, p, "(XRayResultSucc==1)", "func:(ProcessCompleted),process:(" + p->Id + ")");
		}
		else
		{
			CreateRule(t, p, "", "func:(ProcessCompleted),process:("+p->Id+")");
		}

		


	//AddTask(t);
	return t;
}


/* ---------- PROCESS TEMPLATES -------------- */
UProcess* UTaskCont::CreateRayProcess(UTask* Task, FString RayFrom, FString RayTo)
{
	if (!Task) { dispatchErr("TaskNull"); return 0; }
	UProcess* p = CreateProcess(Task);
	CreateRule(Task,p, "(RayFrom==" + RayFrom + +")&&(RayTo=="+ RayTo +")"); //succ
	return p;
}


UProcess* UTaskCont::CreateFieldProcess(UTask* Task, FString IdTool, FString IdProcessComp, FString StartParams,  FString SuccComparison, bool IsTakeTool, int MetricAction)
{
	if (!Task) { dispatchErr("TaskNull"); return 0; }
	AToolItem* tool = Cast<AToolItem>(UResource::INS->GetItem(IdTool, true));
	if (!tool) { dispatchErr("Tool Nod Found : " + IdTool); return 0; }
	FString commandStart = "func:(StartFieldProcess),item:("+ tool->IdCat +"),comp:("+ IdProcessComp +"),"  + StartParams;
	

	if (IdTool.IsValidIndex(0) && IsTakeTool)
	{ //GoToTask surecinde, MoveCont icinde CurrEq null olmamasi icin

		UProcess* processTakeCommands = CreateProcess(Task);
		URule* takeToolsCommands = CreateRule(Task, processTakeCommands, "", "func:(TakeTool),tool:(" + IdTool + ")",1,true);
	}



	UProcess* p = CreateProcess(Task);
	URule* ruleStartProcess = CreateRule(Task, p, "", commandStart);
	URule* ruleSucc = CreateRule(Task, p, SuccComparison);
	
	ruleStartProcess->commands.Add("rule:(" + ruleSucc->Id + ")",0);

	p->MetricStart = FMetricJson::CreateStartProcess(IdProcessComp, Task->Id);
	p->MetricEnd = FMetricJson::CreateEndProcess(IdProcessComp, Task->Id);

	FString commandEnd = "func:(StopFieldProcess),item:(" + tool->IdCat + "),comp:(" + IdProcessComp + ")";
	p->SucceedCommand = FOmValues(commandEnd);


	return p;

}


/* this function included RayProcess */
void  UTaskCont::CreateInsertToolPartProcess(UTask* Task,  FString IdTool, FString IdToolPart, FString IdVolumeComp, FString IdProcessComp, FString StartParams, FString SuccComparison, int MetricAction)
{
	if (!Task) { dispatchErr("TaskNull"); return ; }

	AToolItem* tool = Cast<AToolItem>(UResource::INS->GetItem(IdTool, true));
	AToolItem* toolPart = Cast<AToolItem>(UResource::INS->GetItem(IdToolPart, true));
	if (!tool || !toolPart)  return ; 


	{//rule command : take tool & toolpart
		
		if (IdToolPart.IsValidIndex(0)) {  //GoToTask surecinde, MoveCont icinde CurrEq null olmamasi icin
			UProcess* processTakeCommands = CreateProcess(Task);
			URule* takeToolsCommands = CreateRule(Task, processTakeCommands, "", "func:(TakeTool),tool:(" + IdTool + ")",1,true);
			takeToolsCommands->commands.Add( "func:(TakeTool),tool:(" + IdToolPart + ")");
		};
	}
	CreateRayProcess(Task, toolPart->IdCat, IdVolumeComp);
	//CreateFieldProcess(t, IdTool, IdProcessComp, StartParams + "ForceToFit:(1)", SuccComparison, MetricAction);


	{//rule  : field process
		UProcess* processField = CreateFieldProcess(Task, IdTool, IdProcessComp, StartParams + "ForceToFit:(1),IsReActiveTool:(1)", SuccComparison, false, MetricAction);
		//UProcess* processField = CreateFieldProcess(t, IdImplant, IdProcessComp, StartParams + "ForceToFit:(1)", SuccComparison, MetricAction);
		UProcess* pConnect = CreateProcess(Task);
		//URule* cmdRuleConnect = CreateRule(Task, pConnect, "", "func:(ConnectItem),child:(" + IdToolPart + "),parent:(" + IdProcessComp + "),ConnectPoint:(ConnectPointToHole),IsSnapTarget:(1),disableEq:(1)");
		URule* cmdRuleConnect = CreateRule(Task, pConnect, "", "func:(ConnectItem),child:("+IdTool+"/@CurrPart),parent:(" + IdProcessComp + "),ConnectPoint:(ConnectPointToHole),IsSnapTarget:(1),disableEq:(1)");
		cmdRuleConnect->commands.Add("func:(SetDisable),funcTarget:(" + IdProcessComp + "/@Insert"+ toolPart->IdCat+")");



	}
	




}


UTask* UTaskCont::CreateTakeFieldTask(FString TaskId, FString TaskLabel, FString IdTool, FString IdField, FString IdCatPart, FString IdVolumeComp, FString IdProcessComp, FString StartParams, FString SuccComparison, int MetricAction)
{
	AToolItem* tool = Cast<AToolItem>(UResource::INS->GetItem(IdTool, true));
	//AToolItem* processComp = Cast<AToolItem>(UResource::INS->GetItem(IdProcessComp, true));
	if (!tool )  return 0;

	UTask* t = CreateTask(TaskId, TaskLabel);

	{//0->Ray
		CreateRayProcess(t, tool->IdCat, IdVolumeComp);
	}


	{//1->Process Field
		UProcess* processField = CreateFieldProcess(t, IdTool, IdProcessComp, StartParams , SuccComparison, true, MetricAction);
	}

	{//2->Take

		UProcess* p = CreateProcess(t);
		CreateRule(t, p, "", "func:(TakePart),tool:(" + IdTool + "),field:(" + IdField + ")");
		CreateRule(t, p, "(PutToolCat==" + IdCatPart + ")", "",0);


	}

	{//3->put (offline)
		//UProcess* p = CreateProcess(t);
		//CreateRule(t, p, "", "func:(PutPart),tool:(" + IdTool + "),field:(TrayField)", 1,true);
	}



	return t;
}






void UTaskCont::SetTask(FString _id)
{
	Debug("#task.SetTask : "+ _id);
	UTask* t = GetTask(_id);

	if (t)
	{
		currTask = t;
		onTaskStatus(EStatus::ON_PRE_START);

	}

}


void UTaskCont::GoToTask(int8 _task)
{
	IsAIMode = true;

	UTask* tTarget = GetTask(_task);
			
	int8 pastTaskStartIndex = 0;// AMainCont::INS->IsOperationStarted ? _task : 0;
	int8 pastTaskEndIndex = _task;// AMainCont::INS->IsOperationStarted ? _task + 1 : _task;
	if (!AMainCont::INS->IsOperationStarted)
	{

		for (int8 i = pastTaskStartIndex; i < pastTaskEndIndex; i++)
		{
			UTask* t = GetTask(i);
			if (t)
			{
				t->isAutoNext = false;
				SetTask(t->Id);
				for (int8 i2 = 0; i2 < t->CountTotalProcess; i2++)
				{
					UProcess* p = t->GetProcess(i2);
					
					p->CompleteManual();

					//UObserver::INS->Send(r->GetKeyAsParam());


					/*FProcessResult r;
					r.Result = true;
					p->EventStatus.Broadcast(EEventType::COMPLETE, r);
					Debug("task onEventProcess gototask "+ p->ToString());*/
				}
			}

		}
	}

	IsAIMode = false;

	if (tTarget)
	{
		SetTask(tTarget->Id);
	}
}

void UTaskCont::GoBackTask(FString _idTask)
{
	for (UProcess* p : currTask->GetProcesses())
		p->CompleteManual();

	currTask->IsCompleted = true;
	UTask* goBackTask = GetTask(_idTask);
	for (UTask* t : listTasks)
	{
		if (t->Index >= goBackTask->Index)
		{
			if (t->IsCompleted)
			{
				
				t->Reset();
			}
		}

	}

	SetTask(_idTask);
}

void UTaskCont::onTaskStatus(EStatus _status)
{
	
	if (_status == EStatus::ON_PRE_START)
	{
		if (currTask->CountTotalProcess > 0)
		{
			onTaskStatus(EStatus::ON_STARTED);
		}
		else { //bu task'a ait hic process yok ise
			onTaskStatus(EStatus::ON_COMPLETED);
		}
	}

	else if (_status == EStatus::ON_STARTED)
	{
		
		FString currTaskName = currTask->Label;
		FString nextTaskName = "";
		Debug("#task(" + currTask->Id +") Started!", 1);
		
		if (currTask->Index == listTasks.Num() - 1) //all task completed!
			nextTaskName = "operation completed";
		else 
			nextTaskName = GetTask(currTask->Index + 1)->Label;
		
		if (currTask->IsShowInTaskMenu)
		{
			AUICont::INS->UpdateTaskMonitor(currTaskName, nextTaskName);

		}
		SetProces(0);
	}

	
	else if (_status == EStatus::ON_COMPLETED_SUCC)
	{
		onTaskStatus(EStatus::ON_COMPLETED);
	}


	else if (_status == EStatus::ON_COMPLETED)
	{
		currTask->IsCompleted = true;
		
		//UReport::SendEvent(EReportEvent::END_TASK, currTask->Id + currTask->Name);
		if (currTask->Index == listTasks.Num() - 1) //all task completed!
		{
			for (auto It = listProcesses.CreateConstIterator(); It; ++It)
			{
				UProcess* p = Cast<UProcess>(It->Value);
				if (p->EventStatus.IsBound())
					p->EventStatus.RemoveDynamic(this, &UTaskCont::onEventProcess);
			}
			AMainCont::INS->CompletedOperation();
			UServer::INS->SendMetric(FMetricJson::CreateEndOperation());
			currTask = nullptr;
			currProcess = nullptr;
		}
		else {
			if (currTask->isAutoNext)
				SetTask(GetTask(currTask->Index + 1)->Id);
			
		}
	}
	//AMainCont::INS->OnTask(currTask->Index, _status);

	
}


void UTaskCont::SetProces(int _processIndex)
{
	if (!currTask) return;

	if (currProcess)
	{
		currProcess->IsCompleted = true;
	}
	if (_processIndex < currTask->CountTotalProcess)
	{
		currProcess = currTask->GetProcess(_processIndex);
		onProcessStatus(EStatus::ON_PRE_START);
		onProcessStatus(EStatus::ON_STARTED);


	}
}

void UTaskCont::onProcessStatus(EStatus _status)
{
	if (!currTask || !currProcess) return;
	if (currTask->IsCompleted) return;

	FString strProcess = currTask->Id + "("+ currTask->Id + ") /process(" + FString::FromInt(currProcess->Index) +")";
	if (_status == EStatus::ON_PRE_START)
	{
		currProcess->Reset();
	}

	else if (_status == EStatus::ON_STARTED)
	{
		Debug("#task.process(" + currProcess->Id + ").started : " + currProcess->ToString() ,1);
		currProcess->OnStarted();
		//UObserver::INS->CheckCurrProcess();

		//FProcessResult processResult;
		//processResult.Process = currProcess;
		//EventStatus.Broadcast(EEventType::START, processResult);
		
	}

	else if (_status == EStatus::ON_PROGRESS)
	{

	}



	else if (_status == EStatus::ON_COMPLETED)
	{
		for (UProcess* p : currTask->GetProcesses())
			Debug("#task.process(" + p->Id + "("+FString::FromInt(p->Index) + "/" + FString::FromInt(currTask->CountTotalProcess) +") ).completed! -" + p->SucceedCommand.ToString(),1);

		currProcess->OnCompleted();
		if (currProcess->SucceedCommand.IsValid) RunCommands(currProcess->SucceedCommand);
		if (currProcess->Index == currTask->CountTotalProcess - 1)
		{
			onTaskStatus(EStatus::ON_COMPLETED_SUCC);
		}
		else {
			SetProces(currProcess->Index + 1);
		}
		//eventDisp->dispatchEvent(EEventType::COMPLETE, this, currProcess);
	}



	
}

/******* ON EVENTS **********/
void UTaskCont::onEventProcess(EEventType _eventType, FProcessResult _result )
{
	if (!currProcess ) return;
	if (currProcess->Id  != _result.Process->Id) return;

	if (_eventType == EEventType::COMPLETE)
	{


		if (_result.Result && currProcess)
		{
			if (!currProcess->IsCompleted)
			{
				RunCommands(FOmValues("func:(ProcessCompleted),process:(" + currProcess->Id + ")"));

			}
				

		}


		if (_result.SuccRule)
		{


			if (!_result.SuccRule->isOffline || (_result.SuccRule->isOffline && IsAIMode))
			{

				if (_result.SuccRule->comparisionResult == EComparisionResult::COMPARISION_TRUE)
				{
					_result.SuccRule->SetIsCmdExecuted(true);
					
					//Debug("#task onEventProcess : @_result:SuccRule->commands : " + _result.SuccRule->commands.ToString(),  1);
					RunCommands(_result.SuccRule->commands);

				}

			}

		}


		//if(currProcess) Debug("#task-------------->end : onEventProcess  : gelen process : " + _result.Process->ToString() + "--------  currProcess: " + currProcess->Id, 1);
	}
}



URule* UTaskCont::getProcessResult(UProcess*  _process)
{
	
	FProcessResult pr = observer->CheckProcess(FString::FromInt(_process->Index));
		URule* result = pr.SuccRule;
	
	//if (rawResult.status == 0 || rawResult.status == 2)
	//if (rawResult.status == EComparisionResult::COMPARISION_TRUE)
	//{
		/*newResult.commands = FOmValues("func:(ProcessCompleted)");*/

	//}
	//else if (rawResult.status == 1) //rule eslesmesi var
	//{
		//newResult.commands = rawResult.rule->commands;
	//}
	return result;
	
}




void UTaskCont::RunCommands(FOmValues _commands)
{
	if (!_commands.IsValid) return;
	
	for (FOmValue ov : _commands.list)
	{
		if (ov.IsValid)
			runCommand(ov);

	}

}

void UTaskCont::runCommand(FOmValue ov)
{
	FString func;
	AItem* item = 0;
	AToolItem* toolItem = 0;
	UOmComp* comp = 0;
	UProcessComp* compProcess = 0;

	if(currProcess)	Debug("		--->>RunCommand(" + ov.ToString() + ") -> "  + currProcess->ToString());


	func = ov.GetValue("func").GetString(0).ToLower();
	FString move = ov.GetValue("move").IsValid ? ov.GetValue("move").GetString() : "MoveR";
	//if (AMainCont::INS->IsHapticMode == 0) inputSource = EInputSource::MOUSE;
	FString idItem = ov.GetValue("item").GetString(0);
	if(idItem.IsValidIndex(0)) item = Cast<AItem>(UResource::INS->GetItem(idItem));
	//toolItem = Cast<AToolItem>(UResource::INS->GetItem(ov.GetValue("tool").GetString(0)));
	if ( item) toolItem = Cast<AToolItem>(item);
	FString idComp = ov.GetValue("comp").GetString(0);
	if (idComp.IsValidIndex(0))  comp = Cast<UOmComp>(UResource::INS->GetOmComp(idComp));
	compProcess = Cast<UProcessComp>(UResource::INS->GetOmComp(ov.GetValue("comp").GetString(0)));

	
	FString funcTarget = ov.GetValue("funcTarget").GetString(0).ToLower();
	if(funcTarget.IsValidIndex(0))
	{
		AItem* item =  Cast<AItem>(UResource::INS->GetItem(funcTarget));
		if (item)
			item->Run(ov);
		else
		{
			UOmComp* comp = Cast<UOmComp>(UResource::INS->GetComp(funcTarget));
			if (comp)
				comp->Run(ov);
		}
	}

	if (func == "ProcessCompleted")
	{
		if (currProcess)
		{
			FString processId = ov.GetValue("process").GetString(0).ToLower();
			if(processId == currProcess->Id)
				onProcessStatus(EStatus::ON_COMPLETED);
		}
		
	}
			
		

	if (func == "StartFieldProcess" && compProcess)
	{
		APlayerCont::INS->SetProcess(true, move, toolItem, compProcess, ov);
	}
	
	if (func == "StopFieldProcess" && compProcess)
	{
		APlayerCont::INS->SetProcess(false, move, toolItem, compProcess, ov);
	}
	




	if (func == "TakeTool")
	{

		FString _idTool = ov.GetValue("tool").GetString();
		APlayerCont::INS->TakeTool("MoveR", _idTool);


	}



	if (func == "resetTrans")
	{
		



			if (item)
			{
				item->RestoreRelTrans("InitRel");
			}
			else if (comp)
			{
				comp->RestoreTrans("Init");
			}
		

	}


	

	 if (func == "ConnectItem")
	{
		
			FString parentId = ov.GetValue("parent").GetString(0);
			FString childId = ov.GetValue("child").GetString(0);
			if (parentId.IsValidIndex(0) && childId.IsValidIndex(0))
			{
				toolItem = Cast<AToolItem>(UResource::INS->GetItem(childId));
				//if (AMainCont::INS->IsAppStarted)
				{
					//toolItem = APlayerCont::INS->PutTool("MoveR", true);
				}


				if (toolItem) childId = toolItem->Id;
				
				UToolsCont::INS->ConnectItems(childId, parentId, ov);
				
			}

		
	}

	 if (func == "UnconnectItem")
	{
		
			FString parentId = ov.GetValue("parent").GetString(0);
			FString childId = ov.GetValue("child").GetString(0);
			if (parentId.IsValidIndex(0) && childId.IsValidIndex(0))
			{
				UToolsCont::INS->UnConnectItems(childId, ov);
			}

		
	}

	if (func == "PutToToolset" )/* tool transform reset to default inited transform on toolset */
	{
		
			toolItem = Cast<AToolItem>(UResource::INS->GetItem(ov.GetValue("tool").GetString(0)));
			//if (toolItem)
			//{
				UToolsCont::INS->PutToToolset(toolItem);
			//}
	

		

	}

	if (func == "ResetCurrentTask")
	{
		
		
			GoToTask(currTask->Index);
		

	}


	

	if (func == "TakePart")
	{
		
			toolItem = Cast<AToolItem>(UResource::INS->GetItem(ov.GetValue("tool").GetString(0)));
			AFieldItem* field = Cast<AFieldItem>(UResource::INS->GetItem(ov.GetValue("field").GetString(0)));
			if (toolItem && field)
			{
				toolItem->TakePartInField(field);
			}
			else {
				Debug::Warning("TakeCompInField Error! Tool ve/veya Field bulunamadi");
			}


		
	}
	if (func == "PutPart")
	{
		
			toolItem = Cast<AToolItem>(UResource::INS->GetItem(ov.GetValue("tool").GetString(0)));
			AFieldItem* field = Cast<AFieldItem>(UResource::INS->GetItem(ov.GetValue("field").GetString(0)));
			if (toolItem && field)
			{
				toolItem->PutPartInField(field);
			}
			else {
				Debug::Warning("PutPartInField Error! Tool ve/veya Field bulunamadi");
			}


		
	}

	




	else if (func == "EnableCarm")
	{
		
			AXRayRender::INS->RefreshCaptures();
			//if(UResource::INS->ROOM) UResource::INS->ROOM->Run(FOmValue("cmd:(EnableXRayDevice)"));
			AItem* deviceCArm = UResource::INS->GetItem("DeviceCArm", false);
			if(deviceCArm) deviceCArm->Run(FOmValue("func:(EnableXRayDevice)"));
			FString IdTool = ov.GetValue("IdTool").GetString(0);
			FString IdMeshList = ov.GetValue("IdMeshList").GetString(0);
			if(AUICont::INS->UiXray)AUICont::INS->UiXray->ShowResult(IdTool, IdMeshList);

			{//#metric carm view
				
				FMetricJson m = FMetricJson::Create();
				m.Json->SetStringField("category", "carm");
				m.Json->SetStringField("action", "view");
				m.Json->SetStringField("value", currTask->Id);

				UServer::INS->SendMetric(m);
			}
		
		
	}
	else if (func == "DisableCarm")
	{
		

			//if (_isStart && UResource::INS->ROOM) UResource::INS->ROOM->Run(FOmValue("cmd:(DisableXRayDevice)"));
			AItem* deviceCArm = UResource::INS->GetItem("DeviceCArm", false);
			if (deviceCArm) deviceCArm->Run(FOmValue("func:(DisableXRayDevice)"));
		
		
	}

	else if (func == "CarmResult")
	{
		bool carmResult = AUICont::INS->UiXray->ExecuteResult();

		{//#metric carm check

			FMetricJson m = FMetricJson::Create();
			m.Json->SetStringField("category", "carm");
			m.Json->SetStringField("action", "check");
			m.Json->SetStringField("value", currTask->Id);
			m.AddDataParam("carmResult", FString::FromInt((int)carmResult));

			UServer::INS->SendMetric(m);
		}

	}
	else if (func == "XrayRefresh")
	{
		 	AXRayRender::INS->RefreshCaptures();

	}



	else if (func == "OpenPopup")
	{
		FString msg = ov.GetValue("msg").GetString();
		float time = (ov.GetValue("time").IsValid) ? ov.GetValue("time").GetFloat(0) : 0; //popup auto close time
		AUICont::INS->ShowMessage(msg, 1, time);
	}
	else if (func == "ClosePopup")
	{
			AUICont::INS->HideMessage();
	}


	else if (func == "resetItem")
	{
		
		
			if (item)
			{
				item->Reset();
			}

		
	}

	else if (func == "goBackTask")
	{
		
			FString idTask = ov.GetValue("task").GetString(0);
			GoBackTask(idTask);

		
	}

	



	


	
	
}






void UTaskCont::AddTask(UTask* _task, bool isShowTaskMenu, int8 _insertIndex) //oncesinde en son nvagriliyordu. simdi processler eklenmeden cagriliyor.
{
	if (!_task) { dispatchErr("TaskNull"); return; }
	int _index = listTasks.Num();
	//FString _id = UTask::IDPREFIX + FString::FromInt(_index);
	_task->Index = _index;
	//_task->Id = _id;
	_task->IsShowInTaskMenu = isShowTaskMenu;
	if (_insertIndex > -1)
		listTasks.Insert(_task, _insertIndex);
	else
		listTasks.Add(_task);


	Debug("#taskadd : " + _task->Id  + " listTasks " , listTasks.Num(),1);

	{//update all task index
		int8 i = 0;
		for (UTask* t : listTasks)
		{
			t->Index = i;
			i++;
		}
	}
}



void UTaskCont::AddTaskToNext(UTask* _task, FString _targetPRevTaskId, bool isShowTaskMenu)
{
	UTask* prevTask = GetTask(_targetPRevTaskId);
	if (prevTask)
	{
		int8 prevTaskIndex = prevTask->Index + 1;
		AddTask(_task, isShowTaskMenu, prevTaskIndex);
		
	}

	
	
}


UProcess* UTaskCont::AddProcess(UTask* _task, UProcess* _process, int8 _index)
{
	if (!_task) { Debug::Warning("Task not found!"); return 0; }
	if (!_process) { Debug::Warning("Process not found!"); return 0; }
		
	_task->AddTaskProcess(_process, _index);
	if (!_process->EventStatus.IsBound())
		_process->EventStatus.AddDynamic(this, &UTaskCont::onEventProcess);
	return _process;
}








UTask* UTaskCont::GetTask(FString _id)
{
	UTask* task = 0;
	for (UTask* t : listTasks)
	{
		if (t->Id == _id)
			task = t;
	}
	return task;
}
UTask* UTaskCont::GetTask(int8 _index)
{
	UTask* task = 0;
	if(listTasks.IsValidIndex(_index))
		task = listTasks[_index];
	
	return task;
}




FString UTaskCont::GetTaskLabel(FString _id)
{
	FString s = "";
	UTask* t = GetTask(_id);
	if (t)
		s = t->Label;
	return s;
}
URule* UTaskCont::GetRule(FString _id)
{
	URule* r = listRules[_id];
	return r;

}
//UPModel* UTaskCont::GetModel(FString _id)
//{
//	UPModel* m = listModels[_id];
//	return m;
//}


void UTaskCont::GenerateOperationId()
{
	FString id =  FGuid::NewGuid().ToString();
	IdOperation = id;
}



void UTaskCont::PrintMsg(bool _type, int8 _status, FString _msg)
{
	if (!AMainCont::INS->IsAppStarted) return; //gototask asamasindayken pritn etmemeliyiz
	if (_type == 0)
	{
		AUICont::INS->ShowMessage(_msg, _status, 1);
	}
	
	else if (_type == 1)
	{
		AUICont::INS->ShowMessage(_msg, _status, 0.5);
	}
	
}

void UTaskCont::dispatchErr(FString _id)
{
	if (_id == "TaskNull")
		Debug::Warning("ERROR -> Task == NULL");
	if (_id == "ProcessNull")
		Debug::Warning("ERROR -> Process == NULL");

}





//************************************
// 
//
// -> BLUEPRINTS
//
//
//************************************
UTask* UTaskCont::CreateTask(FString TaskId, FString TaskLabel , bool AutoNextTask)
{

	UTask* t = UTask::Create(TaskId, TaskLabel, AutoNextTask);
	AddTask(t,true);
	return t;
}

UProcess* UTaskCont::CreateProcess(UTask* Task)
{

	UProcess* p = UProcess::Create();
	AddProcess(Task, p);
	return p;
}

URule* UTaskCont::CreateRule(UTask* Task, UProcess* Process, FString Comparison, FString SuccedCommand, int DefaultResult, bool IsOfflineRule)
{
	UProcess* p = Process;
	if (!p) p = CreateProcess(Task);

	URule* r = URule::Create(Comparison, SuccedCommand);
	//r->Result = DefaultResult;
	r->isOffline = IsOfflineRule;
	//r->isPrint = _isPrint;
	//r->printAfterCounter = _printAfterCounter;
	r->Id = URule::IDPREFIX + FString::FromInt((int)listRules.Num());
	p->AddRule(r);
	listRules.Add(r->Id, r);
	return r;
}




//bool UTaskCont::BooleanAND(FString a)
//{
//	return true;
//}

/*_____________ #UTask  ____________ */
UTask* UTask::Create(FString _id, FString _label, bool _autoNext)
{
	UTask* t = NewObject<UTask>();
	t->Id = _id;
	t->Label = _label;
	t->isAutoNext = _autoNext;
	if (_label.Len() == 0) t->IsShowInTaskMenu = false;
	return t;
}




void UTask::AddTaskProcess(UProcess* _process, int8 _index)
{

	if (_index < 0)
	{
		listProcess.Add(_process);
	}
	else
	{
		_index = FMath::Min<int>(_index, listProcess.Num() - 1);
		listProcess.Insert(_process, _index);
	}


	/* update */
	for (int8 i = 0; i < listProcess.Num(); i++)
	{
		

			int16 _indexProcess = i;
			FString _idProcess = UProcess::IDPREFIX + FString::FromInt(_indexProcess);
			listProcess[i]->Id = Id + "/" + _idProcess;
			listProcess[i]->Index = i;

	}
	CountTotalProcess = listProcess.Num();



}

UProcess* UTask::GetProcess(int _index)
{
	if(listProcess.IsValidIndex(_index))
		return listProcess[_index];	
	else
		return 0;
}
TArray<UProcess*> UTask::GetProcesses()
{
	return listProcess;
}

void UTask::Reset()
{
	for (UProcess* p : listProcess)
	{
		if (p->IsCompleted)
		{
			p->Reset();
		}
	}
	IsCompleted = false;
}


class AMainCont* UTaskCont::GetMainCont()
{
	return AMainCont::INS;
}


