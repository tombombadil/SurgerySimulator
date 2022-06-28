#include "SpineConnector.h"
#include "JsonObjectConverter.h"
#include "OMCEM.h"


FSpineData USpineConnector::LoadSpine()
{
	FSpineData data;
	FJsonObjectConverter::JsonObjectToUStruct<FSpineData>(JsonBase.ToSharedRef(), &data);
	return data;
}

void USpineConnector::SaveSpine(FSpineData Data)
{
	//FSpineData data;
	//return data;


	//for(FSpineHoleData data : ArrHoleDatas)
	//{
	//	GConfig->setar

	//		GConfig->SetString(TEXT("Config"), TEXT("IdBone"), *data.IdBone, ACoreGameCont::PATH_CONFIG);
	//		GConfig->SetString(TEXT("Config"), TEXT("IdDirection"), "R", ACoreGameCont::PATH_CONFIG);
	//		GConfig->SetRotator(TEXT("Config"), TEXT("Bone1Rot"), bone1Rot, ACoreGameCont::PATH_CONFIG);
	//		{ /* bone rotations */
	//		
	//			FRotator bone1Rot = GetBoneRotateValue("ITextBone1");
	//			FRotator bone2Rot = GetBoneRotateValue("ITextBone2");
	//			FRotator bone3Rot = GetBoneRotateValue("ITextBone3");
	//			GConfig->SetRotator(TEXT("Config"), TEXT("Bone1Rot"), bone1Rot, ACoreGameCont::PATH_CONFIG);
	//			GConfig->SetRotator(TEXT("Config"), TEXT("Bone2Rot"), bone2Rot, ACoreGameCont::PATH_CONFIG);
	//			GConfig->SetRotator(TEXT("Config"), TEXT("Bone3Rot"), bone3Rot, ACoreGameCont::PATH_CONFIG);
	//		}


	//		GConfig->Flush(true);
	//		Debug("Flush selectedTask " + selectedTask);
	//		Debug("Flush selectedLevelMode " + selectedLevelMode);

	//		

	//}


	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject<FSpineData>(Data);
	//JsonObject->Values
	//TSharedPtr<FJsonValue> jvalue;
	//FJsonObjectConverter::UStructToJsonAttributes(Data, *Data, jvalue,1,0);

	JsonBase->SetObjectField("Operation", JsonObject);

	//JsonRoot->SetStringField("Version", "Super Sword");
	//JsonObject->SetNumberField("Damage", 15);
	//JsonObject->SetNumberField("Weight", 3);


	FString fileName = PATH_JSON_CONNECTOR;

	FString SaveGameStringData;
	TSharedRef< TJsonWriter<> > JsonWriter = TJsonWriterFactory<>::Create(&SaveGameStringData);
	FJsonSerializer::Serialize(JsonBase.ToSharedRef(), JsonWriter);
	FFileHelper::SaveStringToFile(*SaveGameStringData, *fileName);


	//FString OutputString;
	//TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	//FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	//TSharedRef< TJsonWriter<TCHAR, TPrettyJsonPrintPolicy > > JsonWriter = TJsonWriterFactory<tchar, tprettyjsonprintpolicy = "">::Create(&OutputString)
}

