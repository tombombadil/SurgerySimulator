#pragma once

#include "GameFramework/Actor.h"
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Utils/OmStructs.h"
#include "OmActor.generated.h"


UENUM(BlueprintType)	
enum class ESelectHandlerAction : uint8
{
	CLICK 	UMETA(DisplayName = "CLICK"),
	ROLL_OVER 	UMETA(DisplayName = "ROLL OVER"),
	ROLL_OUT 	UMETA(DisplayName = "ROLL OUT")
};

USTRUCT(BlueprintType)
struct OMCEM_API FSelectHandler
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadOnly) ESelectHandlerAction Action;
		UPROPERTY(BlueprintReadOnly) EChannels RayChannel;
	UPROPERTY(BlueprintReadOnly) UPrimitiveComponent* Comp;
	UPROPERTY(BlueprintReadOnly)  AOmActor* Move;

	FSelectHandler() {};
	FSelectHandler(ESelectHandlerAction _action, EChannels _channel, UPrimitiveComponent* _to, class AOmActor* _move) : Action(_action), RayChannel(_channel), Comp(_to), Move(_move) {};


};


UCLASS()
class OMCEM_API AOmActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AOmActor();



public:	
	UPROPERTY(EditAnywhere, Category = "OMCEM") bool IsEnable = true;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM) FString Id;
	template<class T> T* GetComponent(int8 _index);
	UFUNCTION(BlueprintCallable, Category = "OM3") void SetEnable(bool _isEnable);
	UFUNCTION(BlueprintCallable, Category = "OM3") bool GetEnable();
	UFUNCTION(BlueprintCallable, Category = OMCEM) USceneComponent* GetComponent(FString _insName);
	UFUNCTION(BlueprintCallable, Category = "OM3") AOmActor* GetChildActor(FString _compName); //child actors which added in this actor as child actor. not attached which attacted in world.

	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventRun(const FString& cmd);
	virtual void OnSelect(struct FSelectHandler _handler) { BpEventSelect(_handler); };
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventSelect(struct FSelectHandler SelectHandler);
	virtual void PreDestroy();
private:

};
