#pragma once
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Inputs/Inputs.h"
#include "ToolsCont.generated.h"

class AToolItem;
class UProcessComp;


USTRUCT()
struct OMCEM_API FDataToolClasses
{
	GENERATED_USTRUCT_BODY()
//public:
	FString Id;
	FString Label;


};

UCLASS()
class OMCEM_API UToolsCont : public UObject
{
	GENERATED_BODY()
public:


	FVector ToolStartLocation;
	static UToolsCont* INS;
	bool AttachActor(AItem* _item, FString _parentViewId, FAttachmentTransformRules& _attachRules, FOmValue _params);
	bool DetachActor(AItem* _item, FDetachmentTransformRules& _detachRules);
	UToolsCont();
	void Init();
	void Render(float deltaTime);
	
	AToolItem* GetCurrTool() { 		return currTool; 	};
	TArray<AToolItem*> GetToolItems() { 		return listToolItems; 	};
	
	void SetHightlight(bool _status, FString _id="");
	void HoldItem(AToolItem* _tool, AItem* _item, bool _setActiveTool = true);
	AItem* ReleaseItem(AToolItem* _tool, bool _disableHeldItem);
	void ReleaseAndConnectHeldItem(AToolItem* _tool, FOmValue _ov);
	void PutToToolset(AToolItem* _toolItem);
	//bool ConnectItems(bool _isConnect, AItem* _parent, AItem* _child, FOmValue _params);
	bool ConnectItems(FString _idChild, FString _idParent, FOmValue _params);
	bool UnConnectItems(FString _idChild, FOmValue _params);
	//void AddCloneItem(AItem* _item, AItem* _target, FString _newName);
	void RemoveItem(FString _cloneName);
private:
	float axisX;
	float axisY;
	AToolItem* currTool;
	UPROPERTY() FVector currToolItemLoc;
	TArray<AToolItem*> listToolItems;
	//void setTool(AToolItem* _item, bool _putCurrToolToToolset = true);
	void setTool(AToolItem* _item, EInputSource _hand);
};
