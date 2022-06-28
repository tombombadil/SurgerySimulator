#include "Observer.h"
#include "OMCEM.h"
#include "TaskCont.h"
#include "MainCont.h"
#include "../Utils/OmUtils.h"

UObserver* UObserver::INS = NULL;

UObserver::UObserver()
{

}

void UObserver::Init()
{
	UObserver::INS = this;
}






URule* UObserver::Send(FOmValue _request, bool _print)
{
	if(_print) Debug("		Observer.Send -> : " + _request.ToString());
	URule* resultRule = 0;
	if (UTaskCont::INS->currProcess)
	{
		UTaskCont::INS->currProcess->UpdateRequest(_request);
		resultRule = CheckCurrProcess().SuccRule;



		
		
	}


	


	return resultRule;
}




FProcessResult UObserver::CheckCurrProcess()
{

	FProcessResult result = CheckProcess(FString::FromInt(UTaskCont::INS->currProcess->Index));
	if (result.SuccRule || result.Result && UTaskCont::INS->currProcess)
	{
		{
			UTaskCont::INS->currProcess->EventStatus.Broadcast(EEventType::COMPLETE, result);
		}
	}

			
		return result;




	
		
}



URule* UObserver::CheckRule(FString _ruleId)
{
	URule* rule = UTaskCont::INS->GetRule(_ruleId);
	rule->GetResult();
	return rule;
}

FProcessResult UObserver::CheckProcess(FString _tpId)
{
	int8 tpIndex = UOmUtils::ToInt(_tpId);
	UTask* currTask = UTaskCont::INS->currTask;
	FProcessResult result;
	if (currTask)
	{
		UProcess* p = currTask->GetProcess(tpIndex);
	
		if (p)
		{
			result = p->Check();
			if (result.SuccRule)	Debug("#task.CheckProcess.result.SuccRule : " + result.SuccRule->ToString(), 1);

		}

	}
	

	return result;



}
