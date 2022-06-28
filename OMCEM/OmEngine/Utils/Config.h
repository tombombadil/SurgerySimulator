#pragma once
#include <vector>
#include <Engine/GameInstance.h>
#include "Config.generated.h"

using namespace std;

class UResource;




UCLASS()
class OMCEM_API UConfig : public UGameInstance
{		
	GENERATED_BODY()
private:
	TMap<FString, FString> listDebugParams;
	//TMap<EOperation, struct FOperation> listOperations;
public:
	static UConfig* INS;

	UPROPERTY(BlueprintReadWrite, Category = OMCEM) int DebugMode = -1;
	/* Gamemode 0=Beginner, 1=Advanced */
	UPROPERTY(BlueprintReadWrite, Category = OMCEM) int LevelMode = 0;
	UFUNCTION(BlueprintCallable, Category = OMCEM) class AMainCont* GetMainController();
	UFUNCTION(BlueprintCallable, Category = OMCEM) class UResource* GetResource();
	//UFUNCTION(BlueprintCallable, Category = OMCEM) FOperation GetOperation(EOperation _operation);

	
	UConfig();
	void Init() override;
	void AddDebugParam(FString _paramName, FString _param);
	FString PrintDebugParams();




//static FString TRACE_RAY_RIGIDBODY = "TRACE_RAY_RIGIDBODY";
//static FString TRACE_RAY_PROCESS = "TRACE_RAY_PROCESS";
//static FString TRACE_RAY_SELECT_TOOL = "TRACE_RAY_SELECT_TOOL";

	/* static values */

	static FString GET_MSG(FString _valueId);
	static float OBSERVER_PVALUE_TOL;
	static vector<float> INPUT_MOUSE_MOVE_SPEED_XY;
	static vector<float> INPUT_MOUSE_MOVE_SPEED_Z;
	static vector<float> CAM_MAIN_ZOOM_RANGE;
	static vector<float> INPUT_CAM_ROTATION_OFFSET_LIMIT_X;
	static vector<float> INPUT_CAM_ROTATION_OFFSET_LIMIT_Y;
	static float CAM_MAIN_ZOOM_SPEED;
	static float INPUT_MOUSE_WHEEL_ZOOM_SPEED;
	static FVector PAWN_AFTER_LOC_OFFSET;
	static FVector PAWN_AFTER_TOOL_POSITION_OFFSET;
	static FRotator RIGHT_OP_TOOL_MOVE_ROT; //tool move rotation when on right operation
	static FRotator LEFT_OP_TOOL_MOVE_ROT;
	/* Unrealin kendi world rotation'nda FRotation(0,0,0) dogrudan FVector(1,0,0 )x axisi isaret editor. Biz ise operasyonlarda FVector(0,0,1) z'yi referans aliyoruz. Bu offset bu farki kapatiyor/donusturuyor.*/
	static FRotator OPERATION_ROTATION_OFFSET;
	static float HUD_WIDGET_AWAY_FROM_CAM;
	//static TArray<FString> OperationJsonFiles;
private:
};

