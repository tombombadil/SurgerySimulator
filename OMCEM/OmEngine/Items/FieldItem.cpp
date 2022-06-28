#include "FieldItem.h"
#include "OMCEM.h"
#include "ToolItem.h"
#include "OmEngine/Utils/Resource.h"
#include "OmEngine/Components/OmComp.h"
#include "OmEngine/Controllers/ToolsCont.h"
#include "OmEngine/Controllers/Observer.h"


AFieldItem::AFieldItem()
{
	IsLockCam = true;
}
void AFieldItem::InitAfter()
{
	AItem::InitAfter();

	
	if (ListPuts.Num() > 0)
	{
		listPutTrans.Empty();
		for (FString n : ListPuts)
		{
			AItem* item = UResource::INS->GetItem(n, true);
			if (item)
			{
				listPutTrans.Add(item->GetRootComponent()->GetRelativeTransform());
			}
		}
	}

	if (ListTakes.Num() > 0)
	{
		listTakes.Empty();
		for (FString n : ListTakes)
		{
			AItem* item = UResource::INS->GetItem(n, true);
			if (item)
			{
				listTakes.Add(item);
			}
		}
	}

	
	
	
}




AItem* AFieldItem::TakePart(int8 _index/*=0*/)
{
	AItem* item = 0;
	if (_index < listTakes.Num())
	{
		item = listTakes[_index];
		if (item)
		{
			listTakes.Remove(item);
		}
	}
	return item;
}

void AFieldItem::PutPart(AItem* _item)
{

	//if (!listPuts.Contains(_item))
	{
		UToolsCont::INS->ConnectItems(  _item->Id, this->Id, FOmValue("IsSnapTarget:(1)"));
		
		listPuts.Add(_item);
		int8 currIndex = listPuts.Num() - 1;

		FVector newRelLoc = _item->GetRootComponent()->GetRelativeTransform().GetLocation();
		FRotator newRelRot = FRotator::ZeroRotator;


		if (listPutTrans.IsValidIndex(currIndex))
		{
			newRelLoc += listPutTrans[currIndex].GetLocation();
		}

		else
		{
			if (TakeType == ETakeType::TAKE_ZERO_LOC)
			{
				newRelLoc = FVector::ZeroVector;
			}
			else if (TakeType == ETakeType::TAKE_RANDOM_LOC_IN_VOL)
			{
				if (CompVol)
				{
					FVector box = CompVol->GetUnscaledBoxExtent();
					newRelLoc.X +=  FMath::RandRange(-box.X, box.X);
					newRelLoc.Y += FMath::RandRange(-box.Y, box.Y);
				}
			}
		}
		

		_item->SetActorRelativeLocation(newRelLoc);
		_item->SetActorRotation(FRotator::ZeroRotator);
		_item->SetActorRelativeRotation(newRelRot);

	}
}





















void AHoleField::SetDepth(float _depth)
{
	depth = FMath::Abs(_depth);
}

float AHoleField::GetDepth()
{
	return depth;
}


AToolItem* AHoleField::GetScrew()
{
	UOmComp* screwCOmp = GetCompByName("Screw");
	if (screwCOmp)
	{
		USceneComponent* screw = RootComponent->GetAttachChildren()[0];
	}
	
	return 0;
}
