#pragma once

#include "OmActor.h"
#include "OmEngine/Utils/OmUtils.h"
#include "OmEngine/Utils/OmValue.h"
#include <Components/MeshComponent.h>
#include "Item.generated.h"



struct FContactResult;
struct FOmValues;
struct FSelectHandler;
class UOmComp;
class UResource;
class UToolsCont;



UCLASS()
class OMCEM_API AItem : public AOmActor
{
	friend class UToolsCont;
	GENERATED_BODY()
private:
	//static void omCompEventHandler(UEvent &e);
	TMap<FString, FTransform> listSavedTrans;
	
public:
	//FEventAddItemToComp EventAddItemToComp;
	//FEventRemoveItemToComp EventRemoveItemToComp;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM)  UOmComp* CompTip;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM)  UOmComp* CompRel; /* for relative trans */
	UPROPERTY(BlueprintReadOnly, Category = OMCEM)  UOmComp* CompVol;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) FString ItemName ;
	UPROPERTY(EditAnywhere, Category = OMCEM)	FString IdCat;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM)  FString IdClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = OMCEM) bool IsCatchCollision = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = OMCEM) float CollisionForcStiffness = 0;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM | XRAY") TMap<FString, UMaterialInterface*> ListXrayMats;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OMCEM | XRAY") TArray<FComponentMaterial> ComponentMaterials;
	UPROPERTY() int Index;
	UPROPERTY(BlueprintReadOnly, Category = OMCEM)  bool IsInited;

	UPROPERTY() AItem* OwnerItem; //if this connected to  other items
	UPROPERTY() bool IsCLone; 
	//UPROPERTY(BlueprintReadOnly, Category = OMCEM) bool IsToConnected;
	//UPROPERTY(BlueprintReadOnly, Category = OMCEM) bool IsFromConnected;
	PairTrans InitTrans;
	AActor* initParentActor;
	UFUNCTION(BlueprintCallable, Category=OMCEM) void SetMaterial(UMaterial* _matInstance);
	UFUNCTION(BlueprintCallable, Category="OM3") void SetComponentMat(FString _MaterialId, FString _ComponentName);
	UFUNCTION(BlueprintCallable, Category = "OM3")  TArray<UPrimitiveComponent*> FindComponentsByMatId(FString _MaterialId);
	UFUNCTION(BlueprintCallable, Category = "OM3") void ResetCompMat(FString _ComponentName);
	UFUNCTION(BlueprintCallable, Category = OMCEM) void ResetMaterial();

	UFUNCTION(BlueprintCallable, Category = OMCEM) void SaveTrans(FString _id);
	UFUNCTION(BlueprintCallable, Category = OMCEM) FTransform GetSavedTrans(FString _id);
	UFUNCTION(BlueprintCallable, Category = OMCEM) void SaveRelTrans(FString _id);
	UFUNCTION(BlueprintCallable, Category = OMCEM) FTransform RestoreTrans(FString _id);
	UFUNCTION(BlueprintCallable, Category = OMCEM) FTransform RestoreRelTrans(FString _id);
	virtual void OnCallBackFromBpComp(FString _strCommand) {};

	AItem();
	virtual FString Create(int _index);
	virtual void Init();
	virtual void Start();
	virtual void StartOperation();
	virtual void InitAfter() {};
	virtual void Tick(float DeltaSeconds) override;
	

	//virtual void OnCollision(bool _status, FContactResult* _result);
	//virtual void OnCollisionContinues(FContactResult* _result) ;
	void SetTransWorld(FTransform _trans);
	virtual void SetTrans(PairTrans _pawnTrans);
	virtual PairTrans GetTrans();
	FTransform GetTransWorld();
	/* @_useCompAsPivotPoint : bu id ye sahip component pivot noktasi olarak kullanilir */
	UFUNCTION(BlueprintCallable, Category = OMCEM) void SetItemLoc(FVector _location, bool _isWorld=true, FString _useCompAsPivotPoint="");
	virtual void SetRot(FRotator _rotation, bool _isWorld = true);
	
	AItem* GetChildItem(int8 _index = 0);

	UFUNCTION(BlueprintCallable, Category=OMCEM) virtual void Reset();
	void PreDestroy();
	UFUNCTION(BlueprintCallable, Category = OMCEM) void SetCollisionPreset(FString _presetName);
	UFUNCTION(BlueprintCallable, Category = OMCEM) void ResetCollisionPreset();
	
	UFUNCTION(BlueprintCallable, Category = OMCEM) UOmComp* GetCompByName(FString _id);
	UFUNCTION(BlueprintCallable, Category = OMCEM) TArray<UOmComp*> GetComps();
	UFUNCTION(BlueprintCallable, Category = OMCEM) void SetCollisionEnabled(bool _status);
	UFUNCTION(BlueprintCallable, Category = "OM3") bool GetVisible();
	UFUNCTION(BlueprintCallable, Category = OMCEM) void SetVisible(bool isVisible, bool applyallChildActors);
	UFUNCTION(BlueprintCallable, Category = "OM3") void SetVisibleAllCompsByTag(bool _visiblity, FString _tag, bool _dontVisibleHideComps=true, bool _includeAllChilds=true);
	UFUNCTION(BlueprintCallable, Category = "OM3") void SetVisibleAllCompsByParentComp(bool _visiblity, USceneComponent* _root, bool _dontVisibleHideComps = true, bool _includeAllChilds = true);
	
	AItem* CloneIt();

	UFUNCTION(Category = OMCEM) virtual void Run(FOmValue cmd);
	void UpdateParams(FOmValue _value, bool sendToObserver=true);
	UFUNCTION(BlueprintCallable, Category = OMCEM) void UpdateParams(FString _strParams, bool callPawnEnded=true);
	void SendParams(FOmValue _value);
	FString GetParam(FString _key);
	
	
	/* bp */
	UFUNCTION(BlueprintCallable, Category = OMCEM) AItem* GetOwnerItem() { return OwnerItem; }
	UFUNCTION(BlueprintCallable, Category = "OM3") AItem* GetChildItemsByCatId(FString _idCat);
	/* when app(operation) started all of item's start func is calling by maincont */
	UFUNCTION(BlueprintImplementableEvent,  Category = OMCEM) void BpEventStart(); 
	UFUNCTION(BlueprintImplementableEvent,  Category = OMCEM) void BpEventStartOperation(); 
	UFUNCTION(BlueprintImplementableEvent,  Category = OMCEM) void BpEventOnHit( UOmComp* fromComp, UPrimitiveComponent* toComp, AActor* toItem, FVector _hitPoint); //hit
	UFUNCTION(BlueprintImplementableEvent,  Category = OMCEM) void BpEventOnColl( UOmComp* fromComp,  UOmComp* toComp, AItem* toItem, FVector _location); //overlap
	UFUNCTION(BlueprintImplementableEvent,  Category = OMCEM) void BpEventOnCollRemove( UOmComp* fromComp,  UOmComp* toComp, AItem* toItem);
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void RunBpCommand(const FString& _param); //kaldirilacak
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventStartProcess();
	UFUNCTION(BlueprintImplementableEvent, Category = OMCEM) void BpEventEndProcess();
protected:
	UPROPERTY() TArray< UOmComp*> listOmComps;
	UPROPERTY() TArray<AItem*> listChildItems;
	UPROPERTY() TArray<USceneComponent*> storeWillNotVisibleComps; //SetVisibleAllCompsByTag()
	UPROPERTY()	FOmValue params;
	TMap<UMeshComponent*, TArray<UMaterial*>> listStaticMeshComps;
	void checkCollision();
	TArray<USceneComponent*> getVisiblityCompsList(bool _visiblity, USceneComponent* _root, bool _dontVisibleHideComps, bool _includeAllChilds);
	void createListStaticMeshMaterials();
};


