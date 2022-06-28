#pragma once
#include <vector>
#include "../Utils/OmValue.h"
#include "Observer.generated.h"

using namespace std;

//struct FResultProcess;
class URule;
class UProcess;
//class UOmComp;


UCLASS()
class  OMCEM_API UObserver : public UObject
{
	GENERATED_BODY()
public:
	static UObserver* INS;
	UObserver();
	void Init();
	 URule* Send(FOmValue _requestbool , bool _print=true);
	 FProcessResult CheckCurrProcess();
	URule* CheckRule(FString _ruleId);
	FProcessResult CheckProcess(FString _tpId);
private:
	

};

