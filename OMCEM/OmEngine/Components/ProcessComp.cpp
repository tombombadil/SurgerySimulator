#include "ProcessComp.h"
#include "OMCEM.h"
#include "OmEngine/Utils/Resource.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Controllers/TaskCont.h"
#include "OmEngine/Controllers/PlayerCont.h"
#include "OmEngine/Controllers/UICont.h"
#include "OmEngine/Controllers/Process.h"
#include "../Ui/Widget/ToolWidgetComp.h"
#include "../Items/FieldItem.h"


 

UProcessComp::UProcessComp()
{
	SetCollisionProfileName(FName("ProcessComps"));
}


void UProcessComp::Init()
{
	UOmComp::Init();


	hole = Cast<AHoleField>(GetOwner());
		
}



void UProcessComp::updateStartingTrans()
{
	//startingTrans = FTransform(MoveCont->GetRot(), MoveCont->GetLoc());
	MoveCont->UpdateProcessInputOrigin();
	isValidStartingTrans = true;
	
}


void UProcessComp::SetProcessEnable(bool _status, AMoveCont* _Movecont, AToolItem* _toolItem, FOmValue _params)
{
	SetEnable(_status);
	currTool = _toolItem;
	MoveCont = _Movecont;
	ForceFeedbackRatio = 0;
	if (GetEnable())
	{
		//RestoreTrans("init");
		SetRelativeLocation(FVector::ZeroVector);
		SetRelativeRotation(FRotator::ZeroRotator);
		 SaveTrans("BeforeProcess");

		 FTransform g = GetSavedTrans("BeforeProcess");

		{//reset
			lastProcessTransRel.SetIdentity();
			lastProcessTrans.SetIdentity();
			//startingTrans.SetIdentity();
			Ratio = 0;
			isValidStartingTrans = false;
		}

		/* get & set init params */
		float paramStartRatio = _params.GetValue("StartRatio").GetFloat(0, false); 
		paramStartRatio = FMath::Clamp<float>(paramStartRatio, 0, 1);
		progressValuePerClick = _params.GetValue("ClickAppend").GetFloat(0, false);
		IsReActiveTool = _params.GetValue("IsReActiveTool").IsValid ? _params.GetValue("IsReActiveTool").GetBool(0) : true;
		
		float paramProgressMin = _params.GetValue("ProgressMin").GetFloat(0);
		float paramProgressMax = _params.GetValue("ProgressMax").GetFloat(0);
		float paramRatioMin = _params.GetValue("RatioMin").GetFloat(0);
		float paramRatioMax = _params.GetValue("RatioMax").GetFloat(0);
		paramStartRatio = FMath::Clamp<float>(paramStartRatio, 0, 1);

		isRunProcess = -1;
		APlayerCont::INS->SetFocus(EFocus::PROCESS_COMPENENT);

		FString uiProgressSliderParams = DisplayProgressValue; //ui

		bool isHoleValid = false;

		if(hole)
		{//GET start & end from holecomp
			FOmValue defaultProgressValues = FOmValue(DisplayProgressValue);
			if (hole->GetDepth() != 0 && !defaultProgressValues.ContainsKey("labels"))
			{
				isHoleValid = true;
				
				//FString steps = FString::SanitizeFloat(defaultProgressValues.GetValue("steps").GetFloat(0));
				FString steps = FString::FromInt(defaultProgressValues.GetValue("steps").GetInt(0)); //new steps 
				uiProgressSliderParams = "labels:(0," + FString::SanitizeFloat(hole->GetDepth()) + "),steps:("+ steps +"),title:(Depth[cm] )"; //ui
				EndPoint.SetLocation(FVector(0, 0, hole->GetDepth()*-1 ));

				
			}
			
				{//ForceToFit : hole depth, toolpart'dan uzun olsa bile, toolpart uzunlugunun referans alinmasi
			
			bool isForceToFit = _params.GetValue("ForceToFit").GetBool();
				if (isForceToFit && currTool->CurrPart)
				{
					FVector startPointNewLoc = StartPoint.GetLocation();
					startPointNewLoc.Z = currTool->CurrPart->GetTipPoint(false).Z * -1;
					StartPoint.SetLocation(startPointNewLoc);
					EndPoint.SetLocation(FVector::ZeroVector);
				}
		}
		}
		
	

		if (IsShowWidgetBar)
		{
			AUICont::INS->WidgetToolProgress->SetStyle(UiBarStyle);

			bool isIntegerValues = isHoleValid ? false : true;
			AUICont::INS->WidgetToolProgress->Slider->SetSliderData(
				uiProgressSliderParams + 
				",valueMin:(" + FString::SanitizeFloat(paramProgressMin) + ")"+
				",valueMax:(" + FString::SanitizeFloat(paramProgressMax) + ")"+
				",ratioMin:(" + FString::SanitizeFloat(paramRatioMin) + ")"+
				",ratioMax:(" + FString::SanitizeFloat(paramRatioMax) + ")",
				1, 
				isIntegerValues
			);
			
			AUICont::INS->WidgetToolProgress->SetTitle(FOmValue(uiProgressSliderParams).GetValue("title").GetString(0));

			AUICont::INS->WidgetToolProgress->SetVisible(true);
		}

		lastProcessTrans = GetPoint(0);

		SetRatio(paramStartRatio);
		SetStatus(EEventType::START);


		stickAngle = 0;


		{//decision progressType

			if (HapticStickAngleScale != 0)
				progressType = EProgressType::HAPTIC_STICK_ANGLE;
			else if (StartPoint.GetLocation().Size() != EndPoint.GetLocation().Size() || StartPoint.GetRotation().Size() != EndPoint.GetRotation().Size())
				progressType = EProgressType::FOLLOW_PATH;
			else
				progressType = EProgressType::FREE;
		}




	}
	else
	{
		SetStatus(EEventType::COMPLETE);

		APlayerCont::INS->BackToPrevFocus();
		AUICont::INS->WidgetToolAngleX->Reset();
		AUICont::INS->WidgetToolAngleY->Reset();
		AUICont::INS->WidgetToolProgress->Reset();
		AUICont::INS->SetActiveWidget(EWidgetId::TOOL_PROGRESS, false, false);

		observerParams = FOmValue("");
		currTool = 0;
	}

}

void UProcessComp::UpdateTransform()
{
	
	if (IsEnable && MoveCont)
	{

		if (progressType == EProgressType::FOLLOW_PATH || progressType == EProgressType::HAPTIC_STICK_ANGLE)
		{
			//if (!MoveCont->Input->IsHaptic() || (MoveCont->Input->IsHaptic() && (MoveCont->Input->BtnHaptic1 || MoveCont->Input->BtnHaptic2)))
			if (MoveCont->Input->GetKeyPreset("KeyProcess"))
			{
				lastProcessTransRel = GetTransRelative();
				lastProcessTrans = GetTransWorld();
			}


		}
		else if (progressType == EProgressType::FREE)
		{
			lastProcessTrans = GetTransWorld();
		}

	

		FVector worldLoc = lastProcessTrans.GetLocation();
		FRotator worldRot = lastProcessTrans.GetRotation().Rotator();


		FTransform startTrans = GetSavedTrans("BeforeProcess");
		FTransform worldTrans = FTransform(startTrans.GetRotation().Rotator() + worldRot, startTrans.GetLocation() + worldLoc);
		FTransform relTrans = lastProcessTransRel;
		SetStatus(EEventType::UPDATE);

		updateToolMenu(FTransform(worldRot, worldLoc));


		this->SetWorldTransform(worldTrans);
		if (currTool) currTool->SetActorRelativeTransform(relTrans);

	}

}
PairTrans UProcessComp::GetProcessTrans()
{
	PairTrans result;
	
	return result;
}

//FTransform UProcessComp::GetAdditionTrans(FTransform _trans1, FTransform _trans2)
//{
//	FTransform _result;
//	_result.SetLocation(_trans1.GetLocation() + _trans2.GetLocation());
//	_result.SetRotation((_trans1.GetRotation().Rotator() + _trans2.GetRotation().Rotator()).Quaternion());
//	return _result;
//}


FVector UProcessComp::GetStartPoint()
{
	return GetPoint(0).GetLocation() + GetComponentLocation();
}



FVector UProcessComp::GetProcessLoc(bool _isDelta)
{
	FVector loc;

	loc = (GetPoint(1).GetLocation() - GetPoint(0).GetLocation()) * Ratio;
	if (!_isDelta)
		loc += GetComponentLocation();
	return loc;
}

FTransform UProcessComp::GetTransWorld()
{
	FTransform result;
	if (!GetEnable() || !MoveCont)
	{
		return result;
	}


	FVector worldLoc = FVector::ZeroVector;
	FRotator worldRot = FRotator::ZeroRotator;

	//FVector InputDeltaLoc = MoveCont->GetLoc() - startingTrans.GetLocation();
	FVector InputDeltaLoc = MoveCont->GetProcessInputLoc();
	
	//FRotator InputRelRot = MoveCont->GetRot() - startingTrans.GetRotation().Rotator();
	FRotator InputRelRot = MoveCont->GetProcessInputRot();
	FVector scaledDeltaLoc = InputDeltaLoc * InputScales.GetLocation();
	FRotator scaledDeltaRot;// = InputRelRot * InputScales.GetRotation();
	scaledDeltaRot.Pitch = InputRelRot.Pitch * InputScales.GetRotation().Rotator().Pitch;
	scaledDeltaRot.Yaw = InputRelRot.Yaw * InputScales.GetRotation().Rotator().Yaw;
	scaledDeltaRot.Roll = InputRelRot.Roll * InputScales.GetRotation().Rotator().Roll;
	float newRatio = Ratio;


	if ((progressType == EProgressType::FOLLOW_PATH || progressType == EProgressType::HAPTIC_STICK_ANGLE) && AMainCont::INS->IsOperationStarted && isValidStartingTrans)
	{
		//if (!MoveCont->Input->IsHaptic() || (MoveCont->Input->IsHaptic() && (MoveCont->Input->BtnHaptic1 || MoveCont->Input->BtnHaptic2)))
		if (MoveCont->Input->GetKeyPreset("KeyProcess"))
		{

			if (progressType == EProgressType::HAPTIC_STICK_ANGLE)
			{
				if (HapticStickAngleScale > 0)
				{
					float stickYawMagnitude = 0.002;
					float scaledInputStickAngle = MoveCont->GetAxisYaw() * 1;
					float deltaProgress = (scaledInputStickAngle - stickAngle) * stickYawMagnitude * -1;
					newRatio = Ratio + deltaProgress;
					stickAngle = MoveCont->GetAxisYaw();

				}

			}
			else
			{
				newRatio = FMath::Max(scaledDeltaLoc.X, FMath::Max(scaledDeltaLoc.Y, scaledDeltaLoc.Z));

			}

		}
	//}



		SetRatio(newRatio);

		worldLoc = GetPoint(0).GetLocation() + ((GetPoint(1).GetLocation() - GetPoint(0).GetLocation()) * Ratio);
		worldRot = GetPoint(0).GetRotation().Rotator() + ((GetPoint(1).GetRotation().Rotator() - GetPoint(0).GetRotation().Rotator()) * Ratio);

		


	}
	else if (progressType == EProgressType::FREE)
	{
		worldLoc = GetPoint(0).GetLocation() + scaledDeltaLoc;
		worldRot = GetPoint(0).GetRotation().Rotator() + scaledDeltaRot;
	}





	if (ApllyLimits)
	{

		worldLoc.X = FMath::Clamp<float>(worldLoc.X, -Limits.GetLocation().X, Limits.GetLocation().X);
		worldLoc.Y = FMath::Clamp<float>(worldLoc.Y, -Limits.GetLocation().Y, Limits.GetLocation().Y);
		worldLoc.Z = FMath::Clamp<float>(worldLoc.Z, -Limits.GetLocation().Z, Limits.GetLocation().Z);
		worldRot = UOmUtils::GetLimitedRotator(worldRot, Limits.GetRotation().Rotator());

	}


	result = FTransform(worldRot, worldLoc);
	return result;
}



FTransform UProcessComp::GetTransRelative()
{
	FTransform result;
	if (!GetEnable() || !MoveCont)
	{
		return result;
	}



	FVector loc = RelativeStartLocation  + ((RelativeEndLocation - RelativeStartLocation) * Ratio);
	FVector rotVec = RelativeStartRotation + ((RelativeEndRotation -RelativeStartRotation) * Ratio);
	FRotator rot = UOmUtils::FVectorToFRotator(rotVec);


	result = FTransform(rot, loc);
	return result;
}




void UProcessComp::SetRatio(float _ratio)
{
	Ratio = FMath::Clamp<float>(_ratio, -1, 1);
	if(currTool) currTool->BpEventRun("func:(UpdatedProcess)");

	float forceRatio = 0;
		if (Ratio > EndForceThreshold)
			forceRatio = (Ratio - EndForceThreshold) / (1 - EndForceThreshold);
	ForceFeedbackRatio = forceRatio ;
}




FRotator UProcessComp::GetProcessRot(bool _isDelta)
{
	FRotator rot;

	rot = GetTransWorld().GetRotation().Rotator();


	return rot;
}


/* call from MoveAgent */
void UProcessComp::OnKeyPress(FString _keyId)
{
	if (_keyId == "KeyProcess" )
	{

		if (this->progressType == EProgressType::HAPTIC_STICK_ANGLE)
		{
			stickAngle = MoveCont->GetAxisYaw();
		}

		if (!isValidStartingTrans)
		{
			updateStartingTrans();
		}
	}


}




void UProcessComp::updateToolMenu(FTransform _processTrans)
{
	FRotator rot = _processTrans.GetRotation().Rotator();


	float pitch = rot.Pitch;
	float roll = rot.Roll;
	AUICont::INS->WidgetToolAngleX->Slider->SetValue(pitch * -1);
	AUICont::INS->WidgetToolAngleY->Slider->SetValue(roll);
	AUICont::INS->WidgetToolProgress->Slider->SetRatio(GetProgress());

}



void UProcessComp::UpdateParamsFromObserver(FOmValue _params, bool callPawnEnded)
{
	FOmCharList _progress = _params.GetValue("Progress");
	FOmCharList _progressStatus = _params.GetValue("ProgressStatus");
	FOmCharList _progressRot = _params.GetValue("ProgressRot");
	FOmCharList _progressLen = _params.GetValue("ProgressLen");

	FRotator paramRotMin = _params.GetValue("RotMin").GetRotator();
	FRotator paramRotMax = _params.GetValue("RotMax").GetRotator();
	float paramProgressMin = _params.GetValue("ProgressMin").GetFloat(0);
	float paramProgressMax = _params.GetValue("ProgressMax").GetFloat(0);

	

	
	 SetRatio(1); 

	SetStatus(EEventType::PAWN_COMPLETE);


}



FOmValue UProcessComp::GetObserverParams()
{
	FString strProgress = FString::SanitizeFloat(GetProgress());
	FString strProgressStatus = FString::SanitizeFloat(progressStatus);

	FRotator progressRot = savedLastProgressRelTrans.GetRotation().Rotator();
	FVector vprogressRot = FVector(progressRot.Pitch, progressRot.Roll, 0);
	FString strProgressRot = UOmUtils::FVectorToOmValueStr(vprogressRot);

	float sizeofProgress = (GetTransWorld().GetLocation() - GetPoint(0).GetLocation()).Size();

	FString strProgressLen = FString::SanitizeFloat(sizeofProgress);
	observerParams.UpdateValue("Progress", strProgress, true);
	observerParams.UpdateValue("ProgressStatus", strProgressStatus, true);
	observerParams.UpdateValue("ProgressRot", strProgressRot, true);
	observerParams.UpdateValue("ProgressLen", strProgressLen, true);
	return observerParams;
}




void UProcessComp::onClickTimer()
{
	this->SetRatio(this->GetProgress() + this->progressValuePerClick);
}



float UProcessComp::GetProgress()
{
	return Ratio;
}


AToolItem* UProcessComp::GetCurrTool()
{
	return currTool;
}



FTransform  UProcessComp::GetPoint(int8 _index)
{
	FTransform r = _index ? EndPoint : StartPoint;
	FVector loc = r.GetLocation();
	FVector newLoc = GetComponentTransform().Rotator().RotateVector(loc);
	r.SetLocation(newLoc);
	return r;
}







/*_________________________________________________________ */
/*____________________ CLASS<MakeHole> ____________________ */
/*_________________________________________________________ */
void UMakeHoleComp::Init()
{
	//step = -1;
	Super::Init();

}

void UMakeHoleComp::OnKeyPress(FString _keyId)
{
	if (_keyId == "KeyProcess" )
	{
		if (step < 2)
			setStep(step + 1);
	}

	Super::OnKeyPress(_keyId);


}
void UMakeHoleComp::SetProcessEnable(bool _status, AMoveCont* _moveCont, AToolItem* _toolItem, FOmValue _params)
{
	{//before override 
		if (_status)
		{
			hole->SetDepth(0);
			hole->RestoreTrans("init");
		}
			
	}

	Super::SetProcessEnable(_status, _moveCont, _toolItem, _params);

	if (_status)
	{
		defaultLimits = Limits.GetRotation().Rotator();
		{//reset makehole rotation for gobacktask here
			SetRelativeRotation(FRotator::ZeroRotator);
	
			InputScales = FTransform(FRotator(1, 1, 1), FVector::ZeroVector);
			
		}
		
		if (IsShowWidgetBar)
		{
			float xRotMin, xRotMax, yRotMin, yRotMax = 0;
			//FVector paramRotMin = _params.GetValue("RotMin").GetFVector();
			//FVector paramRotMax = _params.GetValue("RotMax").GetFVector();
			FRotator paramRotMin = _params.GetValue("RotMin").GetRotator();
			FRotator paramRotMax = _params.GetValue("RotMax").GetRotator();
			//paramRotMax.Y *= -1;
			/*	xRotMin = (FMath::Min<float>(paramRotMin., paramRotMax.X));
				xRotMax = (FMath::Max<float>(paramRotMin.X, paramRotMax.X));
				yRotMin = (FMath::Min<float>(paramRotMin.Y, paramRotMax.Y));
				yRotMax = (FMath::Max<float>(paramRotMin.Y, paramRotMax.Y));*/

			xRotMin = (FMath::Min<float>(paramRotMin.Pitch, paramRotMax.Pitch));
			xRotMax = (FMath::Max<float>(paramRotMin.Pitch, paramRotMax.Pitch));
			yRotMin = (FMath::Min<float>(paramRotMin.Roll, paramRotMax.Roll));
			yRotMax = (FMath::Max<float>(paramRotMin.Roll, paramRotMax.Roll));


			FString sliderAngleXData = "labels:(0,90),steps:(1),isDouble:(1),valueMin:(" + FString::SanitizeFloat(xRotMin) + "),valueMax:(" + FString::SanitizeFloat(xRotMax) + ")";
			FString sliderAngleYData = "labels:(0,90),steps:(1),isDouble:(1),valueMin:(" + FString::SanitizeFloat(yRotMin) + "),valueMax:(" + FString::SanitizeFloat(yRotMax) + ")";
			AUICont::INS->WidgetToolAngleX->Slider->SetSliderData(sliderAngleXData);
			AUICont::INS->WidgetToolAngleY->Slider->SetSliderData(sliderAngleYData);

		}

	


		stickAngle = 0;


		progressType = EProgressType::FREE;
		setStep(0);

	}
	else
	{
		Limits.SetRotation(defaultLimits.Quaternion()); //reset

		

		if (hole)
		{
			hole->SetActorRotation(hole->GetActorRotation() + GetRelativeTransform().GetRotation().Rotator());
			//float depth = GetProcessLoc(true).Z;
			float depth = -StartPoint.GetLocation().Z + ((EndPoint.GetLocation().Z - StartPoint.GetLocation().Z) * Ratio);
			hole->SetDepth(depth);
		}
		
		SetRelativeRotation(FRotator::ZeroRotator);

		{//#metric makehole params

			float sagital = hole->GetActorRotation().Pitch * -1;
			float axial = hole->GetActorRotation().Roll;
			float depth = hole->GetDepth();

			//hole bilgilerini kaydediyoruz
			UTaskCont::INS->currProcess->MetricEnd.AddDataParam("hole_sagital", FString::SanitizeFloat(sagital));
			UTaskCont::INS->currProcess->MetricEnd.AddDataParam("hole_axial", FString::SanitizeFloat(axial));
			UTaskCont::INS->currProcess->MetricEnd.AddDataParam("hole_depth", FString::SanitizeFloat(depth));

			//vertabra bilgilerini kaydediyoruz
			FRotator boneRot = this->GetOwner()->GetRootComponent()->GetAttachParent()->GetRelativeTransform().Rotator();
			float parentSagital = boneRot.Pitch * -1;
			float parentAxial = boneRot.Roll;
			UTaskCont::INS->currProcess->MetricEnd.AddDataParam("vertabra_sagital", FString::SanitizeFloat(parentSagital));
			UTaskCont::INS->currProcess->MetricEnd.AddDataParam("vertabra_axial", FString::SanitizeFloat(parentAxial));
		}

		
	}

}

void UMakeHoleComp::setStep(int8 _step)
{

	if (_step == 0)
	{
		ApllyLimits = true;
		Limits.SetRotation(FRotator(defaultLimits.Pitch, 0, 0).Quaternion());

	}
	else if (_step == 1)
	{
		updateStartingTrans();
		SaveTrans("BeforeProcess");
		//SetRelativeRotation(GetRelativeTransform().GetRotation().Rotator() + GetProcessRot(true));

		ApllyLimits = true;
		Limits.SetRotation(FRotator(0, 0, defaultLimits.Roll).Quaternion());
		
		
	}
	else if (_step == 2)
	{
		SaveTrans("BeforeProcess");
		//SetRelativeRotation(GetRelativeTransform().GetRotation().Rotator() + GetProcessRot(true));
		savedLastProgressRelTrans = GetRelativeTransform();



		Limits.SetRotation(FRotator(0, 0, 0).Quaternion());
		ApllyLimits = false;
		progressType = EProgressType::FOLLOW_PATH;
		InputScales.SetLocation(FVector(0, 0, -0.5));
		updateStartingTrans();
		
		progressStatus = 1; //now we re waiting vr trigger click
		
	}
	else if (_step == 3)
	{
		

	}

	step = _step;
	AUICont::INS->WidgetToolAngleX->SetVisible(step == 0);
	AUICont::INS->WidgetToolAngleY->SetVisible(step == 1);
	AUICont::INS->WidgetToolProgress->SetVisible(step == 2);
	
}





void UMakeHoleComp::UpdateParamsFromObserver(FOmValue _params, bool callPawnEnded)
{
	FOmCharList _progress = _params.GetValue("Progress");
	FOmCharList _progressStatus = _params.GetValue("ProgressStatus");
	FRotator paramRotMin = _params.GetValue("RotMin").GetRotator();
	FRotator paramRotMax = _params.GetValue("RotMax").GetRotator();
	float paramProgressMin = _params.GetValue("ProgressMin").GetFloat(0);
	float paramProgressMax = _params.GetValue("ProgressMax").GetFloat(0);

	if (hole)
	{
		FRotator averageRot = UOmUtils::GetAverageRot(paramRotMin, paramRotMax);
		SetRelativeRotation(GetRelativeTransform().GetRotation().Rotator() + averageRot);

		float locZ = paramProgressMin + ((paramProgressMax - paramProgressMin) / 2);
		EndPoint.SetLocation( FVector(0,0,-locZ));

	}


	SetRatio(1);

	SetStatus(EEventType::PAWN_COMPLETE);


}

int UMakeHoleComp::GetProcessStep()
{
	return step;
}





