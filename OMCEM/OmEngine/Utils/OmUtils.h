#pragma once

#include <vector>
#include "Config.h"
#include "OmStructs.h"
#include "OmEngine/Utils/CoreServer.h"
#include "OmUtils.generated.h"

class UOmComp;
class AItem;
typedef TPair<FString, TArray<FString>> OmCommandParam;
UCLASS(Blueprintable)
class OMCEM_API UOmUtils : public UObject
{
	GENERATED_BODY()
	
public:
	static TArray<FHitResult>  RayCast(bool _isMulti, EChannels _channel, FVector _from, FVector _to, int8 _showLine, FColor _lineColor);
	static void DrawLine(FVector _from, FVector _to, FColor _color, bool _persistance = false, bool _forceDebugLine = false);
	template <typename EnumType> static  TPair<bool, EnumType> GetEnumValueFromString(const FString& EnumName, const FString& String);
	static void DrawPoint(FVector _point, FColor _color, bool _persistance = false);
	static OmCommandParam ParseCommand(FString _command);
	static FTransform GetSubtractTrans(FTransform _trans1, FTransform _trans2);
	static FTransform GetAdditionTrans(FTransform _trans1, FTransform _trans2);
	static FString	FVectorToOmValueStr(FVector _vec);
	static FVector OmValueStrToFVector(FString _strData, FString _valueName);
	static	float OmValueStrToFloat(FString _strData, FString _valueName);
	static FRotator GetAverageRot(FRotator _min, FRotator _max);
	static FRotator	GetLimitedRotator(FRotator _origin, FRotator _limit);
	static TArray<FString> ParseString(FString _str, FString _delim, bool _isCulling=true);
	static TArray<FString> ParseComparison(FString _str, bool _printErr=true);
	static int	ToInt(FString _intStr);
	static float ToFloat(FString _intStr);
	static FString	FromVectorToString(std::vector<FString> _vec);
	static FString FromArrayToString(TArray<FString> _arrStr);
	static	FString IntToString(TArray<int8> _arr);
	static	FString FloatToString(TArray<float> _arr);
	static FString FloatToString(float _float, int8 _precision=-1);
	static	FRotator FVectorToFRotator(FVector _vec);
	template <typename StructType> void GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput);
	template <typename StructType> void GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput);
	static TArray<int8> ToIntArray(TArray<FString> _arrStr);
	static TArray<float> ToFloatArray(TArray<FString> _arrStr);
	// iki vector arasindaki aciyi operasyon icin belirlenen rotasyon vector formatina donusturur (ProcessComp icin yapildi) x(pitch), y(roll),  z(0) 
	static FRotator FromNormalToOperationRot(FVector _vec);
	static FRotator FromNormalToOperationRot(FRotator r);
	static FRotator FromOperationToUeRot(FRotator r);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static FString GetStringFromOmValues(FString _omValues, FString _name, int _element = 0, bool _toLower=true);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static FString GetCommandStrFromOmValues(FString _omValues, int _element=0);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static bool GetBoolFromOmValues(FString _omValues, FString _name, int _element = 0);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static float GetFloatFromOmValues(FString _omValues, FString _name, int _element = 0);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static AItem* GetItemFromOmValues(FString _omValues, FString _name="item", int _element = 0);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static FVector GetVectorFromOmValues(FString _omValues, FString _name, int _element = 0);
	UFUNCTION(BlueprintCallable, Category = OMCEM)	static UOmComp* GetCompFromOmValues(FString _omValues, FString _name="comp", int _element = 0);
	//UFUNCTION(BlueprintCallable, Category = OMCEM)	static FString GetCmdParam(FString _cmd, FString _name, int _cmdIndex, bool _toLower = true);
	//UFUNCTION(BlueprintCallable, Category = OMCEM)	static FString GetCmdParam_Func(FString _cmd,  int _cmdIndex);
	//UFUNCTION(BlueprintCallable, Category = OMCEM) static bool GetCmdParam_Boolean(FString _cmd, FString _name, int _cmdIndex);
	//UFUNCTION(BlueprintCallable, Category = OMCEM) static float GetCmdParam_Float(FString _cmd, FString _name, int _cmdIndex);
	//UFUNCTION(BlueprintCallable, Category = OMCEM)	static	AItem* GetCmdParam_Item(FString _cmd, FString _name, int _cmdIndex);
	//UFUNCTION(BlueprintCallable, Category = OMCEM)	static	FVector GetCmdParam_Vec(FString _cmd, FString _name, int _cmdIndex);
	//UFUNCTION(BlueprintCallable, Category = OMCEM)	static	UOmComp* GetCmdParam_OmComp(FString _cmd, FString _name, int _cmdIndex);
};








