#include "ToolWidgetComp.h"
#include "OMCEM.h"
#include "TextBlock.h"
#include "ProgressBar.h"
#include "OmEngine/Utils/OmValue.h"
#include "Runtime/UMG/Public/UMG.h"
#include "OmEngine/Components/ProcessComp.h"
#include "../UIBase.h"




void UToolWidgetComp::BeginPlay()
{
	UOmWidgetComp::BeginPlay();

	SliderAngle = Cast<UOmWidgetSliderCircle>(GetUserWidgetObject()->GetWidgetFromName("SliderAngle"));
	SliderAngle->Init();
	Reset();
}

void UToolWidgetComp::Init()
{
	
}
void UToolWidgetComp::Reset(bool _resetVisiblity)
{
	Super::Reset(_resetVisiblity);
	SliderAngle->DefaultValue = 0.5;
}


/* widget progress*/

void UToolWidgetProgress::BeginPlay()
{
	UOmWidgetComp::BeginPlay();
	Slider= Cast<UOmWidgetSlider>(GetUserWidgetObject()->GetWidgetFromName("Slider"));
	Slider->Init();
	Reset();
}

void UToolWidgetProgress::SetStyle(FString _styleName)
{
	//UUIBase* ui = Cast<UUIBase>(GetUserWidgetObject());
	//if (ui) ui->BpEventRun("func:(SetStyle),style:("+ _styleName +")");
	TSubclassOf<UUIBase> ui = Styles[_styleName];
	if (ui)
	{
		SetWidgetClass(ui);
		Slider = Cast<UOmWidgetSlider>(GetUserWidgetObject()->GetWidgetFromName("Slider"));
		Slider->Init();
	}
}
void UToolWidgetProgress::SetTitle(FString _txt)
{

		if (!_txt.IsValidIndex(0))
			_txt = "Progress :";
		UTextBlock* txtTitle = (UTextBlock*)GetUserWidgetObject()->GetWidgetFromName("TxtTitle");
		if (txtTitle) txtTitle->SetText(FText::FromString(_txt));

		

}

void UToolWidgetProgress::Reset(bool _resetVisiblity)
{
	Super::Reset(_resetVisiblity);
	Slider->DefaultValue = 0.5;

}


