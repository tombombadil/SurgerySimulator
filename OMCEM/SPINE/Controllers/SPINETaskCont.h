#pragma once

#include "OmEngine/Controllers/TaskCont.h"

#include "SPINETaskCont.generated.h"



UCLASS(BlueprintType, Blueprintable)
class OMCEM_API USPINETaskCont : public UTaskCont
{
	GENERATED_BODY()
public: 
	static USPINETaskCont* INS;
	USPINETaskCont();

	
	
protected:
	virtual void InitTasks() override;
	//void GenRod(FString _taskName, FRodInfo _rodInfo);
	//UTask* GenNutPut(FString _taskName, FNutFieldInfo _nutFieldInfo);
	//UTask* GenNutTighten(FString _taskName, FNutFieldInfo _nutFieldInfo, FString _model, FString _addParams);
	//UTask* GenSeperate(bool _isSeperate, FRodInfo rodInfo, FString _model, FString _addParams, FString _boardMsg);
	//void GenScrewApply(FHoleInfo _hole, FOmValue _makeHoleModel, FString _operationDirection);
	//UTask* GenBreakTask(int8 _index, FPartFieldInfo _takeField);
	//UTask* GenBreakTask(int8 _index, FPartFieldInfo _takeField, FPartFieldInfo _putField);
	//void GenInsertCage();
	//void GenPlaceCage();
	//void GenDiscCleaner(int8 _discCount);
	//void GenPutDiscGrafts(int8 _partCount);
};



