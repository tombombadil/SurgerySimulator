#include "OmActor.h"
#include "OMCEM.h"


AOmActor::AOmActor()
{
	PrimaryActorTick.bCanEverTick = true;
}







USceneComponent* AOmActor::GetComponent(FString _insName) //to-do : her seferinde allComponents çaðýrmak performanslý deðil!
{
	
	USceneComponent* _comp = 0;
	if (!_insName.IsEmpty())
	{
		TArray<USceneComponent*> allComponents;
		GetComponents(allComponents,true);
		for (USceneComponent* sceneComp : allComponents)
		{
			if (sceneComp->GetName() == _insName)
			{
				_comp = sceneComp;
			}

		}
	}
	return _comp;
}

template<class T>
T* AOmActor::GetComponent(int8 _index) //to-do : her seferinde allComponents çaðýrmak performanslý deðil!
{
	T* _comp = 0;
	if (_index >= 0)
	{
		TArray<UActorComponent*> allComponents = GetComponentsByClass(UActorComponent::StaticClass());
		TArray<T*> arr;
		int8 i = 0;
		for (UActorComponent* actorComp : allComponents)
		{
			T* castComp = Cast<T>(actorComp);
			if (castComp)
			{
				if (i == _index)
				{
					_comp = castComp;
					return _comp;
				}
				i++;

			}
		}
	}
	return _comp;
}

void AOmActor::SetEnable(bool _isEnable)
{
	IsEnable = _isEnable;
	if(GetName() == "s_45_6")
	Debug(GetName() + " SetEnable ", _isEnable);
}


bool AOmActor::GetEnable()
{
	return IsEnable;
}

AOmActor* AOmActor::GetChildActor(FString _compName)
{
	UChildActorComponent* caComp = Cast<UChildActorComponent>( GetComponent(_compName));
	AOmActor* actor = 0;
	if (caComp) actor = Cast<AOmActor> (caComp->GetChildActor());
	return actor;
}


void AOmActor::PreDestroy()
{

}


