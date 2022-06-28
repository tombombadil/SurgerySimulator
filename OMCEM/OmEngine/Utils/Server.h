// Copyright 2015 - 2020 SharpSurgeon 

#pragma once

#include "OmEngine/Utils/CoreServer.h"
#include "OmValue.h"
#include <JsonObject.h>
#include "Server.generated.h"




//
//
//USTRUCT()
//struct FMetric 
//{
//	GENERATED_BODY()
//	UPROPERTY() FString slug;
//	UPROPERTY() FString type;
//	UPROPERTY() FString type_id;
//	UPROPERTY() FString category; /* OPERATION,PROCESS */
//	UPROPERTY() FString category_id;
//	UPROPERTY() FString action; /* START,END */
//	UPROPERTY() FString label;
//	UPROPERTY() FString value;
//	UPROPERTY() FString result;
//	UPROPERTY() FString data;
//	FOmValue DataParams; //bu daha sonra 'data' stringine cevriliyor
//	UPROPERTY()FDateTime client_time;
//	bool EnableStartAction = false;
//	bool EnableEndAction = false;
//	int8 sendActionStatus = 0; //0(none) 1(sent start) 2(send end) */
//	TSharedPtr<FJsonObject> jsonObject;
//	FMetric()
//	{
//		slug = "TLIF";
//		type = "SURGERY";
//
//
//	}
//	static FMetric Create();
//	static FMetric CreateStartOperation();
//	static FMetric CreateOperationCompleted();
//
//
//	static FMetric CreateStartProcess(FString _processId, FString _label, bool _startAction, bool _endAction);
//	static FMetric CreateEndProcess(FString _processId, FString _label);
//
//	void ConvertToStartProcess();
//
//	void ConvertToEndProcess();
//
//	void createJsonDataField();
//
//	//void flushJsonDataField();
//
//	void AddVectorField(FString _dataLabel, FVector _data);
//	void AddFloatField(FString _label, float _data);
//
//
//
//
//};




USTRUCT()
struct FMetric_Response {
	GENERATED_BODY()

		FMetric_Response() {}
};



USTRUCT()
struct FAllMetrics {
	GENERATED_BODY()
		UPROPERTY() FString data;
	FAllMetrics() {}
};


USTRUCT()
struct FAllMetrics_Response {
	GENERATED_BODY()

		FAllMetrics_Response() {}
};



UENUM(BlueprintType)
enum ECoreServerEvent
{
	START_TASK,
	END_TASK

};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMetricResponse, FMetric_Response, responseData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAllMetricResponse, FAllMetrics_Response, responseData);


USTRUCT()
struct FMetricJson 
{
	GENERATED_BODY()

		TSharedPtr<FJsonObject>  Json;
	static FMetricJson Create();
	static FMetricJson CreateStartProcess(FString _processId, FString _label);
	static FMetricJson CreateEndProcess(FString _processId, FString _label);
	static FMetricJson CreateStartOperation();

	static FMetricJson CreateEndOperation();
	void AddDataParam(FString _key, FString _value);
};



UCLASS(Blueprintable, hideCategories = (Rendering, Replication, Input, Actor, "Actor Tick"))
class OMCEM_API UServer : public UObject
{
	GENERATED_BODY()
//------------UServer
private:
	TArray<TSharedPtr<FJsonValue>> listAllMetrics2;
	//void sendMetric(FMetric reqMetric);
	//void sendLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void sendMetricResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	//void sendAllMetrics(FAllMetrics allMetrics);
	//void sendAllMetricsResponse(FHttpRequestPtr Request, FHttpResponsePtr httpResponse, bool bWasSuccessful);
	FString getOperationDataPath();
public:

	static UServer* INS;
	//void SendMetric(FMetric _metric);
	void SendMetric(FMetricJson _metric);
	//void SendAllMetrics();

	void Init();
	UPROPERTY() FMetricResponse OnMetricResponse;
	UPROPERTY() FAllMetricResponse OnAllMetricResponse;

	static const char* TOCHAR(FString _str);







//------------CORE
private:


	FString baseUrl = "http://sharpsurgeon.roket.ws/api/";
	//void sendLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/* api */
	FHttpModule* Http;
	FString authorizationHeader = TEXT("Authorization");
	void setAuthorizationHash(FString Hash, TSharedRef<IHttpRequest>& Request);
	TSharedRef<IHttpRequest> requestWithRoute(FString Subroute);
	 void setRequestHeaders(TSharedRef<IHttpRequest>& Request);
	TSharedRef<IHttpRequest> getRequest(FString Subroute);
	TSharedRef<IHttpRequest> postRequest(FString Subroute, FString ContentJsonString);
	void sendRequest(TSharedRef<IHttpRequest>& Request);
	bool sesponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);


};
