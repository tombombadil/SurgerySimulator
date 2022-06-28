#pragma once
 
#include "Blueprint/UserWidget.h"
#include "OmEngine/Ui/UIBase.h"
#include "UIXRay.generated.h"



 UCLASS()
class OMCEM_API UUIXRay : public UUIBase
{
	GENERATED_BODY()
public:
	void ShowResult(FString _idRuleRot, FString _idRuleLen);
	bool ExecuteResult();
	void ClearResultText();

	bool GetTaskResult();
private:
	bool taskErr = false;
 };


 UCLASS()
	 class OMCEM_API UUIStatusMonitor : public UUIBase
 {
	 GENERATED_BODY()
 public:
	 void SetTexts(FString _currTaskName, FString _nextTaskName);
 private:
 };