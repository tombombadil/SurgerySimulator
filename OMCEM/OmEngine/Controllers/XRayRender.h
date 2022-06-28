#pragma once

#include "OmEngine/Utils/Resource.h"
#include "OmEngine/Items/Item.h"
#include "XRayRender.generated.h"

class AItem;

/* ___________________ #XrayElement ___________________ */
USTRUCT()
struct OMCEM_API FXrayElement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY() UPrimitiveComponent* comp;
	UPROPERTY()	bool isStaticMesh;
	UPROPERTY()	TArray< UMaterialInterface*> listDefaultMats;

	
	FXrayElement() {};
	FXrayElement(UPrimitiveComponent* _comp)
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

/* ___________________ #AXRayRender ___________________ */
UCLASS()
class OMCEM_API AXRayRender : public AItem
{
	GENERATED_BODY()
public:
		static  AXRayRender* INS;
		AXRayRender();
	UPROPERTY(EditAnywhere, Category = OMCEM) TArray<FString> ListClassIdShowInXRay;

	UPROPERTY(EditAnywhere, Category = OMCEM) TArray<class UTextureRenderTarget2D*> ListRenderTargetsRight;
	UPROPERTY(EditAnywhere, Category = OMCEM) TArray<class UTextureRenderTarget2D*> ListRenderTargetsTop;
	UPROPERTY(EditAnywhere, Category = OMCEM) TArray< AItem*> ListBones;
	UPROPERTY(EditAnywhere, Category = OMCEM) float MaxDistanceToElements = 50;
	void Init();
	void DetermineHiddenItems();
	void setElementMat(FXrayElement _elem, UMaterialInterface* _mat);
	void RefreshCaptures();

	
private:
	UPROPERTY() USceneCaptureComponent2D* captureCompRigt;
	UPROPERTY() USceneCaptureComponent2D* captureCompTop;
	//UPROPERTY() TArray<FXrayElement> listXrayElements;
	UPROPERTY() TArray<UPrimitiveComponent*> listXrayElements;
	UPROPERTY() TArray<AItem*> listHiddenXrayItems;
	UPROPERTY() UOmComp* compXrayArea;
	void capture();
	void ClearCaptures();
	void applyXrayMats();
	void applyResetMats();
	void applyMats(bool _status);
};


