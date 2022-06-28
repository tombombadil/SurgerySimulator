#include "Server.h"
#include "OMCEM.h"
#include "CoreServer.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Controllers/TaskCont.h"
#include "../../SPINE/Controllers/SPINECont.h"


void UServer::Init() {
	Http = &FHttpModule::Get();
	

}




UServer* UServer::INS = NULL;





void UServer::SendMetric(FMetricJson _metric)
{

	if (!AMainCont::INS->IsEnableMetric) return;
	_metric.Json->SetStringField("deviceid", FGenericPlatformMisc::GetMacAddressString().ToLower());
	_metric.Json->SetStringField("userid", "SharpSurgeonDemo");
	_metric.Json->SetStringField("operation", ASPINECont::INS->SpineData.NameOperation);
	_metric.Json->SetStringField("operation_id", ASPINECont::INS->SpineData.IdOperation);
	_metric.Json->SetStringField("version", AMainCont::INS->Version);
	_metric.Json->SetStringField("operation_sessionid", UTaskCont::INS->IdOperation);
	_metric.Json->SetStringField("section", ASPINECont::INS->SpineData.NameSection);
	_metric.Json->SetStringField("section_id", ASPINECont::INS->SpineData.IdSection);
	_metric.Json->SetStringField("user_token", AMainCont::INS->LoginToken);
	_metric.Json->SetStringField("client_time", FDateTime::Now().ToString());

	TSharedRef< FJsonValueObject > _jsonValue = MakeShareable(new FJsonValueObject(_metric.Json));
	listAllMetrics2.Add(_jsonValue);

	TSharedPtr<FJsonObject> jsonAll = MakeShareable(new FJsonObject);
	jsonAll->SetArrayField("data", listAllMetrics2);



	FString fileName = getOperationDataPath() + "metrics.txt";

	FString SaveGameStringData;
	TSharedRef< TJsonWriter<> > JsonWriter = TJsonWriterFactory<>::Create(&SaveGameStringData);
	FJsonSerializer::Serialize(jsonAll.ToSharedRef(), JsonWriter);

		FFileHelper::SaveStringToFile(*SaveGameStringData, *fileName);
		//FFileHelper::SaveStringToFile(*allMetrics.data, *fileName);



}








FString UServer::getOperationDataPath()
{
	return FPaths::ProjectDir() + "Content/Data/" + FGenericPlatformMisc::GetMacAddressString().ToLower() + "_" + UTaskCont::INS->IdOperation + "/";
}
















/****************************************************************************/
/***************************************************************************/
/********************************[ CORE ]**********************************/
//
//void UServer::sendMetric(FMetric reqMetric) {
//	FString ContentJsonString;
//	getJsonStringFromStruct<FMetric>(reqMetric, ContentJsonString); //quest icin kapatildi
//
//	TSharedRef<IHttpRequest> Request = postRequest("event/save", ContentJsonString);
//	Request->OnProcessRequestComplete().BindUObject(this, &UServer::sendMetricResponse);
//	sendRequest(Request);
//
//	Debug("ContentJsonString" + ContentJsonString);
//
//}
//
//void UServer::sendMetricResponse(FHttpRequestPtr Request, FHttpResponsePtr httpResponse, bool bWasSuccessful) {
//	if (!sesponseIsValid(httpResponse, bWasSuccessful)) return;
//
//	FMetric_Response response;
//	getStructFromJsonString<FMetric_Response>(httpResponse, response);
//	OnMetricResponse.Broadcast(response);
//}
//
//
//
//void UServer::sendAllMetrics(FAllMetrics allMetrics)
//{
//
//	FString ContentJsonString;
//
//	getJsonStringFromStruct<FAllMetrics>(allMetrics, ContentJsonString);  //quest icin kapatildi
//
//	TSharedRef<IHttpRequest> Request = postRequest("surgery/save", ContentJsonString);
//
//
//	Request->OnProcessRequestComplete().BindUObject(this, &UServer::sendAllMetricsResponse);
//	sendRequest(Request);
//
//	//
//	///*	------------------------------------BAHADIR---------------------------------------*/
//	//	Debug("allMetrics.data" + allMetrics.data);
//	//	Debug("ContentJsonString" + ContentJsonString);
//	//
//	//	pathSpineDir = FPaths::ProjectDir();
//	//
//	//	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject<FAllMetrics>(allMetrics);
//	//	FString fileName = pathSpineDir + "Content/Json/metric.txt";
//	//
//	//	FString SaveGameStringData;
//	//	TSharedRef< TJsonWriter<> > JsonWriter = TJsonWriterFactory<>::Create(&SaveGameStringData);
//	//	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);
//	//
//	//	 FFileHelper::SaveStringToFile(*SaveGameStringData, *fileName); //  **********         rnt  kodlarýnýn bulunduðu format 
//	//	//FFileHelper::SaveStringToFile(*allMetrics.data, *fileName);
//
//
//}
//
//void UServer::sendAllMetricsResponse(FHttpRequestPtr Request, FHttpResponsePtr httpResponse, bool bWasSuccessful)
//{
//	if (!sesponseIsValid(httpResponse, bWasSuccessful)) return;
//
//	FAllMetrics_Response response;
//	getStructFromJsonString<FAllMetrics_Response>(httpResponse, response);
//	OnAllMetricResponse.Broadcast(response);
//	Debug("		sendAllMetricsResponse.httpResponse  : " + httpResponse->GetContentAsString(), 1);
//}







TSharedRef<IHttpRequest> UServer::requestWithRoute(FString Subroute) {
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetURL(baseUrl + Subroute);
	setRequestHeaders(Request);
	return Request;
}

void UServer::setRequestHeaders(TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(TEXT("User-Agent"), TEXT("OMCEM-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

	Request->SetHeader(TEXT("Authorization"), "Bearer " + AMainCont::INS->LoginToken); //added for roket
}

TSharedRef<IHttpRequest> UServer::getRequest(FString Subroute) {
	TSharedRef<IHttpRequest> Request = requestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

TSharedRef<IHttpRequest> UServer::postRequest(FString Subroute, FString ContentJsonString) {
	TSharedRef<IHttpRequest> Request = requestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

void UServer::sendRequest(TSharedRef<IHttpRequest>& Request) {
	Request->ProcessRequest();
}

bool UServer::sesponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful) {
	if (!bWasSuccessful || !Response.IsValid()) return false;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode())) return true;
	else {
		//UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code: %d"), Response->GetResponseCode());
		Debug("CoreServer::Response -> ErrorCode : ", Response->GetResponseCode());
		return false;
	}
}

void UServer::setAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request) {
	Request->SetHeader(authorizationHeader, Hash);
}










const char* UServer::TOCHAR(FString _str)
{
	return TCHAR_TO_ANSI(*_str);

}














FMetricJson FMetricJson::Create()
{
	FMetricJson m;
	m.Json = MakeShareable(new FJsonObject());
	
	m.Json->SetObjectField("data", MakeShareable(new FJsonObject()));

	
	return m;
}

FMetricJson FMetricJson::CreateStartProcess(FString _processId, FString _label)
{
	FMetricJson m = FMetricJson::Create();
	m.Json->SetStringField("category", "process");
	m.Json->SetStringField("action", "start");
	m.Json->SetStringField("value", _processId);


	return m;
}

FMetricJson FMetricJson::CreateEndProcess(FString _processId, FString _label)
{
	FMetricJson m = FMetricJson::CreateStartProcess(_processId, _label);
	m.Json->SetStringField("action", "end");
	return m;
}


FMetricJson FMetricJson::CreateStartOperation()
{

	FMetricJson m = FMetricJson::Create();
	m.Json->SetStringField("category", "operaration");
	m.Json->SetStringField("action", "start");
	m.Json->SetStringField("value", "operaration_started");
	
	

	TArray< TSharedPtr<FJsonValue> > _arr;
	for (FSpineHoleData hole : ASPINECont::INS->SpineData.ListBones)
	{
		_arr.Add(MakeShareable(new FJsonValueString(hole.IdBone)));
	}

	TSharedPtr<FJsonObject> dataJson = m.Json->GetObjectField("data");
	dataJson->SetArrayField("vertabras", _arr);
	m.Json->SetObjectField("data", dataJson);

	return m;
}

FMetricJson FMetricJson::CreateEndOperation()
{

	FMetricJson m = FMetricJson::Create();
	m.Json->SetStringField("category", "operaration");
	m.Json->SetStringField("action", "end");
	m.Json->SetStringField("value", "operaration_ended");

	return m;
}

void FMetricJson::AddDataParam(FString _key, FString _value)
{
	TSharedPtr<FJsonObject> dataJson = Json->GetObjectField("data");
	dataJson->SetStringField(_key, _value);
	Json->SetObjectField("data", dataJson);
}
