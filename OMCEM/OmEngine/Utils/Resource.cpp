#include "Resource.h"
#include "OMCEM.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Components/ProcessComp.h"
#include "OmEngine/Components/BleedingComp.h"
#include "OmEngine/Components/FxComp.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Items/FieldItem.h"
#include <JsonReader.h>
#include "Json.h"
#include "../Controllers/PlayerCont.h"

UResource * UResource::INS = NULL;



FHoleInfo::FHoleInfo(FString holeId, AToolItem* _toolEntry, AToolItem* _toolMakeHole, AToolItem* _toolProbe, AToolItem* _toolScrew, AToolItem* _screw, FString _label)
{
	id = holeId;
	label = _label;
	hole = Cast<AItem>(UResource::INS->GetItem(holeId));
	screw = _screw;
	volume = Cast<UOmComp>(UResource::INS->GetOmComp(holeId + "/Volume"));
	processEntry = Cast<UProcessComp>(UResource::INS->GetOmComp(holeId + "/Entry"));
	pawnMakeHole = Cast<UProcessComp>(UResource::INS->GetOmComp(holeId + "/MakeHole"));
	pawnProbe = Cast<UProcessComp>(UResource::INS->GetOmComp(holeId + "/Probe"));
	pawnScrew = Cast<UProcessComp>(UResource::INS->GetOmComp(holeId + "/Screw"));
	toolEntry = _toolEntry;
	toolMakeHole = _toolMakeHole;
	toolProbe = _toolProbe;
	toolScrew = _toolScrew;
}



FRodInfo::FRodInfo(FString _fieldId, FString _rodId, AToolItem* _instertTool, AToolItem* _distractTool)
{
	rodField = Cast<AItem>(UResource::INS->GetItem(_fieldId));
	vol = Cast<UOmComp>(UResource::INS->GetOmComp(_fieldId + "/Volume"));
	processSep = Cast<UProcessComp>(UResource::INS->GetOmComp(_fieldId + "/Seperate"));
	processInsert = Cast<UProcessComp>(UResource::INS->GetOmComp(_fieldId + "/Insert"));
	rod = Cast<AToolItem>(UResource::INS->GetItem(_rodId));
	instertTool = _instertTool;
	distractTool = _distractTool;
}

FNutFieldInfo::FNutFieldInfo(FString _fieldId, FString _nutId, AToolItem* _insertTool, AToolItem* _tightenTool)
{
	nutField = Cast<AItem>(UResource::INS->GetItem(_fieldId));
	vol = Cast<UOmComp>(UResource::INS->GetOmComp(_fieldId + "/Volume"));
	tighten = Cast<UProcessComp>(UResource::INS->GetOmComp(_fieldId + "/NutTighten"));
	insert = Cast<UProcessComp>(UResource::INS->GetOmComp(_fieldId + "/NutInsert"));
	nut = Cast<AToolItem>(UResource::INS->GetItem(_nutId));
	insertTool = _insertTool;
	tightenTool = _tightenTool;
}



FPartFieldInfo::FPartFieldInfo(FString _fieldId, AToolItem* _takeTool)
{
	field = Cast<AFieldItem>(UResource::INS->GetItem(_fieldId));
	vol = Cast<UOmComp>(UResource::INS->GetOmComp(_fieldId + "/Volume"));
	take = Cast<UProcessComp>(UResource::INS->GetOmComp(_fieldId + "/Take"));
	put = Cast<UProcessComp>(UResource::INS->GetOmComp(_fieldId + "/Put"));
	takeTool = _takeTool;
}

UResource::UResource()
{
	INS = this;
}

void UResource::Load()
{
	loadAndParseJSONs();
}

void UResource::Init()
{
	
	registerObjects();

	initDefinations();

	
	for (AItem* item : GetItems())
	{
			item->Init();
	}
	
	
	
	for (AItem* item : GetItems())
	{
		item->InitAfter();
	}



	

}


void UResource::initDefinations()
{
	TOOLSET = Cast<AItem>(GetItem("Toolset"));
	TOOLS = Cast<AItem>(GetItem("Tools"));
}



/* JSON */
void UResource::loadAndParseJSONs()
{
	FString pathJson = FPaths::ProjectContentDir() + "Json/";
	FString JsonString;
	jsonMain = loadJSON(pathJson +"data.txt");
	//if (AMainCont::INS->Operation.IsValid)
	//{
	 jsonOperation = loadJSON(pathJson + "operation.txt");
		//AMainCont::INS->Operation.Json = opJson;
	//}

	//eventDisp->dispatchEvent(EEventType::COMPLETE, this);
	EventOnStatus.Broadcast(EEventType::COMPLETE);
}

TSharedPtr<FJsonObject> UResource::GetOpJsonObj( FString _field)
{
	TSharedPtr<FJsonObject> result;
	//FOperation operation = AMainCont::INS->Operation;
	//TSharedPtr<FJsonObject> JsonObject = operation.Json;
	if (_field.IsValidIndex(0))
	{
		if (jsonOperation.IsValid())
		{
			result = jsonOperation->GetObjectField(_field);
		}
	}
	else
	{
		result = jsonOperation;
	}
	
	return result;
}

TSharedPtr<FJsonObject> UResource::GetMainJsonObj(FString _field)
{
	TSharedPtr<FJsonObject> result;
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	if (jsonMain.IsValid())
	{
		result = jsonMain->GetObjectField(_field);
	}
	else
	{
		result = JsonObject;
	}
	return result;
}


FVector UResource::GetJsonVector(TSharedPtr<FJsonObject> _jsonObj, FString _field)
{
	FVector result;
	FString str = GetJsonString(_jsonObj, _field);
	TArray<FString> arrStr = UOmUtils::ParseString(str, ",");
	result.X = (arrStr.IsValidIndex(0)) ? UOmUtils::ToFloat(arrStr[0]) : 0;
	result.Y = (arrStr.IsValidIndex(1)) ? UOmUtils::ToFloat(arrStr[1]) : 0;
	result.Z = (arrStr.IsValidIndex(2)) ? UOmUtils::ToFloat(arrStr[2]) : 0;
	return result;
}

FString UResource::GetJsonString(TSharedPtr<FJsonObject> _jsonObj, FString _field)
{
	FString result;
	result = _jsonObj->GetStringField(_field);
	return result;
}

int32 UResource::GetJsonInt(TSharedPtr<FJsonObject> _jsonObj, FString _field)
{
	int32 result;
	result = _jsonObj->GetIntegerField(_field);
	return result;
}

TArray<TSharedPtr<FJsonValue>>  UResource::GetJsonArray(TSharedPtr<FJsonObject> _jsonObj, FString _field)
{
	TArray<TSharedPtr<FJsonValue>> result;
	TSharedPtr<FJsonObject> jsonObj = _jsonObj.IsValid() ? _jsonObj : jsonMain;

	result = jsonObj->GetArrayField(_field);
	return result;
}


TSharedPtr<FJsonObject> UResource::loadJSON(FString _path)
{
	FString JsonString; //Json converted to FString

	FFileHelper::LoadFileToString(JsonString, *_path);

	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

	FJsonSerializer::Deserialize(JsonReader, JsonObject);
	return JsonObject;
}


void UResource::registerObjects()
{

	for (TActorIterator<AActor> actor(AMainCont::INS->GetWorld()); actor; ++actor)
	{ 
		AItem* item = Cast<AItem>(*actor);
		AToolItem* toolItem = Cast<AToolItem>(*actor);
		APlayerCont* player = Cast<APlayerCont>(*actor);

		if(item)
		{
			RegisterItem(item);


		}


		

		

		/* if (toolItem)
		{
			ListToolItems.Add(toolItem->Id, toolItem);
		}*/

		else if (player)
		{
			
				TArray<USceneComponent*> arr;
				player->GetComponents(arr);
				int8 indexOfComp = 0;
				for (USceneComponent* comp : arr)
				{

					FString idComp = player->GetName() + "/" + comp->GetName(); //generate id
					listComps.Add(idComp, comp);
					UOmComp* omComp = Cast<UOmComp>(comp);
					if (omComp)
					{
						//omComp->OwnerItem = player;
						omComp->Create(indexOfComp, idComp);
					}

					indexOfComp++;
				}
			

		}



	}


	
	
}

AItem* UResource::SpawnItem(TSubclassOf<AItem> Class, FVector Location, FRotator Rotation )
{
	FActorSpawnParameters SpawnInfo;
	AItem* newItem = AMainCont::INS->GetWorld()->SpawnActor<AItem>(Class, Location, FRotator::ZeroRotator, SpawnInfo);
	RegisterItem(newItem);
	return newItem;
}

void UResource::DestroyItem(AItem* _item)
{
	if (listItems.Contains(_item->Id))
	{
		listItems.Remove(_item->Id);
	}
	_item->PreDestroy();
	_item->Destroy();
}


UOmComp* UResource::SpawnComp(TSubclassOf<UOmComp> ComponentClass, FString CompName, USceneComponent* Parent)
{
	UOmComp* comp = 0;
	if (Parent)
	{
		AItem* owner = Cast<AItem>(Parent->GetOwner());
		//comp = CreateDefaultSubobject<UOmComp>(*CompName);
		comp = NewObject<UOmComp>(owner, ComponentClass);
		comp->AttachTo(Parent);
		//comp->AttachToComponent(Parent, FAttachmentTransformRules::);
		comp->RegisterComponent();
		//comp->SetupAttachment(Parent);
		
		if (owner) RegisterComp(comp, owner);
	}
	


	return comp;
}

void UResource::DestroyComp(UOmComp* _comp)
{
	if (listComps.Contains(_comp->Id))
	{
		listComps.Remove(_comp->Id);
	}
	_comp->PreDestroy();
	_comp->RemoveFromRoot();
	_comp->DestroyComponent();
}

void UResource::RegisterItem(AItem* _item, bool _callInitFunc)
{
	if (_item)
	{
		int index = listItems.Num();
		FString idItem = _item->Create(index);

		AddItemToList(idItem, _item,  _callInitFunc);

		if (_callInitFunc)
			_item->Init();

	}
	
}



bool  UResource::AddItemToList(FString _id, AItem* _item, bool _callInitFunc)
{
	bool result = false;
	if (!listItems.Contains(_id))
	{
		listItems.Add(_id, _item);
		//TArray<UOmComp*> arr;
		TArray<USceneComponent*> arr;
		_item->GetComponents(arr);
		for (USceneComponent* comp : arr)
		{

			RegisterComp(comp, _item);

			UChildActorComponent* childActorComp = Cast<UChildActorComponent>(comp);
			if (childActorComp)
			{
				AItem* item = Cast<AItem>(childActorComp->GetChildActor());
				if (item)
				{
					bool containtAItemInList = false;
					for (auto& i : listItems)
					{
						if (item == Cast<AItem>(i.Value))
							containtAItemInList = true;
					}
					if (!containtAItemInList)
					{
						RegisterItem(item, _callInitFunc);
						Debug("child item : " + item->GetName());
					}
					
				}
			}
			
		}
		result = true;
	}
	
	return result;
}

AItem* UResource::RemoveItemToList(FString _id)
{
	AItem* item = 0;
	if (listItems.Contains(_id))
	{
		item = listItems[_id];
		TArray<UOmComp*> arr;
		item->GetComponents(arr);
		for (UOmComp* omComp : arr)
		{
			listComps.Remove(omComp->Id);
		}
		listItems.Remove(_id);
	}
	return item;
	
}

void  UResource::RegisterComp(USceneComponent* _comp, AItem* _item)
{
	int8 indexOfComp = _item->GetRootComponent()->GetNumChildrenComponents();

	FString idComp = _item->Id + "/" + _comp->GetName(); //generate id
	listComps.Add(idComp, _comp);
	UOmComp* omComp = Cast<UOmComp>(_comp);
	if (omComp)
	{
		omComp->OwnerItem = _item;
		omComp->Create(indexOfComp, idComp);
	}


}




AItem* UResource::GetItem(FString _id, bool _fatal)
{
	AItem* r = getItemInList(_id, false);

	if(!r)//check @params
	{
		TArray<FString> getIds = GetParamFromParseId(_id);
		//for(FString i : getIds)
		if (getIds.IsValidIndex(0))
		{
			FString getId = getIds.Last();
			r = getItemInList(getId, false);
		}
	
		
	}
	

	if (_fatal && !r) Debug::Warning("The Item <" + _id + "> not found!");

	return r;
}

/* object/@objectid/@objectis stringindeki item/compenentlere gore son idye sahip objenin icindeki paremetreyi doner */
TArray<FString> UResource::GetParamFromParseId(FString _id)
{
	TArray<FString> r;
		FString runtimeParamChar = "@";
		TArray<FString> parsedList = UOmUtils::ParseString(_id, "/", true);
		AItem* lastItem = 0;
		UOmComp* lastComp = 0;
		for (FString p : parsedList)
		{
			TArray<FString> parsedParamChar = UOmUtils::ParseString(p, "@", false);
			if (parsedParamChar.Num() > 1)
			{
				FString keyParam = parsedParamChar[1];

				FString idFromKeyParam;
				if (lastItem )
				{
					idFromKeyParam = lastItem->GetParam(keyParam);
					if (idFromKeyParam.IsValidIndex(0))
					{
						lastItem = getItemInList(idFromKeyParam, false);
						if (lastItem) 
						{
							lastComp = 0;
						}
						r.Add(idFromKeyParam);
					}
					

				}
				else if (lastComp)
				{
					idFromKeyParam = lastComp->GetParam(keyParam);
					if (idFromKeyParam.IsValidIndex(0))
					{
						lastComp = Cast<UOmComp>( getCompInList(idFromKeyParam, false));
						if (lastComp)
						{
							lastItem = 0;
						}
						r.Add(idFromKeyParam);
					}


				}
			}
			else
			{
				AItem* getItemFromId = getItemInList(p, false);
				if (getItemFromId)
				{
					lastItem = getItemFromId;
				}
					
				else if(lastItem)
				{
					lastComp = lastItem->GetCompByName(p);
				}

				r.Add(p);
				
			}

		}

	return r;
}

UOmComp* UResource::GetOmComp(FString _id, bool _fatal /*= false*/)
{
	UOmComp* obj = Cast<UOmComp>(GetComp(_id, _fatal));
	return obj;
}





USceneComponent* UResource::GetComp(FString _id, bool _fatal /*= false*/)
{

	USceneComponent* r = getCompInList(_id, false);

	if (!r)//check @params
	{
		TArray<FString> getIds = GetParamFromParseId(_id);
		
		if (getIds.IsValidIndex(0))
		{
			FString getId = getIds.Last();
			r = getCompInList(getId, false);
			if (!r)
			{
				if (getIds.Num() > 1)
				{
					FString idActor = getIds[getIds.Num() - 2];
					FString nameOfComp = getIds[getIds.Num() - 1];
					FString idComp = idActor + "/" + nameOfComp;
					r = getCompInList(idComp, false);
				}
			}

			
		}
			



	}


	if (_fatal && !r) Debug::Warning("The Component <" + _id + "> not found!");

	return r;



	
	
}
USceneComponent* UResource::GetComp(AActor* _actor, FString _id)
{
	USceneComponent* comp = 0;
	if (!_actor) return 0;

	TArray<USceneComponent*> arrAll;
	_actor->GetComponents(arrAll,true);
	int8 indexOfComp = 0;
	for (USceneComponent* c : arrAll)
	{
		if (c->GetName() == _id) comp = c;
	}

	return comp;
}






UOmComp* UResource::GetOmComp(FOmValue _omValue, FString _key, bool _fatal /*= false*/)
{
	UOmComp* obj = 0;
	
	if (_omValue.GetValue(_key).IsValid)
	{
		FString id = _omValue.GetValue(_key).GetString(0);
		obj = GetOmComp(id, _fatal);
	}
	return obj;
}


TArray<AItem*> UResource::GetItems()
{
	TArray<AItem*> arr;
	for (auto& Elem : listItems)
	{
		arr.Add(Elem.Value);
	}
	return arr;
}


AItem* UResource::getItemInList(FString _id, bool _fatal)
{
	AItem* r = 0;


	if (listItems.Contains(_id)) //if its == before saved id
	{
		r = listItems[_id];
	}

	if (_fatal && !r) Debug::Warning("The Item <" + _id + "> not found in List!");

	return r;
}
USceneComponent* UResource::getCompInList(FString _id, bool _fatal)
{

	USceneComponent* comp = 0;
	if (listComps.Contains(_id))
	{
		comp = listComps[_id];

	}
	else {
		if (_fatal) Debug::Warning("The Component " + _id + " not found!");
	}
	return comp;
}


FString UResource::GenerateCompId(USceneComponent* _comp)
{
	FString id = "";

	AOmActor* ownerActor = Cast<AOmActor>(_comp->GetOwner());
	if(ownerActor)
		id = ownerActor->Id + "/"+ _comp->GetName();
	return id;
}





TArray<USceneComponent*> UResource::GetComponentsByTag(FString _tag)
{
	TArray<USceneComponent*> _result;
	for (TActorIterator<AActor> ActorItr(AMainCont::INS->GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* actor = dynamic_cast<AActor*>(*ActorItr);
		TArray<UActorComponent*> _arrActorComps = actor->GetComponentsByTag(UActorComponent::StaticClass(), FName(*_tag));
		for (UActorComponent* comp : _arrActorComps)
		{
			USceneComponent* castSceneComp = Cast<USceneComponent>(comp);
			if (castSceneComp)_result.Add(castSceneComp);
		}
	}
	return _result;
}







FDataProvider::FDataProvider(vector<float> _data)
{
	IsValid = false;
	if (_data.size() == 1)
	{
		data.Reserve(2);
		data.Init(_data[0], 2);
		data[0] = 0;
		IsValid = true;
	}
	else if (_data.size() > 1)
	{
		IsValid = true;
		for (float f : _data)
		{
			data.Add(f);
		}
	}

	value = 0;
	ratio = 0;

	data.Sort();

	
}
void FDataProvider::SetValue(float _value)
{
	if (!IsValid) return;
	value = FMath::Clamp<float>(_value, data[0], data.Last());
	ratio = (value - data[0]) / (data.Last() - data[0]);
}

float FDataProvider::GetValue()
{
	return value;
}

void FDataProvider::SetRatio(float _ratio)
{
	if (!IsValid) return;
	ratio = FMath::Clamp<float>(_ratio, 0, 1);
	value = data[0] + (ratio * (data.Last() - data[0]));
}

float FDataProvider::GetRatio()
{
	return ratio;
}

FString FDataProvider::ToString()
{
	FString s = "data(";
	if (data.IsValidIndex(0))
	{
		for (float f : data)
			s.Append(FString::SanitizeFloat(f) + ",");
	}
	s.Append("),  value(" + FString::SanitizeFloat(value) + "), ratio(" + FString::SanitizeFloat(ratio) + ")");
	return s;
}
void FDataProvider::Destroy()
{
	data.Empty(0);
	IsValid = false;
}