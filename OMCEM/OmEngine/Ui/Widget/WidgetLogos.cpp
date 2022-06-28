#include "WidgetLogos.h"
#include "OMCEM.h"
#include "OmEngine/Utils/OmValue.h"
#include "Runtime/UMG/Public/UMG.h"
#include "OmEngine/Components/ProcessComp.h"
#include <PanelWidget.h>




void UWidgetLogos::BeginPlay()
{
	UOmWidgetComp::BeginPlay();
	SetVisible(true);
	ui = Cast<UUIBase>(GetUserWidgetObject());
	if (ui)
	{
		 panel = Cast<UPanelWidget>(ui->GetWidgetFromName("Panel"));
		
		for (UWidget* wid : panel->GetAllChildren())
		{
			UUserWidget* userwid = Cast<UUserWidget>(wid);
			userwid->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
}


void UWidgetLogos::Init()
{
	SetVisible(true);
	TSharedPtr<FJsonObject>  jsonLogos = UResource::INS->GetMainJsonObj("Logos");
	if (jsonLogos)
	{
		for (auto It = jsonLogos->Values.CreateConstIterator(); It; ++It)
		{
				TSharedPtr<FJsonValue> jValue = (TSharedPtr<FJsonValue>) It.Value();
				FString key = It.Key();
				FString value = jValue->AsString();
				UUserWidget* logo = getLogo(key);
				if (logo)
				{
					if (value == IdView)
						logo->SetVisibility(ESlateVisibility::Visible);
				}

		}
	}
}



UUserWidget* UWidgetLogos::getLogo(FString _name)
{
	UUserWidget* u = 0;
	if(ui) u = Cast<UUserWidget>(ui->GetWidgetFromName(*_name));
	return u;
}