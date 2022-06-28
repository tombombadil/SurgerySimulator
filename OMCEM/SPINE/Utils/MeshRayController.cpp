


#include "MeshRayController.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Items/Item.h"
#include "DrawDebugHelpers.h"








 UMeshRayController::UMeshRayController()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UMeshRayController::Init(UMeshComponent* _MeshComp, EChannels _RayChannel, FVector _RayDirection, TSubclassOf<class AItem> _TargetClass, float _cornerPaddingRatio)
{
	this->MeshComp = _MeshComp;
	this->RayChannel = _RayChannel;
	this->RayDirection = _RayDirection;
	this->TargetItemClass = _TargetClass;
	cornerPadding = 0.05f;// _cornerPaddingRatio;


	this->SetComponentTickEnabled(true);

	
	RegisterComponent();

	setRayStartPoints();


}


void UMeshRayController::setRayStartPoints()
{
	TArray<FVector> listPoints;

	FBoxSphereBounds box = MeshComp->Bounds;
	listPoints.Add(box.Origin);

	int xCount = 10;
	int YCount = 4;
	float xMaxAngle = 60;
	float yMaxAngle = 60;
	FVector startPoint = box.Origin + FVector::UpVector * 3;
	FVector endPoint;
	for (int i = 0; i < xCount * YCount; i++)
	{
		int xIndex = (i % xCount);
		int YIndex =  FMath::FloorToInt((float)i / (float)YCount);
		float xRatio = (float)xIndex / (float)xCount;
		float yRatio = (float)YIndex / (float)YCount;
		float angleX = -(xMaxAngle/2) + (xRatio * xMaxAngle);
		float angleY = -(yMaxAngle /2) + (yRatio * yMaxAngle);
		FRotator rot = FRotator(angleX, angleY, 0);
		endPoint = startPoint + (rot.RotateVector(FVector::UpVector  * -10));
		//UOmUtils::DrawLine(startPoint, endPoint, FColor::Red, true);
		//DrawDebugSphere(GetWorld(), hit.ImpactPoint, 0.04f, 6, FColor::Yellow, false, -1, 0, 0.2f);
		//_list.Add(_center + FVector(x, y, 0));

		TArray<FHitResult> listHitResult = UOmUtils::RayCast(true, RayChannel, startPoint, endPoint, 0, FColor::Black);
		//UOmUtils::DrawLine(from, to, FColor::Black, true, true);
		if (listHitResult.Num() > 0)
		{
			int i = 0;
			for (FHitResult hit : listHitResult)
			{

				if (hit.GetComponent() == MeshComp)
				{
					if (listRayStartPoints.IsValidIndex(i))
						listRayStartPoints[i] = (hit.ImpactPoint);
					else
						listRayStartPoints.Add(hit.ImpactPoint);

					//if (DrawDebugStartRayPoints) DrawDebugSphere(GetWorld(), hit.ImpactPoint, 0.04f, 6, FColor::Yellow, true, -1, 0, 0.2f);
					i++;
				}
			}

		}

	}


	//addPointsToRect(box.Origin, box.BoxExtent, listPoints);
	//addPointsToRect(box.Origin, box.BoxExtent/2, listPoints);

	//for (FVector point : listPoints)
	//{
	//	FVector from = point;
	//	FVector to = point + RayDirection;
	//	TArray<FHitResult> listHitResult = UOmUtils::RayCast(true, RayChannel, from, to, 0, FColor::Black);
	//	//UOmUtils::DrawLine(from, to, FColor::Black, true, true);
	//	if (listHitResult.Num() > 0)
	//	{
	//		int i = 0;
	//		for (FHitResult hit : listHitResult)
	//		{

	//			if (hit.GetComponent() == MeshComp)
	//			{
	//				if(listRayStartPoints.IsValidIndex(i))
	//					listRayStartPoints[i] = (hit.ImpactPoint);
	//				else
	//					listRayStartPoints.Add(hit.ImpactPoint);

	//				if(DrawDebugStartRayPoints) DrawDebugSphere(GetWorld(), hit.ImpactPoint, 0.04f, 6, FColor::Yellow, true, -1, 0, 0.2f);
	//				i++;
	//			}
	//		}

	//	}

	//}
}

void UMeshRayController::addPointsToRect(FVector _center, FVector _extents, TArray<FVector>& _list)
{
	int rowCount = 2;
	FVector boxSize = _extents * 2;
	FVector spaces = boxSize * cornerPadding;
	float w = (boxSize.X - (rowCount * spaces.X)) / (rowCount - 1);
	float h = (boxSize.Y - (rowCount * spaces.Y)) / (rowCount - 1);
	for (int i = 0; i < RayCount / 2; i++)
	{

		int xIndex = FMath::FloorToInt((float)i / (float)rowCount);
		int YIndex = (i % rowCount);
		float x = (-_extents.X + spaces.X) + (xIndex * w);
		float y = (-_extents.Y + spaces.Y) + (YIndex * h);

		_list.Add(_center + FVector(x, y, 0));

	}
}
bool UMeshRayController::GetResult(TArray<AItem*> GroupList)
{
	
	return GetResult(GroupList, 0);
}




bool UMeshRayController::GetResult(TArray<AItem*> GroupList, UMeshRayController* TargetMeshRay)
{
	bool result = false;
	FBoxSphereBounds box = MeshComp->Bounds;
	FVector boxExtent = box.BoxExtent;
	//FVector from = MeshComp->GetComponentLocation();
	//DrawDebugBox(GetWorld(), box.Origin, boxExtent, FColor::Purple, true, 1, 0, 0.2f);
	if(listRayStartPoints.IsValidIndex(0))
	{

		result = (getSingleRayResult(listRayStartPoints[0], GroupList, TargetMeshRay) != 0);
	}

	if (!result && listRayStartPoints.Num() > 1)
	{


		for (int i = 1; i < listRayStartPoints.Num(); i++)
		{
			if (getSingleRayResult(listRayStartPoints[i], GroupList, TargetMeshRay) != 0)
			{
				result = true;
				return result;
				//break;
			}
		}

		

		
			


	}

	return result;
}


void   UMeshRayController::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//setRayStartPoints();
}

AActor* UMeshRayController::getSingleRayResult(FVector _from, TArray<AItem*> _exceptItemsList, UMeshRayController*_fromMeshRay)
{
	FVector hitLoc = FVector::ZeroVector;
	AActor* result = 0;
	FVector to = _from + RayDirection;
	TArray<FHitResult> listHitResult = UOmUtils::RayCast(true, RayChannel, _from, to, 0, FColor::Black);
	if (listHitResult.Num() > 0)
	{
		//FHitResult hit = listHitResult[0];
		//UOmUtils::DrawLine(_from, to, FColor::Yellow, false, true);
		//for (FHitResult hit : listHitResult)
		for (FHitResult hit : listHitResult)
		{
			if (hit.GetActor()->IsA(TargetItemClass) && hit.GetActor() != this->GetOwner())
			{
				hitLoc = hit.ImpactPoint;
				UMeshRayController* meshRay = getTargetMeshRayCont(hit.GetActor());

				int32 foundIndex = -1;
				_exceptItemsList.Find((AItem*)hit.GetActor(), foundIndex);
				if (meshRay &&  foundIndex > -1)
				{
					TArray<AItem*> emptylist;
					bool targetRayContResult = meshRay->GetResult(emptylist,this);
					if (targetRayContResult) result = hit.GetActor();

				}
				else
				{
					result = hit.GetActor();
				}

				break;


			}
			//if(!_exceptItemsList.Find((AItem*) hit.GetActor()))
				//result = true;
		//if ( Cast<TargetItemClass>( hit.GetActor()) )
		//if ( TargetItemClass->IsA( hit.GetActor() ))
		//if (hit.GetActor()->IsA((AItem*) TargetItemClass->StaticClass()))
			//result = true;
		}
	}
	
	if (_fromMeshRay)
	{
		if(result)
		if (result == _fromMeshRay->GetOwner())
			return 0;
	}
	else
	{
		if (result)
		{
			if(DrawDebugHitLines)UOmUtils::DrawLine(_from, to, FColor::Red, false, true);
			//DrawDebugSphere(GetWorld(), hitLoc, 0.2f, 6, FColor::Purple, false, -1, 0, 0.2f);
		}
			
		else
			if (DrawDebugHitLines)UOmUtils::DrawLine(_from, to, FColor::Green, false, true);

		
	}
	
	
	return result;
}




UMeshRayController* UMeshRayController::getTargetMeshRayCont(AActor* _actor)
{

	UMeshRayController* r = 0;
	if (_actor == GetOwner()) return r;
	if (_actor)
	{
		AItem* item = Cast<AItem>(_actor);
		if (item)
		{
			UMeshRayController* meshRay = Cast<UMeshRayController>(item->GetComponentByClass(UMeshRayController::StaticClass()));
			if (meshRay)
			{
				r = meshRay;
			}
		}
	}
	return r;
}