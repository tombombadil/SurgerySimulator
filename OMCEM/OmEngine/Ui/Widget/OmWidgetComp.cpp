#include "OmWidgetComp.h"
#include "OMCEM.h"



void UOmWidgetComp::BeginPlay()
{
	Super::BeginPlay();
	//ui = GetUserWidgetObject();
	SetVisible(isVisible);
	

}

void UOmWidgetComp::Init()
{

}

void UOmWidgetComp::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	Render(DeltaTime);
}

void UOmWidgetComp::SetVisible(bool _visible)
{
	isVisible = _visible;
	SetVisibility(isVisible);
	UOmWidgetComp* child = Cast<UOmWidgetComp>(GetChildComponent(0));
	if (child)
	{
		child->SetVisibility(isVisible);
	}
}



void UOmWidgetComp::Reset(bool _resetVisiblity)
{
	if (_resetVisiblity) SetVisible(false);
}

void UOmWidgetComp::Render(float DeltaTime)
{
}
