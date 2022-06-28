#include "HudWidgetComp.h"
#include "OMCEM.h"




void UHudWidgetComp::BeginPlay()
{
	UOmWidgetComp::BeginPlay();

	//uiPopupTitleTxt = Cast<UTextBlock>(ui->GetWidgetFromName("Title"));
	uiPopupMsgTxt = Cast<UTextBlock>(GetUserWidgetObject()->GetWidgetFromName("Msg"));


	Reset();




}

void UHudWidgetComp::Init()
{
	
}



void UHudWidgetComp::Render(float DeltaTime)
{
	if (isEnableAutoClose)
	{
		autoCloseTimer -= DeltaTime;
		if (autoCloseTimer < 0)
		{
			isEnableAutoClose = false;
			SetVisible(false);
		}
	}
	
}




void UHudWidgetComp::Reset(bool _resetVisiblity)
{
	if(uiPopupMsgTxt) uiPopupMsgTxt->SetText(FText::FromString(""));
	
	Super::Reset(_resetVisiblity);
}

