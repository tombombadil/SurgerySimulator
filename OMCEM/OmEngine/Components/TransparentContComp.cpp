#include "TransparentContComp.h"
#include "OMCEM.h"
#include <Components/MeshComponent.h>




void UTransparentContComp::BeginPlay()
{
	Super::BeginPlay();
	initElements();
}

void UTransparentContComp::Render(float DeltaTime)
{
	Super::Render(DeltaTime);

	
}



void UTransparentContComp::initElements()
{
	TArray<USceneComponent*> listComps;
	GetChildrenComponents(true, listComps);
	for (USceneComponent* sceneComp : listComps)
	{
		UMeshComponent* comp = Cast<UMeshComponent>(sceneComp);
		if (comp)
		{
			if (comp->GetOwner() == this->GetOwner())
			{
				listElements.Add(FTransparentElement(comp));
			}
				
		}


	}


	
}


void UTransparentContComp::SetEnableTransparent(bool _status, FString _meshTag)
{
	for (FTransparentElement elem : listElements)
	{
		if (_status)
		{
			bool isApplyStatus = false;
			if (_meshTag.IsValidIndex(0))
			{
				if (elem.comp->ComponentHasTag(*_meshTag))
				{
					isApplyStatus = true;
					
				}
			}
			else
			{
				isApplyStatus = true;
			}

			if (isApplyStatus) setElementMat(elem, TransparentMaterial);
		}
		else
		{
			setElementMat(elem, 0);
		}
		

	}



}



void UTransparentContComp::setElementMat(FTransparentElement _elem, UMaterialInterface* _mat)
{
	if (_elem.comp)
	{
		for (int8 i = 0; i < _elem.listDefaultMats.Num(); i++)
		{
			if (_mat)
			{
				_elem.comp->SetMaterial(i, _mat);
			}
			else
			{
				_elem.comp->SetMaterial(i, _elem.listDefaultMats[i]);
			}

		}
	}

}
