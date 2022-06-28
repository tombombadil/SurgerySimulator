#pragma once

#include <vector>
#include "OmEngine/Utils/OmStructs.h"
#include <JsonValue.h>
#include "Resource.generated.h"

class UConfig;
class UResource;
class AItem;
class UUIApp;
class AToolItem;
class AFieldItem;
class UOmComp;
class UProcessComp;
class UMoveComp;
class UMoveAgent;
class UOmUtils;
struct FOmValue;

/*_____________________ Resource _____________________ */
UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class  OMCEM_API UResource : public UObject
{
	GENERATED_BODY()
public:
	FEventOnStatus EventOnStatus;
	UResource();
	void Load();
	
	static UResource* INS;
	UPROPERTY() AItem* TOOLSET; 
	UPROPERTY() AItem* TOOLS; //TOOL HOLDER
	//UPROPERTY() AItem* ROOM;
	//UPROPERTY() UMoveAgent* MOVE_AGENT; //NEW MOVE AGENT
	UPROPERTY() AItem* DEBUG_HAPTIC; 
	//UPROPERTY() AItem* ITEM_XRAY;
	
	/* Classes */
	/*________________ Classes ________________ */
	//UPROPERTY() UMaterial* MAT_BONEXRAY;
	//UPROPERTY() UMaterial* MAT_TOOLXRAY;
	//UPROPERTY() UMaterial* MAT_NERVOUS;
	//UPROPERTY() UMaterial* MAT_HIGHLIGHTTOOL;

	void Init();

	UFUNCTION(BlueprintCallable) USceneComponent* GetComp(FString _id, bool _fatal = false);
	USceneComponent* GetComp(AActor* _actor, FString _id);
	UFUNCTION(BlueprintCallable) UOmComp* GetOmComp(FString _id, bool _fatal = false);
	UOmComp* GetOmComp(FOmValue _omValue, FString _key, bool _fatal = false);
	UFUNCTION(BlueprintCallable) AItem* GetItem(FString _id, bool _fatal = true);
	void RegisterItem(AItem* _item, bool _callInitFunc=false);
	TArray<FString> GetParamFromParseId(FString _id);
	TArray<AItem*> GetItems();
	UFUNCTION(BlueprintCallable)  FString GenerateCompId(USceneComponent* _comp);
	UFUNCTION(BlueprintCallable, Category="OM3") AItem* SpawnItem(TSubclassOf<AItem> Class, FVector Location, FRotator Rotation);
	UFUNCTION(BlueprintCallable, Category = "OM3")void DestroyItem(AItem* _item);
	UFUNCTION(BlueprintCallable, Category = "OM3") UOmComp* SpawnComp(TSubclassOf<UOmComp> Class, FString CompName, USceneComponent* Parent);
	UFUNCTION(BlueprintCallable, Category = "OM3")void DestroyComp(UOmComp* _comp);
	bool AddItemToList(FString _id, AItem* _item, bool _callInitFunc=false);
	AItem* RemoveItemToList(FString _id);
	void RegisterComp(USceneComponent* _comp, AItem* _item);
	TArray<USceneComponent*> GetComponentsByTag(FString _tag);
	
	/* json */

	TSharedPtr<FJsonObject> GetOpJsonObj(FString _field);
	TSharedPtr<FJsonObject> GetMainJsonObj(FString _field);
	FVector GetJsonVector( TSharedPtr<FJsonObject> _jsonObj, FString _field);
	FString GetJsonString( TSharedPtr<FJsonObject> _jsonObj, FString _field);
	int32 GetJsonInt( TSharedPtr<FJsonObject> _jsonObj, FString _field);
	TArray<TSharedPtr<FJsonValue>> GetJsonArray( TSharedPtr<FJsonObject> _jsonObj, FString _field);
	//UPROPERTY() TMap<FString, AToolItem*> ListToolItems;
protected:
	UPROPERTY() TMap<FString, AItem*> listItems;
	UPROPERTY() TMap<FString, USceneComponent*> listComps;
	void registerObjects();

	
	void loadAndParseJSONs();

	virtual void initDefinations();
	/* json */
	TSharedPtr<FJsonObject> loadJSON(FString _path);
	TSharedPtr<FJsonObject> jsonMain;
	TSharedPtr<FJsonObject> jsonOperation;

private:
	AItem* getItemInList(FString _id, bool _fatal);
	USceneComponent* getCompInList(FString _id, bool _fatal);
	
};









