#include "BleedingCont.h"
#include "OMCEM.h"
#include "OmEngine/Utils/Resource.h"
#include "MainCont.h"
#include "OmEngine/Components/BleedingComp.h"
#include "UICont.h"




UBleedingCont* UBleedingCont::INS = NULL;

/* FString -> { comp:(), min:(), max:() } */
void UBleedingCont::Init(TArray<FString> itemsInitData)
{
	UBleedingCont::INS = this;


	if (itemsInitData.Num() > 0)
	{
		int _time = 0;
		for (FString strOmValue : itemsInitData)
		{
			FOmValue ov = FOmValue(strOmValue);
			if (ov.IsValid)
			{
				FBleedingItem item;
				FString idComp = ov.GetValue("comp").GetString();
				UBleedingComp* comp = Cast<UBleedingComp>(UResource::INS->GetOmComp(idComp, false));
				if (comp)
				{
					item.comp = comp;
					int minRand = ov.GetValue("min").GetInt(0);
					int maxRand = ov.GetValue("max").GetInt(0);
					int interval = FMath::RandRange(minRand, maxRand);
					_time += interval;
					item.time = _time;
					ListItems.Add(item);
				}
				

			}
		}
	}
	timer = 0;
	currIndex = 0;
	currLevelIndex = 0;

}



void UBleedingCont::Render(float DeltaTime)
{
	if (!AMainCont::INS->IsOperationStarted) return;

	float itemPerValue = AMainCont::INS->BleedingPerBloodValue;
	
	
	for (FBleedingItem item : ListItems)
	{
		if (item.comp->IsBleeding)
			totalBlood += itemPerValue;
	}



	if (currLevelIndex < AMainCont::INS->BleedingLevelTimes.Num())
	{
		float currLevel = AMainCont::INS->BleedingLevelTimes[currLevelIndex];
		if (totalBlood >= currLevel)
		{
			setLevel(currLevelIndex + 1);
		}
	}



	if (currIndex <ListItems.Num())
	{
		FBleedingItem currItem = ListItems[currIndex];
		if (timer >= currItem.time)
		{
			setItem(currIndex + 1);
		}
	}

	timer += DeltaTime;
		
}


void UBleedingCont::setItem(int8 _index)
{

	if (_index < ListItems.Num())
	{
		currIndex = _index;
		FBleedingItem currItem = ListItems[currIndex];
		if (currItem.comp)
		{
			currItem.comp->StartBleeding();
		}
	}
	
}

void UBleedingCont::setLevel(int8 _index)
{
	if (_index < AMainCont::INS->BleedingLevelTimes.Num())
	{
		currLevelIndex = _index;
		AUICont::INS->ShowMessage(UConfig::GET_MSG("err_blood_level_1"), 0, 5);
	}
	
	
	
}