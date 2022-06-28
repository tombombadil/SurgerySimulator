#include "OmWidgetSliderCircle.h"
#include "OMCEM.h"





void UOmWidgetSliderCircle::Init()
{
	UUIBase::Init();
	
	cArrow = (UCanvasPanel*)GetWidgetFromName("cArrow");
	txtAngle = (UTextBlock*) GetWidgetFromName("TxtAngle");
	

}



void UOmWidgetSliderCircle::SetRatio(float _ratio)
{

	float _angle = (-90) + (_ratio * 180);
	cArrow->SetRenderTransformAngle(_angle);
	int8 viewAngle = 0;
	if (arrValues.IsValidIndex(0))
		viewAngle = FMath::FloorToInt( arrValues[0] + (_ratio * (arrValues[arrValues.Num() - 1] - arrValues[0])));
	txtAngle->SetText(FText::FromString(FString::FromInt(viewAngle * DisplayUnitScale)));

	Super::SetRatio(_ratio);
}

void UOmWidgetSliderCircle::SetData(float _start, float _end, int _steps, bool _IsdoubleDirection, float _defaultRatio /*= 0*/)
{
	//int8 _total = FMath::FloorToInt(FMath::Abs(_end - _start) / _steps) + 1;
	int8 _total =  FMath::FloorToInt(_end -  _start);
	float start = (float)_start;
	float end = (float)_end;
	arrValues.Reset(0);
	if (_total > 0)
	{
		if (_IsdoubleDirection)
		{
			for (int8 i = 0; i < _total - 1; i++)
			{
				float _index = (_total - i - 1);
				float _ratio = (float)_index / (float)(_total - 1);
				float _point = _ratio * (end - start);
				float _value = start - _point;
				arrValues.Add(_value);
			}
		}

		for (int8 i = 0; i < _total; i++)
		{
			float _ratio = (float)i / (float)(_total - 1);
			float _point = _ratio * (end - start);
			float _value = start + _point;
			arrValues.Add(_value);
		}
		Reset();
	}
}
