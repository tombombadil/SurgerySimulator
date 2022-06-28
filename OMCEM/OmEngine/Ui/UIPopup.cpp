#include "UIPopup.h"
#include "OMCEM.h"
#include "UserWidget.h"
#include "WidgetComponent.h"
#include "Runtime/UMG/Public/UMG.h"


void UUIPopup::SetTitle(FString _msg)
{
	txtTitle = (UEditableText*)GetWidgetFromName("_title");
	txtTitle->SetText(FText::FromString(FString(_msg)));
}


void UUIPopup::SetMsg( FString _msg)
{
	txtMsg = (UMultiLineEditableText*)GetWidgetFromName("_msg");
	txtMsg->SetText(FText::FromString(_msg));

	
}

void UUIPopup::SetType(EPopupType _type)
{
	type = _type;
	switch (_type)
	{
	case EPopupType::PROCESS_ERR:
		SetTitle("HATA!");
		break;
	case EPopupType::PROCESS_SUCC:
		SetTitle("TASK SUCCEED");
		break;
	}
}