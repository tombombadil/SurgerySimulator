#include "UIToolMenu.h"
#include "OMCEM.h"
#include "UserWidget.h"
#include "WidgetComponent.h"
#include "Runtime/UMG/Public/UMG.h"
#include "OmEngine/Controllers/ToolsCont.h"





void UUIToolMenu::Open()
{
	UUIBase::Open();
	BpEventOpen();

}

void UUIToolMenu::Close()
{
	UUIBase::Close();
	BpEventClose();

}

