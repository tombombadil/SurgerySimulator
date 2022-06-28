#include "SPINETaskCont.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Items/FieldItem.h"



USPINETaskCont* USPINETaskCont::INS = NULL;
//FString UTask::IDPREFIX = "SPINE_task";





USPINETaskCont::USPINETaskCont():UTaskCont()
{
	USPINETaskCont::INS = this;
}


void USPINETaskCont::InitTasks()
{
	UTask* t = 0;
	UProcess* p = 0;
	UTask* tStart = 0;


	//t = UTask::Create("start");
	//p = AddProcess(t, {
	//	//"func:(Run),cmd:(MonitorAnimIn),item:(Lightset),id:(lamp)",
	//	"func:(OpenPopup),msg:(" + UConfig::GET_MSG("WELCOME") + "),time:(0)"
	//	});
	//p->AddRule(CreateRule(AddModel(FOmValue("BtnHapticR1:(1)")), "BtnHapticR1=1", FCommand("func:(ProcessCompleted)")));
	//p->Metric = FMetric::CreateStartOperation();
	//AddProcess(t, "func:(ClosePopup)");
	//AddTask(t);


	//GenScrewApply(USPINEResource::INS->hole1, FOmValue("ProgressLen:(0.4/5),ProgressRot:(-20/-10,10/25,0/0")); //ProgressLen(cm)
	//GenScrewApply(USPINEResource::INS->hole2, FOmValue("ProgressLen:(0.4/5),ProgressRot:(-20/-10,10/25,0/0"));
	//GenRod("Use the rod holder to place the rod", USPINEResource::INS->rodInfo1);
	//GenNutPut("Introduce nut into the pedicle screw " + USPINEResource::INS->hole1.label, USPINEResource::INS->nutFieldInfo1);
	//GenNutPut("Introduce nut into the pedicle screw " + USPINEResource::INS->hole2.label, USPINEResource::INS->nutFieldInfo2);
	//GenNutTighten("Semi-tighten nut  " + USPINEResource::INS->hole1.label, USPINEResource::INS->nutFieldInfo1, "Progress:(1)", "StartRatio:(0),ClickAppend:(0.05)");
	//GenNutTighten("Semi-tighten nut  " + USPINEResource::INS->hole2.label, USPINEResource::INS->nutFieldInfo2, "Progress:(0.4/0.6)", "StartRatio:(0),ClickAppend:(0.05)");

	//t = GenSeperate(true, USPINEResource::INS->rodInfo1, "Progress:(1)", "StartRatio:(0),ClickAppend:(0.1),ToolAnimStart:(-0.3),ToolAnimEnd:(-0.6),MeshAnimStart:(0),MeshAnimEnd:(0.5),IsReActive:(0)", "Apply distraction between pedicle screws placed at L4 and L5");

	//t = GenNutTighten("Tighten nut fully " + USPINEResource::INS->hole2.label, USPINEResource::INS->nutFieldInfo2, "Progress:(1)", "StartRatio:(0.5),ClickAppend:(0.05)");
	//AddProcess(t, "func:(PutToToolset),tool:(" + USPINEResource::INS->DISTRACTOR->Id + ")");
	//GenScrewApply(USPINEResource::INS->hole3, FOmValue("ProgressLen:(0.4/5),ProgressRot:(-20/-10,-20/-10,0/0)"), "left");
	//GenScrewApply(USPINEResource::INS->hole4, FOmValue("ProgressLen:(0.4/5),ProgressRot:(-20/-10,-20/-10,0/0)"), "left");


	//t = GenBreakTask(1, FPartFieldInfo(USPINEResource::INS->TAKES_BONE_PART_2->Id, USPINEResource::INS->RONGEUR), FPartFieldInfo(USPINEResource::INS->TAKE_DISCGRAFTS->Id, USPINEResource::INS->RONGEUR));
	//t = GenBreakTask(0, FPartFieldInfo(USPINEResource::INS->TAKES_BONE_PART_1->Id, USPINEResource::INS->RONGEUR), FPartFieldInfo(USPINEResource::INS->TAKE_DISCGRAFTS->Id, USPINEResource::INS->RONGEUR));
	//AddProcess(t, "func:(CloseToolMenu)");


	//{//---------> dissector root

	//	t = UTask::Create("Use Dura retractor to protect the Dura");
	//	UPModel* m0 = HelperCreateCollisionModel(USPINEResource::INS->DURA->IdGroup, USPINEResource::INS->TAKE_DISCS->CompVol->Id);
	//	UPModel* m1 = AddModel(FOmValue("Progress:(1)"));
	//	AddProcess(t, HelperCreateCollisionProcess(m0));
	//	HelperCreatePawnProcess(t, m1, "Progress=1", "", "", "item:(" + USPINEResource::INS->DURA->Id + "),comp:(" + USPINEResource::INS->NERVOUS_PROCESS_ROOT->Id + "),IsReActive:(0)", true);
	//	AddProcess(t, "func:(Run),cmd:(SetDisable),item:(" + USPINEResource::INS->DURA->Id + ")");
	//	AddTask(t);
	//}

	//{//---------> protect the exit root of L4
	//	t = UTask::Create("Use dissector to protect the exit root of L4");

	//	UPModel* m0 = HelperCreateCollisionModel(USPINEResource::INS->DISSECTOR->IdGroup, USPINEResource::INS->NERVOUS_PROCESS_RIGHT->Id);
	//	UPModel* m1 = AddModel(FOmValue("Progress:(1)"));

	//	AddProcess(t, HelperCreateCollisionProcess(m0));
	//	HelperCreatePawnProcess(t, m1, "Progress=1", "", "", "item:(" + USPINEResource::INS->DISSECTOR->Id + "),comp:(" + USPINEResource::INS->NERVOUS_PROCESS_RIGHT->Id + "),IsReActive:(0),StartRatio:(0),ClickAppend:(1)", true);
	//	AddProcess(t, "func:(Run),cmd:(SetDisable),item:(" + USPINEResource::INS->DISSECTOR->Id + ")");
	//	AddTask(t);
	//}
	//{//---------> cut disc surface
	//	t = UTask::Create("Use scalpel to cut the annulus fibrosus");
	//	UPModel* m0_0 = HelperCreateCollisionModel(USPINEResource::INS->SCALPEL->IdGroup, USPINEResource::INS->TAKE_DISCS->CompVol->Id);
	//	UPModel* m0_1 = AddModel(FOmValue("Progress:(1)"));
	//	AddProcess(t, HelperCreateCollisionProcess(m0_0));
	//	AddProcess(t, "func:(visible),widget:(CamDiscDetail,1)");
	//	HelperCreatePawnProcess(t, m0_1, "Progress=1", "", "", "item:(" + USPINEResource::INS->SCALPEL->Id + "),comp:(" + USPINEResource::INS->TAKEDISCS_CUT->Id + "),IsReset:(0),IsReActive:(1)");
	//	AddProcess(t, "func:(OpenToolMenu),isSelectTool:(1)");
	//	AddTask(t);

	//	//AddProcess(t, "func:(Run),cmd:(MonitorAnimIn),item:(Lightset),id:(b)");
	//	AddProcess(t, "func:(Run),cmd:(EnableCapture),item:(CamDisc)");


	//}

	//{ /* curette */
	//	t = UTask::Create("Use a curette to scrape the disc");
	//	UPModel* m0 = HelperCreateCollisionModel(USPINEResource::INS->CURETTE->IdGroup, USPINEResource::INS->TAKE_DISCS->CompVol->Id);
	//	UPModel* m1 = AddModel(FOmValue("Progress:(1)"));
	//	AddProcess(t, HelperCreateCollisionProcess(m0));
	//	HelperCreatePawnProcess(t, m1, "Progress=1", "", "", "item:(" + USPINEResource::INS->CURETTE->Id + "),comp:(" + USPINEResource::INS->TAKE_DISCS_BREAK->Id + "),IsReset:(0),IsReActive:(1s),StartRatio:(0),ClickAppend:(1)");
	//	AddTask(t);

	//}

	//GenDiscCleaner(4);
	//GenPutDiscGrafts(4);
	//GenInsertCage();
	//GenPlaceCage();

	//{
	//	t = UTask::Create("Remove Dissector & NerveHook");
	//	//AddProcess(t, "func:(Run),cmd:(MonitorAnimOut),item:(Lightset),id:(b)");
	//	AddProcess(t, "func:(Run),cmd:(DisableCapture),item:(CamDisc)");

	//	AddProcess(t, "func:(Run),cmd:(SetDisable),item:(" + USPINEResource::INS->DISSECTOR->Id + ")");
	//	AddProcess(t, { "func:(resetTrans),item:(" + USPINEResource::INS->DISSECTOR->Id + ")" });
	//	AddProcess(t, "func:(Run),cmd:(SetDisable),item:(" + USPINEResource::INS->DURA->Id + ")");
	//	AddProcess(t, { "func:(resetTrans),item:(" + USPINEResource::INS->DURA->Id + ")" });
	//	AddTask(t, false);
	//}

	//GenBrowseXRay(0, 0, 0, "Cage", "Check cage position with the C-Arm");

	//GenRod("Insert the left rod", USPINEResource::INS->rodInfo2);
	//GenNutPut("Introduce nut " + USPINEResource::INS->hole3.label, USPINEResource::INS->nutFieldInfo3);
	//t = GenNutPut("Introduce nut " + USPINEResource::INS->hole4.label, USPINEResource::INS->nutFieldInfo4);


	//AddProcess(t, "func:(setOperationDir),dir:(right)");

	//GenNutTighten("Release nut " + USPINEResource::INS->hole2.label, USPINEResource::INS->nutFieldInfo2, "Progress:(0.4/0.6)", "StartRatio:(1),ClickAppend:(-0.05),IsReActive:(1)"); //sag nut1 yarim gevset
	//t = GenSeperate(false, USPINEResource::INS->rodInfo1, "Progress:(1)", "StartRatio:(0),ClickAppend:(0.05),ToolAnimStart:(-2),ToolAnimEnd:(-1.7),MeshAnimStart:(0.5),MeshAnimEnd:(0.25),IsReActive:(0)", "Apply compression between pedicle screws placed at L4 and L5"); //rod1 compress!
	//t = GenNutTighten("Final - tighten nut " + USPINEResource::INS->hole2.label, USPINEResource::INS->nutFieldInfo2, "Progress:(1)", "StartRatio:(0.6),ClickAppend:(0.05),IsReActive:(1)"); //sag nut2 full!
	//AddProcess(t, "func:(PutToToolset),tool:(" + USPINEResource::INS->DISTRACTOR->Id + ")");
	//AddProcess(t, "func:(setOperationDir),dir:(left)");


	//GenNutTighten("Final - tighten nut " + USPINEResource::INS->hole3.label, USPINEResource::INS->nutFieldInfo3, "Progress:(1)", "StartRatio:(0),ClickAppend:(0.05),IsReActive:(1)"); //sol nut3 full!
	//t = GenSeperate(false, USPINEResource::INS->rodInfo2, "Progress:(1)", "StartRatio:(0),ClickAppend:(0.05),ToolAnimStart:(-1.7),ToolAnimEnd:(-1.4),MeshAnimStart:(0.25),MeshAnimEnd:(0),IsReActive:(0)", "Apply compression between pedicle screws placed at L4 and L5"); //rod2 compress!
	//t = GenNutTighten("Final - tighten nut " + USPINEResource::INS->hole4.label, USPINEResource::INS->nutFieldInfo4, "Progress:(1)", "StartRatio:(0),ClickAppend:(0.05),IsReActive:(1)"); //sol nut4 full!
	//AddProcess(t, "func:(PutToToolset),tool:(" + USPINEResource::INS->DISTRACTOR->Id + ")");









}


//
//
//void USPINETaskCont::GenScrewApply(FHoleInfo _hole, FOmValue _makeHoleModel, FString _operationDirection)
//{
//
//	UTask* t = 0;
//	UProcess* p = 0;
//	UTask* tStart = 0;
//	UPModel* mHoleAngle = 0;
//
//
//	{
//
//		/*--------->(0) : "ENTRY POINT" */
//		UPModel* m0_0 = HelperCreateCollisionModel(_hole.toolEntry->IdGroup, _hole.volume->Id);
//		UPModel* m0_1 = AddModel(FOmValue("TimeTurnOn:(1/2)"));
//		t = UTask::Create("Prepare the entry point with burr " + _hole.label);
//
//		AddProcess(t, "func:(setOperationDir),dir:(" + _operationDirection + ")");
//
//
//
//		p = AddProcess(t, HelperCreateCollisionProcess(m0_0));
//
//		HelperCreatePawnProcess(t, m0_1, "TimeTurnOn=1", "", "", "item:(" + _hole.toolEntry->Id + "),comp:(" + _hole.pawnEntry->Id + "),IsReActive:(1)", true);
//
//		AddProcess(t, "func:(SetDecal),idDecal:(HoleEntry),field:(" + _hole.hole->Id + "),isEnable:(1)");
//		AddTask(t);
//	}
//	{
//
//		//--------->(1) : "MAKE HOLE" 
//		UPModel* m1_0 = HelperCreateCollisionModel(_hole.toolMakeHole->IdGroup, _hole.volume->Id);
//		_makeHoleModel.Add(FOmValue("BtnTrigger:(1),ProgressStatus:(0/99)"), true);
//		mHoleAngle = AddModel(_makeHoleModel);
//		{
//			t = UTask::Create("Prepare the pedicle pathway with pedicle finder " + _hole.label);
//			AddProcess(t, "func:(resetItem),item:(" + _hole.hole->Id + ")");
//			AddProcess(t, "func:(PutToToolset),tool:(" + _hole.toolProbe->Id + ")");
//			AddProcess(t, HelperCreateCollisionProcess(m1_0));
//			HelperCreatePawnProcess(t, mHoleAngle, "BtnTrigger=1&&ProgressStatus=1", "", "", "item:(" + _hole.toolMakeHole->Id + "),comp:(" + _hole.pawnMakeHole->Id + "),IsReActive:(1)", true);
//
//			AddProcess(t, "func:(SetDecal),idDecal:(HoleCompleted),field:(" + _hole.hole->Id + "),isEnable:(1)");
//			AddTask(t);
//		}
//		tStart = t;
//	}
//
//	if (AMainCont::INS->DebugMode < 3)
//	{
//		//--------->(2) : "CONTROL HOLE"
//		UPModel* m2_0 = HelperCreateCollisionModel(_hole.toolProbe->IdGroup, _hole.volume->Id);
//		UPModel* m2_1 = AddModel(FOmValue("Progress:(1)"));
//
//		t = UTask::Create("Confirm the walls of the pedicle have not been violated with probe");
//		AddProcess(t, HelperCreateCollisionProcess(m2_0));
//		HelperCreatePawnProcess(t, m2_1, "Progress=1", "", "", "item:(" + _hole.toolProbe->Id + "),comp:(" + _hole.pawnProbe->Id + "),IsReActive:(0))", true);
//		AddProcess(t, { "func:(Run),cmd:(SetDisable),item:(" + _hole.toolProbe->Id + ")" });
//
//		
//
//		AddTask(t);
//	}
//
//	{
//
//		URule* ruleProgressRot = CreateRule(mHoleAngle, "ProgressRot=1", FCommand(), UConfig::GET_MSG("err_hole_angle"), false, false);
//		URule* ruleProgressLine = CreateRule(mHoleAngle, "ProgressLen=1", FCommand(), UConfig::GET_MSG("err_hole_len"), false, false);
//		t = GenBrowseXRay(tStart, ruleProgressRot, ruleProgressLine, _hole.pawnProbe->Id, "Check probe position with the C-Arm");
//		AddProcess(t, { "func:(PutToToolset),tool:(" + _hole.toolProbe->Id + ")", "func:(Run),cmd:(SetEnable),item:(" + _hole.toolProbe->Id + ")" });
//
//	}
//
//
//
//
//
//	{
//		//--------->(4) : "INSERT SCREW"  
//		t = UTask::Create("Insert the appropriate screw into " + _hole.label);
//
//		HelperMakeInsertEquipment(t, _hole.toolScrew, _hole.screw, _hole.volume, _hole.pawnScrew, "Progress=1", "ForceToFit:(1)");
//		AddProcess(t, "func:(SetDecal),idDecal:(HoleCompleted),field:(" + _hole.hole->Id + "),isEnable:(0)");
//		AddTask(t);
//	}
//
//	{
//		t = GenBrowseXRay(0, 0, 0, _hole.pawnScrew->Id, "Check screw position with the C-Arm");
//	}
//
//}
//
//void USPINETaskCont::GenRod(FString _taskName, FRodInfo _info)
//{
//	UTask* t = UTask::Create(_taskName);
//	HelperMakeInsertEquipment(t, _info.instertTool, _info.rod, _info.vol, _info.processInsert, "BtnHapticR1=1");
//	AddTask(t);
//}
//
//UTask* USPINETaskCont::GenNutPut(FString _taskName, FNutFieldInfo _nutFieldInfo)
//{
//	UTask* t = UTask::Create(_taskName);
//	HelperMakeInsertEquipment(t, _nutFieldInfo.insertTool, _nutFieldInfo.nut, _nutFieldInfo.vol, _nutFieldInfo.insert, "BtnHapticR1=1");
//	AddTask(t);
//	return t;
//}
//
//UTask* USPINETaskCont::GenNutTighten(FString _taskName, FNutFieldInfo _nutFieldInfo, FString _model, FString _addParams)
//{
//	UTask* t = UTask::Create(_taskName);
//	UProcess* p = 0;
//	UPModel* m1 = HelperCreateCollisionModel(_nutFieldInfo.tightenTool->Id, _nutFieldInfo.vol->Id);
//
//	UPModel* m2 = AddModel(FOmValue(_model));
//
//
//
//
//	AddProcess(t, HelperCreateCollisionProcess(m1));
//	FString pawnParams = "item:(" + _nutFieldInfo.tightenTool->Id + "),comp:(" + _nutFieldInfo.tighten->Id + "),IsReActive:(1)" + _addParams;
//	HelperCreatePawnProcess(t, m2, "Progress=1", "", "", pawnParams, true);
//
//	AddTask(t);
//	return t;
//}
//
//UTask* USPINETaskCont::GenSeperate(bool _isSeperate, FRodInfo rodInfo, FString _model, FString _addParams, FString _boardMsg)
//{
//	//FString taskName = _isSeperate ? "Apply distraction between pedicle screws" : "Compress";
//
//	//AToolItem* tool = _isSeperate ? UResource::INS->DISTRACTOR : UResource::INS->DISTRACTOR;
//	UTask* t = UTask::Create(_boardMsg);
//
//	UPModel* m1_0 = HelperCreateCollisionModel(rodInfo.distractTool->Id, rodInfo.vol->Id);
//	UPModel* m1_1 = AddModel(FOmValue(_model));
//
//	AddProcess(t, HelperCreateCollisionProcess(m1_0));
//
//	//FString pawnParams =  "item:(" + tool->Id + "),comp:(" + rodInfo.processSep->Id + "),IsReset:(0),IsReActive:(0),SepType:(" + taskName + ")" + _addParams;
//	FString pawnParams = "item:(" + rodInfo.distractTool->Id + "),comp:(" + rodInfo.processSep->Id + "),IsReset:(0),IsReActive:(0)" + _addParams;
//	HelperCreatePawnProcess(t, m1_1, "Progress=1", "", "", pawnParams, true);
//
//
//
//	AddTask(t);
//	return t;
//}
//
//UTask* USPINETaskCont::GenBreakTask(int8 _index, FPartFieldInfo _takeField, FPartFieldInfo _putField)
//{
//	UProcess* p = 0;
//	UTask* t = UTask::Create("Remove the left inferior articular process by using rongeur");
//
//	FString metridId = "Break" + _takeField.field->Id;
//	FString metridLabel = "Break and take " + _takeField.field->Id;
//	FMetric metricStart = FMetric::CreateStartProcess(metridId, metridLabel, true, false);
//	FMetric metricEnd = FMetric::CreateEndProcess(metridId, metridLabel);
//
//
//	HelperTakePart(true, t,  _takeField, metricStart);
//
//	AddProcess(t, "func:(OpenToolMenu),isSelectTool:(0)");
//	//HelperTakePart(false, t, _takeField.takeTool, FPartFieldInfo( UResource::INS->TAKE_DISCGRAFTS->Id), metricEnd);
//	HelperTakePart(false, t, _putField, metricEnd);
//	AddTask(t);
//	return t;
//
//}
//
//void USPINETaskCont::GenInsertCage()
//{
//	UTask* t = 0;
//	UProcess* p = 0;
//	t = UTask::Create("Insert cage");
//	AddProcess(t, "func:(visible),widget:(CamDiscDetail,1)");
//	HelperMakeInsertEquipment(t, USPINEResource::INS->CAGEHOLDER, USPINEResource::INS->CAGE, USPINEResource::INS->TAKE_DISCS->CompVol, USPINEResource::INS->CAGEFIELD_INSERT);
//
//	AddTask(t);
//
//
//}
//
//void USPINETaskCont::GenPlaceCage()
//{
//
//	UTask* t = 0;
//	UProcess* p = 0;
//	t = UTask::Create("Adjust the final position of the cage");
//
//
//	UPModel* m2 = AddModel(FOmValue("Progress:(1)"));
//
//
//
//	AddProcess(t, {
//		"func:(Run),cmd:(SetEnable),item:(" + USPINEResource::INS->CAGE->Id + ")"
//		//"func:(HoldItem),tool:(" + UResource::INS->HAMMER_SET->Id + "),item:(" + UResource::INS->CAGE->Id + ")"
//		});
//
//	HelperCreatePawnProcess(t, m2, "Progress=1", "", "", "item:(" + USPINEResource::INS->HAMMER_SET->Id + "),comp:(" + USPINEResource::INS->CAGEFIELD_PLACEMENT->Id + "),IsReset:(0),IsReActive:(1),isReset:(0),StartRatio:(0),ClickAppend:(0.1),ClickAppendDelay:(0.3)", true);
//
//	AddProcess(t, {
//		"func:(ReleaseItem),tool:(" + USPINEResource::INS->HAMMER_SET->Id + ")" ,
//		"func:(Run),cmd:(SetEnable),item:(" + USPINEResource::INS->CAGE->Id + ")",
//		"func:(setTool),tool:(none)"
//		});
//
//
//
//
//	AddTask(t);
//
//
//}
//
//void USPINETaskCont::GenDiscCleaner(int8 _discCount)
//{
//	UProcess* p = 0;
//	UTask* t = 0;
//
//	t = UTask::Create("Scoop away the disc by using disc rongeur");
//
//	FString metridId = "Take";
//	FString metridLabel = "Take ";
//	for (int8 i = 0; i < _discCount; i++)
//	{
//		FString namePart = "DiscPart" + FString::FromInt(i);
//		FString metricId = "Take" + namePart;
//		FString metricLabel = "Take " + namePart + " from disc";
//		FMetric metricStart = FMetric::CreateStartProcess(metricId, metricLabel, true, false);
//		FMetric metricEnd = FMetric::CreateEndProcess(metricId, metricLabel);
//
//		HelperTakePart(true, t, FPartFieldInfo(USPINEResource::INS->TAKE_DISCS->Id, USPINEResource::INS->DISCRONGEUR), metricStart);
//		HelperTakePart(false, t, FPartFieldInfo(USPINEResource::INS->TAKE_DISCGRAFTS->Id, USPINEResource::INS->DISCRONGEUR ), metricEnd);
//
//	}
//	AddProcess(t, "func:(visible),widget:(CamDiscDetail,0)");
//	AddTask(t);
//}
//
//void USPINETaskCont::GenPutDiscGrafts(int8 _partCount)
//{
//	UProcess* p = 0;
//	UTask* t = UTask::Create("Place the bone graft to the disc space");
//	AddProcess(t, "func:(visible),widget:(CamDiscDetail,1)");
//
//
//	for (int8 i = 0; i < _partCount; i++)
//	{
//		FString namePart = "Graft" + FString::FromInt(i);
//		FString metricId = "Take" + namePart;
//		FString metricLabel = "Take " + namePart + " and put into the disc";
//		FMetric metricStart = FMetric::CreateStartProcess(metricId, metricLabel, true, false);
//		FMetric metricEnd = FMetric::CreateEndProcess(metricId, metricLabel);
//		HelperTakePart(true, t, FPartFieldInfo(USPINEResource::INS->TAKE_DISCGRAFTS->Id, USPINEResource::INS->DISCRONGEUR), metricStart);
//		HelperTakePart(false, t,  FPartFieldInfo(USPINEResource::INS->TAKE_DISCS->Id, USPINEResource::INS->DISCRONGEUR), metricEnd);
//
//	}
//	AddProcess(t, "func:(visible),widget:(CamDiscDetail,0)");
//	AddProcess(t, "func:(CloseToolMenu)");
//	AddTask(t);
//}
//
