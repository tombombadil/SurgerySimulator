#pragma once

#include "CoreMinimal.h"
#include "OmEngine/Items/Item.h"
#include "../Utils/SpineStructs.h"
#include "LaminectomyContainer.generated.h"

USTRUCT(BlueprintType)
struct OMCEM_API FLaminectomyObjects
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString Id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AItem> BoneClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AItem> Ligament1Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TSubclassOf<AItem> Ligament2Class;



};



UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API ALaminectomyContainer : public AItem
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, Category = "OM3") TArray< FLaminectomyObjects> ListData;
	void Init() override;
	void SpawnObjects(FSpineData _data);
private:
	UPROPERTY() UOmComp* containerBones;
	UPROPERTY() UOmComp* containerLigaments;
	void spawnBone(FString _lamBoneId);
	void spawnObject(UOmComp* _parent, TSubclassOf<AItem> _Class);
};
