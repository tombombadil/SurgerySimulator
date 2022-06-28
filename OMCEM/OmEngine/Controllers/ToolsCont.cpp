#include "ToolsCont.h"
#include "OMCEM.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Items/ToolItem.h"
#include "OmEngine/Components/OmComp.h"
#include "Observer.h"
#include "OmEngine/Utils/Resource.h"
#include "PlayerCont.h"


UToolsCont* UToolsCont::INS = NULL;











UToolsCont::UToolsCont()
{
	UToolsCont::INS = this;
}

void UToolsCont::Init()
{

	TArray<TSharedPtr<FJsonValue>> toolClassesArr = UResource::INS->GetJsonArray(0, "ToolClasses"); //first param zero beacuse the array in root node
	TArray<FDataToolClasses> arrData;
	for (int8 i = 0; i < toolClassesArr.Num(); i++)
	{

		TSharedPtr<FJsonObject> itemobj = toolClassesArr[i]->AsObject();
		FDataToolClasses data;

		data.Id = UResource::INS->GetJsonString(itemobj, "Id");
		data.Label = UResource::INS->GetJsonString(itemobj, "Label");
		arrData.Add(data);
	}


		for (AItem* i : UResource::INS->GetItems())
	{
		AToolItem* item = Cast< AToolItem >(i);
		if (item)
		{
			if (item->IdClass.IsEmpty() && item->IsEnable)
			{
				Debug::Warning("IdClass bos olamaz! item : " + item->GetName() + " item->IsEnable : " +  FString::FromInt((int)item->IsEnable));
			}
			else
			{
				bool foundResult = false;
				for (FDataToolClasses data : arrData)
				{
					if (item->IdClass == data.Id)
					{
						foundResult = true;
						item->Label = data.Label;
					}
				}
				AItem* parent = Cast<AItem>(item);
				if (parent && !foundResult)
				{
					if(parent->Id == "Toolset")
						Debug::Warning("Bu tool'un label'i json'a girilmemis! item : " + item->GetName());
				}
				
			}
		}

	}


	
}



void UToolsCont::Render(float deltaTime)
{

}

bool UToolsCont::AttachActor(AItem* _item, FString _parentViewId, FAttachmentTransformRules& _attachRules, FOmValue _params)
{
	bool r = 0;
	USceneComponent* compParent = 0;
	AItem* parentItem = UResource::INS->GetItem(_parentViewId, false);
	USceneComponent* parentComp = UResource::INS->GetComp(_parentViewId, false);
	FString insertKeyName = "Insert" + _item->IdCat;
	if (parentItem)
	{
		compParent = parentItem->GetRootComponent();
		{//added insert key
			parentItem->UpdateParams(insertKeyName + ":(" + _item->Id + ")");
		}
	
	}
	else if (parentComp)
	{
		compParent = parentComp;
		{//added insert key
			AItem* owner = Cast<AItem>(compParent->GetOwner());
			owner->UpdateParams(insertKeyName + ":(" + _item->Id + ")");
			if (Cast<UOmComp>(compParent))
			{
				Cast<UOmComp>(compParent)->UpdateParams(insertKeyName + ":(" + _item->Id + ")");
			}
			
		}

	}

	if (compParent)
	{
		_item->AttachToComponent(compParent, _attachRules);
		r = true;

		Debug("AttachActor  _item : " + _item->Id + " ----> parent comp : " + compParent->GetName() + "  @_params: " + _params.ToString());

		{//connectpoint
			FString connectPointName = _params.GetValue("ConnectPoint").IsValid ? _params.GetValue("ConnectPoint").GetString() : "ConnectPoint";
			_item->SetItemLoc(_item->RootComponent->GetRelativeTransform().GetLocation() , false, connectPointName);

			
		}
		
	}
	return r;

}
bool UToolsCont::DetachActor(AItem* _item, FDetachmentTransformRules& _detachRules)
{
	bool r = 1;
	if (_item)
	{
		_item->DetachFromActor(_detachRules);
	}
	return r;

}




bool UToolsCont::ConnectItems( FString _idChild,  FString _idParent, FOmValue _params)
{
	AItem* child = UResource::INS->GetItem(_idChild, false);
	if (!child) return false;

	AToolItem* castTool = Cast<AToolItem>(child);
	if (castTool)
	{
		AToolItem* parent = Cast< AToolItem>(UResource::INS->GetItem(_idParent, false));
		castTool->SetSelectable(false);
		if (APlayerCont::INS->MoveR->CurrPart == castTool && APlayerCont::INS->MoveR->CurrTool != parent) //todo sadece moveR icin gecerli bu satir
			APlayerCont::INS->PutTool("MoveR", APlayerCont::INS->MoveR->CurrPart->RuleOfPutToToolset); //release curr item, take a empty tool

	//AItem* castParentItem = UResource::INS->GetItem(_idParent, false);

	}



		bool isSnapTarget = _params.GetValue("IsSnapTarget").GetBool();
		FAttachmentTransformRules attachRule = isSnapTarget ? FAttachmentTransformRules::SnapToTargetNotIncludingScale:FAttachmentTransformRules::KeepWorldTransform;
		//_child->OwnerItem = this;
		//_child->SetVisible(true, true);
		//if (!listConnectedItems.Contains(_child)) listConnectedItems.Add(_child);
		AttachActor(child, _idParent, attachRule, _params);

		
		

		
	
		
	
		AToolItem* castEq = Cast<AToolItem>(child);
		if (castEq)
		{
			bool isHideEqAfterProcess = FOmValue(_params).GetValue("hidePartAfter").GetBool();
			//bool isDisableEq = FOmValue(_params).GetValue("disablPart").GetBool();
			if (isHideEqAfterProcess)
			{
				castEq->SetVisible(false, true);
				castEq->SetEnable(false);
			}
		}


	return true;
}


bool UToolsCont::UnConnectItems(FString _idChild, FOmValue _params)
{
	AItem* child = UResource::INS->GetItem(_idChild, false);
	//bool isSnapTarget = _params.GetValue("IsSnapTarget").GetBool();
	FDetachmentTransformRules detachRule = FDetachmentTransformRules::KeepWorldTransform;
	//_child->OwnerItem = this;
	//_child->SetVisible(true, true);
	//if (!listConnectedItems.Contains(_child)) listConnectedItems.Add(_child);
	DetachActor(child,  detachRule);

	AToolItem* castTool = Cast<AToolItem>(child);
	if (castTool)
	{
		castTool->SetSelectable(true);
	}



	return true;
}

/* Bir tool'un herhangi bir Item'ı bir collision tetiklemeden direkt olarak tutması */
void UToolsCont::HoldItem(AToolItem* _tool, AItem* _item, bool _setActiveTool)
{
	if (_tool && _item)
	{
		if (_setActiveTool) APlayerCont::INS->TakeTool("MoveR", _tool);
		//currTool->HoldItem(_item);
		_tool->HoldItem(_item);
		_tool->UpdateParams(FOmValue("isHold:(1)"));
	}
}


AItem* UToolsCont::ReleaseItem(AToolItem* _tool, bool _disableChild)
{
	AItem* heldItem = 0;
	if (_tool)
	{
		heldItem = _tool->ReleaseItem(_disableChild);
		_tool->UpdateParams(FOmValue("isHold:(0)"));
	}
	return heldItem;
}

void UToolsCont::ReleaseAndConnectHeldItem(AToolItem* _tool, FOmValue _ov)
{
	if (_tool)
	{
		AItem* currHeldItem = _tool->ReleaseItem(true);
		AItem* parent = Cast<AItem>(UResource::INS->GetItem(_ov.GetValue("parent").GetString(0)));
		
		if ( currHeldItem)
			ConnectItems( currHeldItem->Id, parent->Id, _ov);
		_tool->UpdateParams(FOmValue("isHold:(0)"));
	}
}

void UToolsCont::PutToToolset(AToolItem* _toolItem)
{
	AToolItem* tool = (_toolItem) ? _toolItem : currTool;  /* itself or equipment */
	if (tool)
	{
		if (tool->CurrPart)
		{
			UToolsCont::INS->ReleaseItem(tool, false);
			tool = tool->CurrPart;
		}


		ConnectItems( tool->Id, UResource::INS->TOOLS->Id, FOmValue("IsSnapTarget:(0)"));
		tool->RestoreRelTrans("InitRel");
		tool->SetEnable(true);
		tool->SetSelectable(true);
	}
	
}







void UToolsCont::SetHightlight(bool _status, FString _id)
{
	/*AToolItem* tool = Cast<AToolItem>( UResource::INS->GetItem(_id,false));
	if (_status)
	{
		if (tool)
		{
		}
	}
	else
	{

	}*/
	
}

//void UToolsCont::AddCloneItem(AItem* _item, AItem* _target, FString _newName)
//{
//	AItem* targetItem = _target ? _target : _item;
//	FActorSpawnParameters Parameters;
//	Parameters.Template = targetItem;
//
//	AItem* clone = _item->GetWorld()->SpawnActor<class AItem>(_item->GetClass(), Parameters);
//	clone->Id = _newName;
//	clone->IsCLone = true;
//	clone->SetEnable(false);
//	clone->Init();
//	clone->CompMain->SetWorldTransform(targetItem->CompMain->GetComponentTransform());
//	clone->CompRel->SetWorldTransform(targetItem->CompRel->GetComponentTransform());
//
//	UResource::INS->AddItemToList(_newName, clone);
//}

void UToolsCont::RemoveItem(FString _cloneName)
{
	AItem* item = UResource::INS->RemoveItemToList(_cloneName);
	item->Destroy(true);
}


