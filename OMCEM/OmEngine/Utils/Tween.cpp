#include "Tween.h"
#include <GameFramework/Actor.h>
#include "OMCEM.h"

UTween* UTween::INS = NULL;

void UTween::Init()
{
	UTween::INS = this;
}

FActorTween UTween::AddActor(AActor* _actor,  FTransform _to, float _speed)
{
	FActorTween tw;
	FString id = _actor->GetName();
	float speed = _speed;
	if (listActorTweens.Contains(id))
	{
			listActorTweens[id].id = id;
			listActorTweens[id].to = _to;
			listActorTweens[id].from = _actor->GetTransform();
			listActorTweens[id].speed = speed;
			listActorTweens[id].status = 1;
			listActorTweens[id].alpha = 0;
			tw = listActorTweens[id];
			

	}
	else
	{
		FActorTween at;
		at.actor = _actor;
		at.from = _actor->GetTransform();
		at.to = _to;
		at.speed = speed;
		at.status = 1;
		at.alpha = 0;
		at.id = id;
		listActorTweens.Add(id, at);
		tw = at;

	}

	return tw;
}


void UTween::Render(float DeltaTime)
{
	for (auto& pair : listActorTweens)
	{
		FString id = pair.Key;
		FActorTween tw = pair.Value;
		if (tw.status == 1)
		{
			FVector _from = tw.actor->GetActorLocation();
			FVector _newLoc;
			FRotator _newRot;
			if (tw.alpha < 1)
			{
				_newLoc = FMath::InterpExpoOut<FVector>(tw.from.GetLocation(), tw.to.GetLocation(), tw.alpha);
				_newRot = FMath::InterpExpoOut<FRotator>(tw.from.GetRotation().Rotator(), tw.to.GetRotation().Rotator(), tw.alpha);
				tw.alpha += tw.speed;

				
				
			}
			else
			{
				_newLoc = tw.to.GetLocation();
				_newRot = tw.to.GetRotation().Rotator();
				tw.status = 0;
				tw.alpha = 0;
			}

			FTransform _newTrans = tw.actor->GetActorTransform();
			_newTrans.SetLocation(_newLoc);
			_newTrans.SetRotation(_newRot.Quaternion());
			tw.actor->SetActorTransform(_newTrans);
		}

		listActorTweens[id] = tw;
			
			
	}
}

