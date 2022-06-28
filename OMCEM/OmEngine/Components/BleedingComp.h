#pragma once

#include "OmEngine/Components/OmComp.h"
#include "BleedingComp.generated.h"


UCLASS(ClassGroup = (Custom), Blueprintable, meta = (BlueprintSpawnableComponent))
class OMCEM_API UBleedingComp : public UOmComp
{
	GENERATED_BODY()
private:
	float timeCollisionWithTool;
	bool isOnColl;

public:
	bool IsBleeding;
	void BeginPlay() override;
	void StartBleeding();
	void StopBleeding();
	void Render(float DeltaTime) override;
	//void OnCollision(bool _status, FContactResult* _collInfo) override;

	/* seconds of collision time to stop bleeding */
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float CollisionTimeToStopBleeding = 2;
};
