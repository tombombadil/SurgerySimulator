// Copyright 2015 - 2020 SharpSurgeon 
#include "Patient.h"
#include "OMCEM.h"


void APatient::Init()
{
	Super::Init();
	TArray<TSharedPtr<FJsonValue>> checkPointsJsonArr = UResource::INS->GetJsonArray(0,"CheckPoints"); //first param zero beacuse the array in root node
	TArray<FCheckPoint> arrData;
	for (int8 i = 0; i< checkPointsJsonArr.Num(); i++)
	{

		TSharedPtr<FJsonObject> itemobj = checkPointsJsonArr[i]->AsObject();
		FCheckPoint data;

		data.Id = UResource::INS->GetJsonInt(itemobj, "Id");
		data.Title = UResource::INS->GetJsonString(itemobj, "Title");
		arrData.Add(data);
	}

	TArray<AActor*> allChildActors ;
	GetAttachedActors(allChildActors);
	for (AActor* actor : allChildActors)
	{
		ACheckPointItem* item = Cast<ACheckPointItem>(actor);
		if (item)
		{
			ListCheckPoints.Add(item);
			if (item->OrderIndex < arrData.Num())
			{
				item->InitItem(arrData[item->OrderIndex]);
			}
		}
			
	};

}
