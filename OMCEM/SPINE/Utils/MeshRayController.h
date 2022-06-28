// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OmEngine/Utils/OmStructs.h"
#include <NoExportTypes.h>
#include <Components/MeshComponent.h>
#include "MeshRayController.generated.h"


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UMeshRayController : public USceneComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") UMeshComponent* MeshComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") EChannels RayChannel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") TSubclassOf<class AItem>  TargetItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") int RayCount = 8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") FVector RayDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") bool DrawDebugStartRayPoints = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "om3") bool DrawDebugHitLines = true;
	 UMeshRayController();
	/*
	@_cornerPaddingRatio : must be between 0-1 */
	UFUNCTION(BlueprintCallable, Category = "om3") void Init(UMeshComponent* _MeshComp, EChannels _RayChannel, FVector _RayDirection, TSubclassOf<class AItem> _TargetClass, float _cornerPaddingRatio);


	/*
	@GroupList : grup olarak alinacak diger itemlarin listesi. bu component  uzerinde bu listeden bir item var ise, bu item check edilir ve bu check edilen item false donerse, bu component de false doner 
	@CheckoutUpper : her zaman true olmali
	*/
	UFUNCTION(BlueprintCallable, Category = "om3") bool GetResult(TArray<AItem*> GroupList);
	bool GetResult(TArray<AItem*> GroupList, UMeshRayController* TargetMeshRay);
	void    TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	AActor* getSingleRayResult(FVector _from, TArray<AItem*> _exceptItemsList, UMeshRayController* _fromMeshRay);
	UMeshRayController* getTargetMeshRayCont(AActor* _actor);
	float cornerPadding  ;
	TArray<FVector> listRayStartPoints;
	void setRayStartPoints();
	void addPointsToRect(FVector _center, FVector _extents, TArray<FVector>& _list);
};
