#include "Process.h"
#include "OMCEM.h"
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Utils/OmValue.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Utils/Resource.h"
#include "Runtime/Core/Public/Misc/DateTime.h"
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Utils/OmStructs.h"
#include "OmEngine/Utils/Server.h"
#include "TaskCont.h"
#include "../Components/ProcessComp.h"
#include "MainCont.h"

FString URule::IDPREFIX = "rule";
FString UProcess::IDPREFIX = "process";






/*____________________ Rule ____________________ */
//int8 URule::GetComparisionsResult()
//{
//	
//	TArray<FString> _parseArrComparisons;
//	int foundAnd = comparison.Find("&");
//	int foundOr = comparison.Find("|");
//	bool isAndOperator = false;
//	if (foundAnd > -1)
//	{
//		isAndOperator = true;
//		_parseArrComparisons = UOmUtils::ParseString(comparison, "&");
//	}
//	else if (foundOr > -1)
//	{
//		isAndOperator = false;
//		_parseArrComparisons = UOmUtils::ParseString(comparison, "|");
//	}
//	else
//	{
//		isAndOperator = true;
//		_parseArrComparisons.Add(comparison);
//	}
//
//	int8 _result = isAndOperator?1:0;
//
//	bool comprasionResult = false;
//
//	FOmValue compareValue = model->GetKey().GetCompare(model->GetRequest());
//	//Log(+ "#task" + Id + " key / req / compareVal : " + model->GetKey().ToString() + "____/____" + model->GetRequest().ToString() + "____/____" +   compareValue.ToString());
//
//	for (FString _comparison : _parseArrComparisons)
//	{
//		
//		TArray<FString> _parseComparison = UOmUtils::ParseString(_comparison, "=");
//		FString idKey = _parseComparison[0];
//		
//		FString patternComprasion = _parseComparison[1]; //eğer patternValueId karşılığı bu ise..
//		
//		FString compare = compareValue.GetValue(idKey).GetString();
//
//		comprasionResult = (compare == patternComprasion);
//
//		if (compare.IsEmpty()) //bir key eslemesi yok demektir
//		{
//			_result = -1;
//		} 
//		else
//		{
//			if (isAndOperator) //and
//			{
//				if (!comprasionResult)	_result = 0;
//			}
//			else { //or
//				if (comprasionResult)	_result = 1;
//			}
//		}
//		
//
//
//
//	}
//	status = (_result > 0);
//	return _result;
//}

/*____________________ om3 ____________________ */

bool URule::GetComparisonResult(FComparison _comparison, FString _value)
{
	bool r = false;
	

	FString Operator = _comparison.Operator;
	FString CompValue = _comparison.Value2;


	
	int8 valueType = 0; //0(string), 1(int), 2(float)


	if (CompValue.IsNumeric())
	{

		if (CompValue.Contains("."))
		{
			valueType = 2;
		}
		else {
			valueType = 1;
		}
	}


	int8 opType = 0;
	if (Operator == "=!" || Operator == "=!")  opType = 0;
	else if (Operator == "==")  opType = 1;
	else if (Operator == "<=")  opType = 2;
	else if (Operator == ">=")  opType = 3;
	else if (Operator == "<")  opType = 4;
	else if (Operator == ">")  opType = 5;



	//Debug("		om3 c1 cont @_comparison   " + _comparison.ToString() + " ------ @_value: " + _value);
		//Debug("		om3 c1 cont ...  " + _value + Operator + CompValue + " valueType : " + FString::FromInt(valueType) + " opType: " + FString::FromInt(opType));

	if (valueType == 0) //string
	{
		if (opType == 0)  r = !(CompValue == _value);
		else  r = (CompValue == _value);
	}
	else if (CompValue.IsNumeric() && _value.IsNumeric())
	{
		 if (valueType == 1)  //int
		{

			int keyValue = (int)UOmUtils::ToInt(CompValue);
			int reqValue = (int)UOmUtils::ToInt(_value);

			if (opType == 0)  r = !(reqValue == keyValue);
			else if (opType == 1)  r = (reqValue == keyValue);
			else if (opType == 2)  r = (reqValue <= keyValue);
			else if (opType == 3)  r = (reqValue >= keyValue);
			else if (opType == 4)  r = (reqValue < keyValue);
			else if (opType == 5)  r = (reqValue > keyValue);
		}
		else if (valueType == 2) //float
		{
			float keyValue = (float)UOmUtils::ToFloat(CompValue);
			float reqValue = (float)UOmUtils::ToFloat(_value);

			if (opType == 0)  r = !(reqValue == keyValue);
			else if (opType == 1)  r = (reqValue == keyValue);
			else if (opType == 2)  r = (reqValue <= keyValue);
			else if (opType == 3)  r = (reqValue >= keyValue);
			else if (opType == 4)  r = (reqValue < keyValue);
			else if (opType == 5)  r = (reqValue > keyValue);
		}
	}
	
	//Debug("		om3 c1 cont result final : ", r);

	return r;
}


void URule::Init(FString _comparison, FString _commands)
{
	if (_comparison.IsValidIndex(0))
	{
		comparison = _comparison;
		TArray<FString> parsedArr = UOmUtils::ParseComparison(_comparison, true);
		for (int8 i = 0; i < parsedArr.Num(); i++)
		{
			FString s = parsedArr[i];

			if (i % 2 == 0) //comparission
			{
				FComparison c = FComparison(s);
				listComparisons.Add(c);
			}
			else //comparission operator (&& , ||)
			{
				listComparisonOps.Add(s);
			}
		}
	}
	if (_commands.IsValidIndex(0)) commands = FOmValues(_commands);
	msg = "";
}

URule* URule::Create(FString _comparison, FString _commands)
{
	URule* r = NewObject<URule>();
	r->Init(_comparison, _commands);
	return r;
}

FString URule::ToString()
{
	return "Id:(" + Id + "), comparison:[" + comparison + "],cmd:[" + commands.ToString() + "],comparisionResult:[" + FString::FromInt((int)comparisionResult) + "]";
}

EComparisionResult URule::GetResult()
{
	//ebug("----------om3.rule.getresult() request : " + GetRequest().ToString() + " comparison : " + comparison);
	EComparisionResult result = EComparisionResult::KEY_NOT_FOUND_IN_REQ;

	if (!comparison.IsValidIndex(0)) //comparison null ise gerek yok bakmaya
	{
		result = EComparisionResult::COMPARISION_TRUE;
		return result;
	}
	TArray<FString> _parseArrComparisons;
	int foundAnd = comparison.Find("&&");
	int foundOr = comparison.Find("||");
	bool isAndOperator = false;
	if (foundAnd > -1)
	{
		isAndOperator = true;
		_parseArrComparisons = UOmUtils::ParseString(comparison, "&&");
	}
	else if (foundOr > -1)
	{
		isAndOperator = false;
		_parseArrComparisons = UOmUtils::ParseString(comparison, "||");
	}
	else
	{
		isAndOperator = true;
		_parseArrComparisons.Add(comparison);
	}



	 
	//once kaydedilen GetKEt() leri loop'a sokup, gelen req icinde bu keylerin hepsinin olup olmadigina bakiyor. GetKey() icindeki herhangi bir key GetRequest() icinde yoksa EComparisionResult::KEY_NOT_FOUND_IN_REQ donuyoruz.
	int8 compIndex = 0;
	//TArray<bool> pairsResult; //(c0 -op- c1), (c1 -op- c2)...
	bool lastResult = false; //for icinde sureki guncelleniyor. once pairsResult = c0 (op) c1, sonra pairsResult = pairsResult (op) c2 ...

	//last result ilk(0) sonuctan baslasin
	if (listComparisons.IsValidIndex(0))
	{
		FComparison c0 = listComparisons[0];
		FString reqValue0 = GetRequest().GetValue(c0.Value1).GetString();
		lastResult = GetComparisonResult(c0, reqValue0);
		if(lastResult)  	result = EComparisionResult::COMPARISION_TRUE;
		//Debug("c0 " + c0.ToString()+  " lastResult ", lastResult);
		//Debug("c0 " + c0.ToString() +" listComparisonOps.num ", listComparisonOps.Num());
	}

	for (int8 i = 0; i < listComparisonOps.Num(); i++)
	{
		FString opPair = listComparisonOps[i];
		bool isAndOp = (opPair == "&&") ? true : false;//bu bir && operator mu? yoksa || mi?

		if (listComparisons.IsValidIndex(i + 1))
		{
			FComparison c = listComparisons[i+1];
			if (GetRequest().ContainsKey(c.Value1)) //req icinde kayitli keyi ariyoruz
			{
				FString reqValue1 = GetRequest().GetValue(c.Value1).GetString();
				bool cResult = GetComparisonResult(c, reqValue1);


				lastResult = isAndOp ? (cResult && lastResult) : (cResult || lastResult);
			}
			else
			{
				result = EComparisionResult::KEY_NOT_FOUND_IN_REQ;
				lastResult = false;
				break;
			}
			


			
		}


		compIndex += 2;
	}

	
	

	if (result != EComparisionResult::KEY_NOT_FOUND_IN_REQ)
	{
		if(lastResult) result = EComparisionResult::COMPARISION_TRUE;
		else result = EComparisionResult::COMPARISION_FALSE;
	}
	comparisionResult = result;
	return result;
}


void URule::SetIsCmdExecuted(bool _isCmdExecuted)
{
	isCmdExecuted = _isCmdExecuted;
}

bool URule::GetIsCmdExecuted()
{
	return isCmdExecuted;
}


bool URule::Save(FOmValue _request)
{
	
	for (FString k : GetKeys())
	{
		if (!_request.GetKeys().Contains(k)) return false;
	}

	request = _request;
	return true;
}

TArray<FString> URule::GetKeys()
{
	TArray<FString> r;
	for (FComparison c : listComparisons)
	{
		r.Add(c.Value1);
	}
	return r;
}

FOmValue URule::GetKeyAsParam()
{
	FOmValue r;
	for (FComparison c : listComparisons)
	{
		r.Add(FOmValue(c.Value1 + ":(" + c.Value2 + ")"), true);
	}
	return r;
}

FComparison URule::GetComparisonByOperator(FString _keyName, FString _operator)
{
	FComparison r;
	for (FComparison c : listComparisons)
	{
		if (c.Value1 == _keyName && c.Operator == _operator)
			r = c;
	}
	return r;
}

void URule::Reset()
{
	isSaved = false;
	request.Reset();
	request = FOmValue("");
	Result = 0;
	SetIsCmdExecuted(false);


}





UProcess* UProcess::Create()
{
	UProcess* p = NewObject<UProcess>();
	return p;
}

void UProcess::UpdateRequest(FOmValue _request)
{
	for (URule* rule : listRules)
	{
		bool saveResult = rule->Save(_request);
		rule->isSaved = true;
	}
}

FProcessResult UProcess::Check()
{
	FProcessResult result;
	result.Process = this;
	URule* currRule = 0;
	//if (listRules.Num() == 0) result.status = 0;
	int8 i = 0;
	for (URule* r : listRules)
	{
		Debug("#task.Process[" + Id + "].[" + FString::FromInt(i) + "].rule :  " + r->ToString() + ".GetIsCmdExecuted() : " + FString::FromInt((int)r->GetIsCmdExecuted()) + "  r->Result: ", r->Result, 1);
		if (!r->GetIsCmdExecuted() && r->Result == 0)
		{
			if (r->GetResult() == EComparisionResult::COMPARISION_TRUE) //req ile rule keyleri eslestirildi!
			{
				currRule = r;
				break;
			}


		}
		i++;
	}

	//URule* resultRule = 0;
	if (currRule)
	{
		currRule->comparisionResult = currRule->GetResult();

		Debug("#task.Process[" + Id + "].[" + FString::FromInt(i) + "] .CurrRule :  " + currRule->ToString() + " @CurrRule.comparisionResult: " + FString::FromInt((int)currRule->comparisionResult), 1);
		//if (comprasionResult == EComparisionResult::KEY_NOT_FOUND_IN_REQ) //key eslemesi yok (gelen request icinde beklenen key bulunamadi)
		//{
		//	result.status = -1;
		//	//return result;
		//}
		if (currRule->comparisionResult == EComparisionResult::COMPARISION_FALSE) //key eslemesi var, value eslemesi yok
		{
			//if(currRule->IsSkippable)
			{
				if (currRule->Index == totalRule - 1)
				{
					//currRule->status = EComparisionResult::COMPARISION_TRUE;
					//return currRule;
				}
			}
		}

		if (currRule->comparisionResult == EComparisionResult::COMPARISION_TRUE) //key eslesmesi var, values eslesmesi var
		{
			currRule->Result = 1;
		}


		//}


	}

	result.SuccRule = currRule;

	bool allRulesResult = true;
	for (URule* r : listRules)
	{
		if (!r->Result) allRulesResult = false;
	}
	result.Result = allRulesResult;
	Debug("#task.process(" + Id + ")  @Result(final): ", result.Result, 1);
	return result;
}

void UProcess::AddRule(URule* _rule)
{
	_rule->Index = listRules.Num();
	listRules.Add(_rule);
	totalRule = listRules.Num();
}

void UProcess::Reset()
{
	for (URule* rule : listRules)
	{
		rule->Reset();
	}
	IsCompleted = false;
	//Result = FResultProcess();
}

void UProcess::OnStarted()
{

	if (AMainCont::INS->IsOperationStarted)
		if(MetricStart.Json.IsValid())
		UServer::INS->SendMetric(MetricStart);
}

void UProcess::CompleteManual()
{
	for (int8 i3 = 0; i3 < listRules.Num(); i3++)
	{
		URule* r = listRules[i3];
		FOmValues commands = r->commands;
		if (commands.IsValid)
		{
			if (!r->comparison.IsValidIndex(0))  if (!r->GetIsCmdExecuted()) UTaskCont::INS->RunCommands(commands);


			for (FOmValue ov : commands.list)
			{
				if (ov.IsValid)
				{
					if (ov.GetValue("func").GetString(0) == "StartFieldProcess")
						if (ov.GetValue("func").IsValid)
						{
							FString idComp = ov.GetValue("comp").GetString(0);
							FString toolId = ov.GetValue("item").GetString(0);
							UProcessComp* processComp = Cast<UProcessComp>(UResource::INS->GetOmComp(idComp));
							AToolItem* tool = Cast<AToolItem>(UResource::INS->GetItem(toolId));

							if (processComp)
							{
								//FOmValue processParams = r->GetKeyAsParam();
								FOmValue processParams = r->commands.GetOmValue(0);
								processParams.UpdateValue("IsByTaskCont", "1", true);
								processComp->UpdateParamsFromObserver(processParams, false);
							}

						}


				}
			}


		}


	}

	if (SucceedCommand.IsValid)
		UTaskCont::INS->RunCommands(SucceedCommand);
}

void UProcess::OnCompleted()
{
	IsCompleted = true;
}

/*____________________ Process ____________________ */

FString UProcess::ToString()
{

	FString s = Id + "("+ FString::FromInt(Index) + ")";
	int8 i = 0;
	for (URule* r : listRules)
	{
		
		s += " @Rule(" + FString::FromInt(i) + ") : " + r->ToString() + ", " ;
		i++;
	}


	return s;
}
