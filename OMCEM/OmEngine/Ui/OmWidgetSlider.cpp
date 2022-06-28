#include "OmWidgetSlider.h"
#include "OMCEM.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Utils/OmValue.h"
#include "UIBase.h"




void UOmWidgetSlider::Init()
{
	UUIBase::Init();
	sliderWidget = (USlider*)GetWidgetFromName("Slider");
	//sliderWidget->OnValueChanged.AddDynamic(this, &UOmWidgetSlider::OnEventValueChanged);
	//sliderWidget->OnMouseCaptureBegin.AddDynamic(this, &UOmWidgetSlider::OnEventMouseCapBegin);
	//sliderWidget->OnMouseCaptureEnd.AddDynamic(this, &UOmWidgetSlider::OnEventMouseCapEnd);

	
}

void UOmWidgetSlider::SetView(bool _status)
{
	if (_status)
		SetVisibility(ESlateVisibility::Visible);
	else
		SetVisibility(ESlateVisibility::Hidden);
}

void UOmWidgetSlider::SetRatio(float _ratio)
{
	if (sliderWidget)
	{
		sliderWidget->SetValue(_ratio);
	}
	

	
		if (arrValues.IsValidIndex(1))
		{

			float value = arrValues[0] + ((arrValues.Last() - arrValues[0]) * _ratio);
			if (validRatioMin != validRatioMax)
			{
				if (_ratio >= validRatioMin && _ratio <= validRatioMax)
					BpUpdateErrorValue(false);
				else
					BpUpdateErrorValue(true);
			}
			else if (validValueMin != validValueMax)
			{
				if (value >= validValueMin && value <= validValueMax)
					BpUpdateErrorValue(false);
				else
					BpUpdateErrorValue(true);
			}

		}
		
		ratio = _ratio;
}


void UOmWidgetSlider::SetSliderData(FString _omValueData, float _displayUnitScale, bool _isInteger)
{
	DisplayUnitScale = _displayUnitScale;
	FOmValue omValue = FOmValue(_omValueData);
	SetView(false); //_slider->Reset();
	FOmCharList _labels = omValue.GetValue("labels");
	FOmCharList _steps = omValue.GetValue("steps");
	validValueMin = omValue.GetValue("valueMin").GetFloat(0);
	validValueMax = omValue.GetValue("valueMax").GetFloat(0);
	validRatioMin = omValue.GetValue("ratioMin").GetFloat(0);
	validRatioMax = omValue.GetValue("ratioMax").GetFloat(0);
	
	bool _isDouble = omValue.GetValue("isDouble").GetBool(0);
	ratio = 0;
	int steps = 1;
	float from = 0;
	float to = 0;
	if (_labels.IsValid)
	{
		from = _labels.GetFloat(0);
		to = _labels.GetFloat(1);
	}
	if (_steps.IsValid)
	{
		steps = _steps.GetInt(0);
	}
	if (from != to)
	{
		if (_isInteger)
		{
			SetData((int)from, (int)to, steps, _isDouble);
		}
		else
		{
			SetData(from, to, steps, _isDouble);
		}
			
		SetView(true);


	}

	//BpUpdateKeyAngleValues(from, to, _keyMin, _keyMax);

}



void UOmWidgetSlider::SetData(float _start, float _end, int _steps, bool _IsdoubleDirection, float _defaultRatio) //#task : bunu daha sonra private yap, ilk harf kucult
{
	
	//int8 _total = FMath::FloorToInt(FMath::Abs(_end - _start) / _steps) + 1;
	int8 _total = FMath::Max<int>(_steps, 2) ;

	float start = _start;
	float end = _end;
	arrValues.Reset(0);
	if (_total > 0)
	{
		if (_IsdoubleDirection)
		{
			for (int8 i = 0; i < _total-1; i++)
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
			float _ratio = (float) i / (float) (_total-1);
			float _point = _ratio * (end - start);
			float _value = start + _point;
			arrValues.Add(_value);
		}
		Reset();
		setLabels(arrValues);
	}
	
}


void UOmWidgetSlider::setLabels(TArray<float> _labels) //to-do : setsize kýsýmlarýný her seferinde deðil Init()'de 1 kez yap 
{
	
	if (!ClassTextItem) return;
	int totalStep = _labels.Num();
	
	FVector2D widgetSize = Cast<UCanvasPanelSlot>(Slot)->GetSize();
	USlider* slider = (USlider*)GetWidgetFromName("Slider");
	
	UGridSlot* slotSlider = Cast<UGridSlot>(slider->Slot);
	//UPanelWidget* canvasTextItems = (UPanelWidget*)GetWidgetFromName("CanvasTextItems");
	UCanvasPanel* canvasTextItems = (UCanvasPanel*)GetWidgetFromName("CanvasItems");
	

	for (int8 i = 0; i < arrTextItems.Num(); i++)
	{
		arrTextItems[i]->SetVisibility(ESlateVisibility::Hidden);
		canvasTextItems->RemoveChild(arrTextItems[i]);
	}

	for (int8 i = 0; i < totalStep; i++)
	{
		UUIBase* textItem = 0;

		if (i < arrTextItems.Num())
		{
			textItem = arrTextItems[i];
			textItem->SetVisibility(ESlateVisibility::Visible);
		}
		else {
			textItem = WidgetTree->ConstructWidget<UUIBase>(ClassTextItem);

			arrTextItems.Add(textItem);
		}
		canvasTextItems->AddChild(textItem);



		{//set texts

			UTextBlock* textNumber = (UTextBlock*)textItem->GetWidgetFromName("Number");
			UTextBlock* line = (UTextBlock*)textItem->GetWidgetFromName("Line");
			//FString _label = FString::SanitizeFloat(_labels[i]);
			FString _label = UOmUtils::FloatToString(_labels[i], 1);
			TArray<FString> _parseLabel = UOmUtils::ParseString(_label, ",");
			if (_parseLabel.Num() > 1)
			{
				FString decimal = _parseLabel[1];
				if (decimal.Mid(0, 1) == "0")
					_label = _parseLabel[0];
			}


			textNumber->SetText(FText::FromString(_label));
			FString strLine = "|";
			line->SetText(FText::FromString(strLine));
		}


		//textItem->BpEventRun("func:(SetAlign),align:(" + align + ")");
		UCanvasPanelSlot* itemSlot = Cast<UCanvasPanelSlot>(textItem->Slot);
		UCanvasPanelSlot* parentSlot = Cast<UCanvasPanelSlot>(itemSlot->Parent->Slot);
		//itemSlot->SetAlignment(FVector2D(0.5,0));

		
		float paddingLeft = Cast<UCanvasPanelSlot>(canvasTextItems->Slot)->GetOffsets().Left;
		float paddingRight = Cast<UCanvasPanelSlot>(canvasTextItems->Slot)->GetOffsets().Right;
		float x = ((float)i / (float)(totalStep - 1)) * (float)(Size.X - paddingLeft - paddingRight);
		

		itemSlot->SetAlignment(FVector2D(0.5, 0));
		itemSlot->SetPosition(FVector2D(x, 0));


		if (textItem)
		{
			FString isFirst = (i == 0) ? "1" : "0";
			FString isLast = (i == totalStep-1) ? "1" : "0";
			textItem->BpEventRun("func:(Update),index:(" + FString::FromInt(i) + "),isFirst:(" + isFirst +"),isLast:(" + isLast + ")");
		}
	}
	

}
void UOmWidgetSlider::Reset()
{
	SetRatio(DefaultValue);
}




float UOmWidgetSlider::GetRatio()
{
	return ratio;
}


void UOmWidgetSlider::SetValue(float _value)
{
	if (arrValues.IsValidIndex(0))
	{
		
		float _ratio = (_value - arrValues[0]) / ((arrValues[arrValues.Num() - 1] - arrValues[0]));
		SetRatio(_ratio);
	}
	
}


float UOmWidgetSlider::GetValue()
{
	return value;
}



FString UOmWidgetSlider::ToString()
{
	return "ratio: " + FString::SanitizeFloat(ratio) + " / value: " + FString::SanitizeFloat(value);
}

