#pragma once

#include <vector>
#include <GameFramework/Actor.h>
#include <JsonObject.h>
#include "OmStructs.generated.h"


class UInputs;
class USceneComponent;

using namespace std;
#define JsObj SharedPtr<FJsonObject>;

/*_____________________ INFO's _____________________ */
USTRUCT()
struct OMCEM_API FHoleInfo
{
	GENERATED_USTRUCT_BODY()

		FString id;
		FString label;
		 
	class AItem* hole;
	class AToolItem* screw;
	//UOmComp* compColl;
	class UOmComp* volume;
	class UProcessComp* processEntry;
	class UProcessComp* pawnMakeHole;
	class UProcessComp* pawnProbe;
	class UProcessComp* pawnScrew;
	class AToolItem* toolEntry;
	class AToolItem* toolMakeHole;
	class AToolItem* toolProbe;
	class AToolItem* toolScrew;
	FHoleInfo() {};
	FHoleInfo(FString holeId, AToolItem* _toolEntry, AToolItem* _toolMakeHole, 	AToolItem* _toolProbe, 	AToolItem* _toolScrew, AToolItem* _screw, FString _label);

};




USTRUCT()
struct OMCEM_API FRodInfo
{
	GENERATED_USTRUCT_BODY()

		class AToolItem* rod;
	class AItem* rodField;
	class AToolItem* instertTool;
	class AToolItem* distractTool;
	class UOmComp* vol;
	class UProcessComp* processSep;
	class UProcessComp* processInsert;
	FRodInfo() {};
	FRodInfo(FString _fieldId, FString _rodId, AToolItem* _instertTool, AToolItem* _distractTool);

};

USTRUCT()
struct OMCEM_API FNutFieldInfo
{
	GENERATED_USTRUCT_BODY()

	class AToolItem* nut;
	class AItem* nutField;
	class UOmComp* vol;
	class UProcessComp* tighten;
	class AToolItem* insertTool;
	class AToolItem* tightenTool;
	UProcessComp* insert;
	FNutFieldInfo() {};
	FNutFieldInfo(FString _fieldId, FString _nutId, AToolItem* _insertTool, AToolItem* _tightenTool);

};

/* take & put something.. */
USTRUCT()
struct OMCEM_API FPartFieldInfo
{
	GENERATED_USTRUCT_BODY()

	class AFieldItem* field;
	class UOmComp* vol;
	class UProcessComp* take;
	class AToolItem* takeTool;
	class UProcessComp* put;
	FPartFieldInfo() {};
	FPartFieldInfo(FString _fieldId, AToolItem* _takeTool);

};

USTRUCT()
struct OMCEM_API FDataProvider
{
	GENERATED_USTRUCT_BODY()
		float value;
	float ratio ;
	TArray<float> data;
	bool IsValid;

	FDataProvider() {
		IsValid = false;
		value = ratio = 0;
	}



	FDataProvider(vector<float> _data);
	

	void SetValue(float _value);
	float GetValue();
	void SetRatio(float _value);
	float GetRatio();

	void Destroy();
	FString ToString();

};




UENUM()
enum class EInputSource : uint8
{
	NONE,
	HAPTIC_RIGHT  UMETA(DisplayName = "HAPTIC_RIGHT"),
	HAPTIC_LEFT  UMETA(DisplayName = "HAPTIC_LEFT"),
	MOUSE  UMETA(DisplayName = "MOUSE"),
	VR_LEFT  UMETA(DisplayName = "VR LEFT HAND"),
	VR_RIGHT  UMETA(DisplayName = "VR RIGHT HAND")

};


USTRUCT(BlueprintType)
struct OMCEM_API FInputInfo
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(BlueprintReadOnly)	EInputSource Source;
	//UPROPERTY(BlueprintReadOnly)	FString Key;
	UPROPERTY(BlueprintReadOnly)	FString KeyPreset;
	UPROPERTY(BlueprintReadOnly)	float Value;


};



UENUM(BlueprintType)
enum class EOperation : uint8
{
	TLIF,
	CERVICAL,
	SCOLIOSIS,
	THA

};


UENUM(BlueprintType)
enum class EShowRayLineRule : uint8
{
	ALWAYS_HIDE,
	SHOW_WHEN_RAY_HIT,
	ALWAYS_SHOW

};


UENUM(BlueprintType)
enum class EPutToolRule : uint8
{
	PUT_KEEP_TRANSFORM,
	PUT_TOOLSET_RAY_POINT, //toolset uzerindeki tam ray noktası kesismesine birakir
	PUT_TOOLSET_RESTORE_INIT_TRANSFORM, //toolsetteki baslangic pozisyonuna geri doner
	PUT_IN_TRAY, //toolset uzerindeki tray(tabak) içindki rasgele bir pozisyona koyar


};



USTRUCT(BlueprintType)
struct OMCEM_API FOperation
{
	GENERATED_USTRUCT_BODY()
		bool IsValid;
		FString Id;
	FString jsonFileName;
	TSharedPtr<FJsonObject> Json;

	FOperation() {};
	FOperation(FString _id)
	{
		Id = _id;
		jsonFileName = Id.ToLower() + ".txt";
		IsValid = true;
	};
};






UENUM(BlueprintType)
enum EEventType
{
	INIT,
	START,
	UPDATE,
	END,
	SUCCESS,
	ERROR,
	COMPLETE,
	PAWN_START,
	PAWN_UPDATE,
	PAWN_END, //pawn sonucucnun observera gonderilmeye hazir oldugu durumlar
	PAWN_SUCCESS, //pawn sonucu observer disinda belirlenen durumlar
	PAWN_COMPLETE, //pawn sonucunun observer tarafindan belirlenen ve pawnin taamlandigi durumlar
	ON_COLLISION,
	ON_COLLISION_EXIT,
	ON_HIT,
	INPUT_MOVE_x,
	INPUT_MOVE_Y,
	INPUT_MOUSE_DOWN,
	INPUT_MOUSE_UP,
	ON_CLICK,
	ON_CLOSE_UI,
	ON_CALL_FROM_BP,
	TO_CONNECTED,
	FROM_CONNECTED,
	TO_UNCONNECTED,
	FROM_UNCONNECTED,
	INPUT_KEY_PRESS,
	INPUT_KEY_DOUBLE_CLICK,
	INPUT_KEY_RELEASE,
	LOOK_AT_START,
	LOOK_AT_END,
	DEBUG,
};

USTRUCT(BlueprintType)
struct OMCEM_API FProcessResult
{
	GENERATED_USTRUCT_BODY()
		bool Result;
	class UProcess* Process;
	class URule* SuccRule;

};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEventOnStatus, EEventType, _event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEventProcessStatus, EEventType, _eventType,  FProcessResult, _result);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusWithParams, EEventType, _event, FOmValue _params);



USTRUCT(BlueprintType)
struct OMCEM_API FComparison
{
	GENERATED_USTRUCT_BODY()
		bool IsValid;
	FString Value1;
	FString Value2;
	FString Operator;
	

	FComparison() {};
	FComparison(FString _str)
	{
		const TCHAR* Delims[] = { TEXT("=="), TEXT("!="),  TEXT(">="), TEXT("<="),  TEXT("<"), TEXT(">") };
		
		TArray<FString> arrParsed;
		_str.ParseIntoArray(arrParsed, Delims, 6);

		if (arrParsed.IsValidIndex(1))
		{
			
			Value1 = arrParsed[0];
			Value2 = arrParsed[1];

			const TCHAR* d[] = { *Value1, *Value2 };
			TArray<FString> arrParsedOp;
			 _str.ParseIntoArray(arrParsedOp, d, 2);
			 if (arrParsedOp.IsValidIndex(0))
			 {
				 Operator = arrParsedOp[0];
				 //IsValid = true;
			 }
			
		}
	};

	FString ToString()
	{
		return Value1 + Operator + Value2;
	}


};



UENUM(BlueprintType) 
enum class EComparisionResult : uint8
{
	KEY_NOT_FOUND_IN_REQ,
	COMPARISION_FALSE,
	COMPARISION_TRUE

};



UENUM(BlueprintType) /* DefaultEngine.ini ile ayni sirada olmali */
enum class EChannels : uint8
{
	TRACE_RAY_RIGIDBODY,
	TRACE_RAY_TOOLSELECT,
	TRACE_RAY_PROCESS,
	TRACE_RAY_WIDGET

};













USTRUCT(BlueprintType)
struct OMCEM_API FComponentMaterial
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, Category="OM3") FString ComponentName;
	/* List<MaterialId, Material>*/
	UPROPERTY(EditAnywhere, Category = "OM3")  TMap<FString, UMaterialInterface*> Materials;


};






















/* ___________________ #FContactResult ___________________ */
USTRUCT(BlueprintType)
struct OMCEM_API FContactResult

{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(BlueprintReadWrite)	USceneComponent* fromComp;
	UPROPERTY(BlueprintReadWrite)	USceneComponent* toComp;
	UPROPERTY(BlueprintReadWrite)	AActor* fromActor;
	UPROPERTY(BlueprintReadWrite)	AActor* toActor;
	UPROPERTY(BlueprintReadWrite) TArray<FVector> ArrPoints;
	UPROPERTY(BlueprintReadWrite) FVector HitLoc;
	UPROPERTY(BlueprintReadWrite) FVector RayHitNormal;
	UPROPERTY(BlueprintReadWrite) float RayHitRatio;
	FContactResult() {}

	FContactResult(USceneComponent* _fromComp, USceneComponent* _toComp)
	{
		toComp = _toComp;
		if (_toComp) toActor = _toComp->GetOwner();
		fromComp = _fromComp;
		if (_fromComp) fromActor = _fromComp->GetOwner();
	}
	static FContactResult Create(USceneComponent* _toComp)
	{
		return FContactResult(0, _toComp);
	}

	UOmComp* GetFromComp();
	AItem* GetFromItem();
	UOmComp* GetToComp();
	AItem* GetToItem();


	FString ToString()
	{
		FString fi = "";//fromItem ? fromItem->GetName() : "";
		FString rta = toActor ? toActor->GetName() : "";
		FString rtc = toComp ? toComp->GetName() : "";
		FString p = HitLoc.ToString();
		FString s = "fromItem:(" + fi + "),  toActor:(" + rta + "), toComp:(" + rtc + "), loc(" + p + "), ratio(" + FString::SanitizeFloat(RayHitRatio) + ")";
		return s;
	}
};

/* ___________________ #FCollisionList ___________________ */
USTRUCT()
struct OMCEM_API FCollisionList
{
	GENERATED_USTRUCT_BODY()

		TArray<FContactResult*> List;
	FCollisionList() {}
	void Add(FContactResult* _collInfo);
	void Remove(FContactResult* _collInfo);
	bool GetIsValid();
	bool Check(UOmComp* _otherOmBody);
	FContactResult* GetCollision(UPrimitiveComponent* _otherOmBody);
};




typedef TPair<FTransform, FTransform> PairTrans;

/* <---------- Status -------------> */
UENUM(BlueprintType)
enum class EStatus : uint8
{
	ON_PRE_START,
	ON_STARTED,
	ON_PROGRESS,
	ON_PROGRESS_ERR,
	ON_ENDED,
	ON_COMPLETED,
	ON_COMPLETED_FAIL,
	ON_COMPLETED_SUCC
};

/* <---------- EquipmentCategory -------------> */
UENUM(BlueprintType)
enum class EEquipmentCat : uint8
{
	SCREW,
	ROD,
	NUT,
	CAGE

};

/* <---------- DataEqMenuItem -------------> */
USTRUCT()
struct OMCEM_API FDataEqItem
{
	GENERATED_USTRUCT_BODY()
		EEquipmentCat Cat;
	FString IdCat;
	class UTexture2D* ClassThumb;
	FDataEqItem()
	{

	};
	FDataEqItem(FString _menuItemId, class UTexture2D* _ClassThumb)
	{
		ClassThumb = _ClassThumb;
		IdCat = _menuItemId;
		IdCat.ToLowerInline();
		if (IdCat == "screw")
			Cat = EEquipmentCat::SCREW;
		else if (IdCat == "rod")
			Cat = EEquipmentCat::ROD;
		else if (IdCat == "nut")
			Cat = EEquipmentCat::NUT;
		else if (IdCat == "cage")
			Cat = EEquipmentCat::CAGE;

	};
};




