#pragma once

#include "OmEngine/Ui/UIBase.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Blueprint/UserWidget.h"
#include "OmWidgetSlider.generated.h"


class UUIApp;



UCLASS()
class OMCEM_API UOmWidgetSlider : public UUIBase
{
	GENERATED_BODY()

private:
	float ratio = 0;
	float value = 0;
	float validValueMin = 0;
	float validValueMax = 0;
	float validRatioMin = 0;
	float validRatioMax = 0;
	UPROPERTY() 	TArray<UUIBase*> arrTextItems;
	void setLabels(TArray<float> _labels);
protected:
	TArray<float> arrValues;
public:
	UPROPERTY() 	USlider* sliderWidget;
	UPROPERTY() 	float DefaultValue = 0;
	UPROPERTY() 	float DisplayUnitScale = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM LIBRARY") TSubclassOf<UUIBase> ClassTextItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OM3") FVector Size;
	virtual void Init() override;
	virtual void SetRatio(float _ratio);
	void SetView(bool _status);
	//UFUNCTION(BlueprintCallable, Category = "OMCEM") void SetSliderData(FString _omValueData);
	UFUNCTION(BlueprintCallable, Category = "OMCEM") void SetSliderData(FString _omValueData, float _displayUnitScale=1, bool _isInteger=true);
	virtual void SetData(float _start, float _end, int _steps, bool _IsdoubleDirection, float _defaultRatio = 0);
	float GetRatio();
	void SetValue(float _value);
	float GetValue();
	void Reset();
	FString ToString();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpUpdateErrorValue(bool IsError);
};