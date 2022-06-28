#pragma once

#include "OmEngine/Items/Item.h"
#include "OmEngine/Ui/Widget/ToolWidgetComp.h"
#include "../Components/ContainerComp.h"
#include "ToolItem.generated.h"

class UOmComp;
class UToolsCont;
class AItem;
class AFieldItem;



UCLASS()
class OMCEM_API AToolItem : public AItem
{
	friend class UToolsCont;
	GENERATED_BODY()

private:
	UPROPERTY() bool isToolActive; //if is selected
	UPROPERTY() bool isPawn;
	
	
	void setFieldItem(AFieldItem* _field, bool _status);

	PairTrans moveTrans;
	PairTrans fromRelToWorldOmTrans(UOmComp* _comp, PairTrans _relTrans);
	
public:
	
	UPROPERTY() UToolWidgetComp* CompWidget;
	UPROPERTY() UContainerComp* CompContainer;
	UPROPERTY()bool IsToogleMouseDown;
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") bool IsTurnOn; //haptic btn on press. so tool is working..
	UPROPERTY(BlueprintReadOnly, Category = "OMCEM") bool IsOnProcess;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OMCEM") bool IsPartOfTool;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OMCEM") int VrHandAnimStatus; //vr hand rig anim status
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "OMCEM") FVector ToolSize; //x(width), z(length)
	UPROPERTY(EditAnywhere, Category = OMCEM)	EPutToolRule RuleOfPutToToolset;
	UPROPERTY() AItem* childItem;
	UPROPERTY(EditAnywhere, Category = "OMCEM") bool IsVibrationTool;
	UPROPERTY() FString Label; 
	UPROPERTY(EditAnywhere, Category = "OMCEM") TArray<FString> ListCanTakeParts;
	UPROPERTY() class AToolItem *CurrPart;
	AToolItem();
	virtual void Init() override;
	void TakePartInField(AFieldItem* _field);
	void PutPartInField(AFieldItem* _field);
	virtual void SetTrans(PairTrans _pawnTrans) override;
	//UFUNCTION(BlueprintCallable, Category = OMCEM) void SetLoc(FVector _location, bool _isWorld);
	void  Tick(float DeltaSeconds) override;
	virtual PairTrans GetTrans() override;
	void SetSelectable(bool _selectable);
	void SetProcess(bool _status, UProcessComp* _compPawn, FOmValue _params);
	UFUNCTION(BlueprintCallable, Category = OMCEM) bool GetIsPawn() { return isPawn; }
	virtual void SetActiveTool(bool _IsActive, bool _putToToolset = true);
	UFUNCTION(BlueprintCallable, Category = OMCEM) bool GetIsToolActive();
	UFUNCTION(BlueprintCallable, Category = OMCEM)  void HoldItem(AItem* _item);
	UFUNCTION(BlueprintCallable, Category = OMCEM)  AItem* ReleaseItem(bool _disableChild);
	FVector GetTipPoint(bool _isWorld);

	 void SetCurrPart(AToolItem* _toolPart);
	 UFUNCTION(BlueprintCallable, Category = "OM3") AToolItem* GetCurrPart();
protected:
	

};





UCLASS()
class OMCEM_API AEqItem : public AToolItem
{
	friend class UToolsCont;
	GENERATED_BODY()
};
