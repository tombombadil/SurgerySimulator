#include "UIXRay.h"
#include "OMCEM.h"
#include "OmEngine/Controllers/XRayRender.h"
#include "OmEngine/Controllers/Observer.h"
#include "OmEngine/Utils/Resource.h"
#include "Runtime/UMG/Public/UMG.h"
#include "OmEngine/Controllers/TaskCont.h"
#include "OmEngine/Controllers/UICont.h"
#include "../Components/CheckOutOfMeshComp.h"




void UUIXRay::ShowResult(FString _idTool, FString _idMeshesList)
{
	if (_idTool != "NULL" && _idMeshesList != "NULL")
	{
		AItem* tool = Cast<AItem>(UResource::INS->GetItem(_idTool));
		if (tool)
		{
			

			UCheckOutOfMeshComp* checkOutMeshComp = Cast<UCheckOutOfMeshComp>( tool->GetComponentByClass(UCheckOutOfMeshComp::StaticClass()));
			if (checkOutMeshComp)
			{
				TArray<FString> listMeshes =	 UOmUtils::ParseString(_idMeshesList, ",");

				if (listMeshes.IsValidIndex(0))
				{
					bool toolInnerMeshResult = checkOutMeshComp->GetInnerResult(listMeshes);
					taskErr = !toolInnerMeshResult;
					


					//AUICont::INS->SetTooltipActivity(EWidgetId::TOOLTIP_XRAY, taskErr, _msg, 1);
				}
				

				if (taskErr)
				{
					FString msg = UConfig::GET_MSG("err_xray_probe");
					BpEventRun("func:(ShowMsg),msg:(" + msg + ")");
				}
				

				
			}
		}
		
		
		
	}
	
}

bool UUIXRay::ExecuteResult()
{
	ClearResultText();
	
	//AXRayRender::INS->RefreshCaptures();
	if(GetTaskResult())
		UObserver::INS->Send(FOmValue("XRayResultSucc:(1)"));
	else
		UObserver::INS->Send(FOmValue("XRayResultFail:(1)"));

	return GetTaskResult();
	
}

void UUIXRay::ClearResultText()
{
	//AUICont::INS->SetTooltipActivity(EWidgetId::TOOLTIP_XRAY, false);
	BpEventRun("func:(ClearMsg)");
}



bool UUIXRay::GetTaskResult()
{
	return !taskErr;
	//return true; //to-do : scoliosis de acilar hata veriyordu gecici sabitledim.
}




void UUIStatusMonitor::SetTexts(FString _currTaskName, FString _nextTaskName)
{
	
	UTextBlock* txtCurrTask = (UTextBlock*)GetWidgetFromName("CurrTaskName");
	UTextBlock* txtNextTask = (UTextBlock*)GetWidgetFromName("NextTaskName");
	if(txtCurrTask)txtCurrTask->SetText(FText::FromString(_currTaskName));
	if (txtNextTask)txtNextTask->SetText(FText::FromString(_nextTaskName));
}
