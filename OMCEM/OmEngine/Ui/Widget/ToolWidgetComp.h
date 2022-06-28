#pragma once


#include "OmEngine/Ui/OmWidgetSliderCircle.h"
#include "OmWidgetComp.h"
#include "ToolWidgetComp.generated.h"

struct FProcessCompInfo;

UCLASS(ClassGroup = (OMCEM), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UToolWidgetComp : public UOmWidgetComp
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void Init();
	UOmWidgetSliderCircle* SliderAngle;
	void Reset(bool _resetVisiblity=true);

private:
	
	
	
	

};



UCLASS(ClassGroup = (OMCEM), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UToolWidgetProgress : public UOmWidgetComp
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void SetStyle(FString _styleName);
	void SetTitle(FString _txt);
	 UPROPERTY(BlueprintReadWrite, Category = OMCEM) UOmWidgetSlider* Slider;
	 UPROPERTY(EditAnywhere, Category = "OM3") TMap<FString, TSubclassOf<UUIBase> > Styles;
	void Reset(bool _resetVisiblity= true);

};

