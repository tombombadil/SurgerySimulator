// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "OmEngine/Ui/OmWidgetSlider.h"
#include "OmWidgetSliderCircle.generated.h"


UCLASS()
class OMCEM_API UOmWidgetSliderCircle : public UOmWidgetSlider
{
	GENERATED_BODY()
	
	
public:

	
	void Init() override;
	void SetRatio(float _ratio) override;
	void SetData(float _start, float _end, int _steps, bool _IsdoubleDirection, float _defaultRatio = 0) override;
private:
	UCanvasPanel* cArrow;
	UTextBlock* txtAngle;
};
