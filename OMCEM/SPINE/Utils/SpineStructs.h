#pragma once

#include <vector>
#include "SpineStructs.generated.h"


USTRUCT(BlueprintType)
struct OMCEM_API FSpineHoleData
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(BlueprintReadWrite) FString IdBone;
		UPROPERTY(BlueprintReadWrite) FString IdDirection;
		UPROPERTY(EditAnywhere, BlueprintReadOnly) FRotator MinAngles;
		UPROPERTY(EditAnywhere, BlueprintReadOnly) FRotator MaxAngles;
		UPROPERTY(EditAnywhere, BlueprintReadOnly) float MinDepth;
		UPROPERTY(EditAnywhere, BlueprintReadOnly) float MaxDepth;
		UPROPERTY(EditAnywhere, BlueprintReadOnly) FString IdScrew;
		UPROPERTY(BlueprintReadOnly) FString IdHole;
		UPROPERTY() class ASpineHoleField* Hole;

		FString ToString()
		{
			return  "IdHole:(" + IdHole + "),"
				+ "IdBone(" + IdBone + "),"
				+ "IdDirection:(" + IdDirection + "),"
				+ "IdScrew:(" + IdScrew + "),"
				+ "MinAngles:(" + MinAngles.ToString() + "),"
				+ "MaxAngles:(" + MaxAngles.ToString() + "),"
				+ "MinDepth:(" + FString::SanitizeFloat(MinDepth) + "),"
				+ "MaxDepth:(" + FString::SanitizeFloat(MaxDepth) + "),";
		}
};

/*_____________________ Spine Operation Data _____________________ */
/* FSpineData operation.txt json  Operation node'u ile ayný yapida olmali */
USTRUCT(BlueprintType)
struct OMCEM_API FSpineData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString IdOperation; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString NameOperation = "Insert Screw";
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString IdSection; //Cervical,Thoracic,Lumbar
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString NameSection = "Lumbar";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)  TArray<FSpineHoleData> ListBones;
		
		 

	FSpineData() {};
	FSpineData(FString _IdOperation) {};
	//FSpineInit(FString _idOperarition, AToolItem* _toolEntry, AToolItem* _toolMakeHole, 	AToolItem* _toolProbe, 	AToolItem* _toolScrew, AToolItem* _screw, FString _label);

};



/* <---------- Status -------------> */
//UENUM(BlueprintType)
//enum class EStatus : uint8
//{
//	ON_PRE_START,
//	ON_STARTED,
//	ON_PROGRESS,
//	ON_PROGRESS_ERR,
//	ON_ENDED,
//	ON_COMPLETED,
//	ON_COMPLETED_FAIL,
//	ON_COMPLETED_SUCC
//};
