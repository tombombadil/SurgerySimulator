#pragma once



#include "OMCEM.h"
#include "../Utils/OmValue.h"
#include "../Utils/Server.h"
#include "Process.generated.h"



using namespace std;


class UTaskCont;
class UPModel;
class URule;
class AToolItem;
struct FOmValue;
struct FOmValue;
struct FMetric;
struct FOmValues;
struct FMetricJson;






/*____________________ Rule ____________________ */
UCLASS() class OMCEM_API URule : public  UObject
{
	GENERATED_BODY()
public:
	static FString IDPREFIX;
	bool GetComparisonResult(struct FComparison _comparison, FString _value);
	UPROPERTY()	FString Id;
	UPROPERTY()	int Index;
	//UPROPERTY() UPModel* model;
	 FOmValues commands;
	UPROPERTY()	FString comparison;
	EComparisionResult  comparisionResult;
	UPROPERTY()	FString msg;
	UPROPERTY()	bool isPrint;
	/* offlineRule : Realtime'da calistirilmayan GoToTask ile otomatik calisitirilan komut(rule) demek */
	UPROPERTY()	bool isOffline = false;
	UPROPERTY()	int Result = 0;
	FEventProcessStatus EventStatus;
	void Init(FString _comparison, FString _commands);
	static URule* Create( FString _comparison, FString _commands);
	FString ToString();
	UFUNCTION(BlueprintCallable, Category = "OM3")  EComparisionResult GetResult();
	void SetIsCmdExecuted(bool _isExecuted);
	bool GetIsCmdExecuted();

public:
	UPROPERTY() bool isSaved = false;

	bool Save(FOmValue _request);
	//FOmValue GetKey() { return key; };
	TArray<FString> GetKeys();;
	FOmValue GetKeyAsParam();;
	FComparison GetComparisonByOperator(FString _keyName, FString _operator);;
	FOmValue GetRequest() { return request; };
	void Reset();

private:
	 TArray<FComparison> listComparisons; // a==y,b==x etc..
	 TArray<FString> listComparisonOps; //&&,||,&& etc..
	//FOmValue key;
	FOmValue request;
	bool isCmdExecuted;
};






/*____________________ Process ____________________ */
UCLASS() class OMCEM_API UProcess : public UObject
{
	GENERATED_BODY()
public:
	static FString IDPREFIX;
	UPROPERTY()  TArray<URule*> listRules;
	UPROPERTY() int8 totalRule;
	UPROPERTY() FString Id;
	UPROPERTY() int8 Index = -1;
	UPROPERTY() bool IsCallStartCmdOnProcessComp;
	UPROPERTY() bool IsCompleted = false;
	 FMetricJson MetricStart;
 FMetricJson MetricEnd;
	 FEventProcessStatus EventStatus;
	 UPROPERTY() FOmValues SucceedCommand;

	UProcess() 
	{
	}

	static UProcess* Create();
	void UpdateRequest(FOmValue _request);
	/* gelen request ile rule listesindeki rulelari karsilastirir. keyleri req keyleri ile uyusan bir rule varsa, ayrica bu rule req karsilastirmasi dogru ise bu rule 'SuccRule' olarak geri doner. */ 
	FProcessResult Check();
	virtual void AddRule(URule* _rule);
	void Reset();
	void OnStarted();
	/* call manual by taskcont to complete and run all commands */
	void CompleteManual();
	void OnCompleted();
	virtual FString ToString();
	private:
	

};









