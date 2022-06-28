// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "BleedingCont.generated.h"

class UBleedingComp;

USTRUCT()
struct OMCEM_API FBleedingItem
{
	GENERATED_USTRUCT_BODY()
		float time;
	UBleedingComp* comp;
	FBleedingItem() {};
};




UCLASS()
class OMCEM_API UBleedingCont : public UObject
{
	GENERATED_BODY()
public:

	 TArray<FBleedingItem> ListItems;
	 static UBleedingCont* INS;
	void Init(TArray<FString> itemsInitData);
	void Render(float DeltaTime);
	
	void setItem(int8 _index);
	void setLevel(int8 _index);
private:

		float timer;
		int8 currItemIndex;
		int8 currLevelIndex;
		int currIndex;
		float totalBlood = 0;
		float loseBloodLevel = 0;
};
