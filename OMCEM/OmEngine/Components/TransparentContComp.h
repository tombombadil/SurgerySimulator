// Copyright 2015 - 2020 SharpSurgeon 
#pragma once

#include "OmComp.h"
#include "TransparentContComp.generated.h"


USTRUCT()
struct OMCEM_API FTransparentElement
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY() UPrimitiveComponent* comp;
	UPROPERTY()	bool isStaticMesh;
	UPROPERTY()	TArray< UMaterialInterface*> listDefaultMats;


	FTransparentElement() {};
	FTransparentElement(UPrimitiveComponent* _comp)
	{
		comp = _comp;
		if (comp)
		{
			for (int8 i = 0; i < _comp->GetNumMaterials(); i++)
			{
				UMaterialInterface* mat = comp->GetMaterial(i);
				listDefaultMats.Add(mat);
			}
		}
	}



};



UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UTransparentContComp : public UOmComp
{
	GENERATED_BODY()
private:
	UPROPERTY() TArray< FTransparentElement> listElements;
	void initElements();
public:
	UPROPERTY(EditAnywhere)	UMaterialInterface* TransparentMaterial;
	void BeginPlay() override;
	void Render(float DeltaTime) override;

	

	UFUNCTION(BlueprintCallable) void SetEnableTransparent(bool _status, FString _meshTag = "");
	void setElementMat(FTransparentElement _elem, UMaterialInterface* _mat);
};
