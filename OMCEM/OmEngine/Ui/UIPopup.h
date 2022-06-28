#pragma once
 
#include "OmEngine/Ui/UIBase.h"
#include "Blueprint/UserWidget.h"
#include "Object.h"
#include "UIPopup.generated.h"

class UEditableText;
class UMultiLineEditableText;

UENUM()
enum class EPopupType : uint8
{
	PROCESS_ERR,
	PROCESS_SUCC

};

 UCLASS()
class OMCEM_API UUIPopup : public UUIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite) UEditableText* txtTitle;
	UPROPERTY(BlueprintReadWrite) UMultiLineEditableText* txtMsg;

	void SetTitle(FString _msg);
	void SetMsg(FString _msg);
	void SetType(EPopupType _type);
	EPopupType GetType() { return type; };
	
private:
	EPopupType type;
};

