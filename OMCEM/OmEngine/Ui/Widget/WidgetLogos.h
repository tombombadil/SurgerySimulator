#pragma once


#include "OmEngine/Ui/OmWidgetSliderCircle.h"
#include "OmEngine/Ui/Widget/OmWidgetComp.h"
#include "WidgetLogos.generated.h"

struct FProcessCompInfo;

UCLASS(ClassGroup = (OMCEM), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UWidgetLogos : public UOmWidgetComp
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
	void Init() override;
	/* IdView (1) wall (2) monitor */
	UPROPERTY(EditAnywhere, Category = "OM3") FString IdView = "1";
	UPROPERTY() UUIBase* ui;
	UPROPERTY()	UPanelWidget* panel;

private:
	UUserWidget* getLogo(FString _name);
	
	
	

};
