#pragma once
 
#include "UserWidget.h"
#include "Object.h"
#include "UIBase.generated.h"

class UButton;
class UUIController;


 UCLASS()
class OMCEM_API UUIBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY() FString Id;

	virtual void Init();
	virtual void Open();
	virtual void Close();
	UFUNCTION(BlueprintCallable, Category = OMCEM) 	virtual void OnClickClose();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString& cmd);
private:
	
};
