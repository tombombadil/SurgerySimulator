#include "OmUtils.h"
#include "OmValue.h"
#include "OMCEM.h"
#include "OmEngine/Components/OmComp.h"
#include "../Items/Item.h"
#include "OmEngine/Controllers/MainCont.h"
#include <DrawDebugHelpers.h>





/*____________________________________________________
___________________    #STRUCT    ___________________
___________________ COLLISION LIST __________________
____________________________________________________
*/
void FCollisionList::Add(FContactResult* _collInfo)
{
	List.Add(_collInfo);
}
void FCollisionList::Remove(FContactResult* _collInfo)
{
	List.Remove(_collInfo);
}

bool FCollisionList::Check(UOmComp* _other)
{
	bool _result = 0;
	if (List.Num() > 0)
	{
		for (FContactResult* info : List)
		{
			if (info->toComp == _other)
				_result = true;
		}
	}
	return _result;
}
bool FCollisionList::GetIsValid()
{
	return (List.Num() > 0);
}


//FContactResult* FCollisionList::GetCollision(UOmComp* _other)
FContactResult* FCollisionList::GetCollision(UPrimitiveComponent* _other)
{
	FContactResult* _result = 0;
	if (List.Num() > 0)
	{
		for (FContactResult* info : List)
		{
			if (info->toComp == _other)
				_result = info;
		}
	}
	return _result;
}


TArray<FHitResult>  UOmUtils::RayCast(bool _isMulti, EChannels _channel, FVector _from, FVector _to, int8 _showLine, FColor _lineColor)
{
	TArray<FHitResult> result;
	FCollisionQueryParams RV_TraceParams ;

	RV_TraceParams.bTraceComplex = true;
	//RV_TraceParams.bTraceAsyncScene = true;
	
	uint8 tracesIndex = (uint8)_channel;
	FString channelName = "ECC_GameTraceChannel" + FString::FromInt(tracesIndex + 1);
	TPair<bool, ECollisionChannel> findChannel = UOmUtils::GetEnumValueFromString<ECollisionChannel>("ECollisionChannel", channelName);
	if (findChannel.Key)
	{
		if (_isMulti)
		{
			AMainCont::INS->GetWorld()->LineTraceMultiByChannel(result, _from, _to, findChannel.Value, RV_TraceParams);
		}
		else
		{
			FHitResult resultSingle;

			if (AMainCont::INS->GetWorld()->LineTraceSingleByChannel(resultSingle, _from, _to, findChannel.Value, RV_TraceParams))
			{
					result.Add(resultSingle);
			}
		
		}
			
		if (_showLine)
		{
			DrawLine(_from, _to, _lineColor, false, _showLine > 1);
		}
	}

	
	return result;
}

template <typename EnumType>
 TPair<bool, EnumType> UOmUtils::GetEnumValueFromString(const FString& EnumName, const FString& String)
{
	TPair<bool, EnumType> result;
	UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
	if (Enum)
	{
		result.Key = true;
		result.Value = (EnumType)Enum->GetIndexByName(FName(*String));
	}
	else
	{
		result.Key = false;
	}
	return result;
}


void UOmUtils::DrawLine(FVector _from, FVector _to, FColor _color, bool _persistance, bool _forceDebugLine)
{
	if (AMainCont::INS->DebugMode < 2 && !_forceDebugLine) return;
	_color =  _color.WithAlpha(64);
	DrawDebugLine(
		AMainCont::INS->GetWorld(),
		_from,
		_to,
		_color,
		_persistance, -1, 0,
		0.05f
	);
}

void UOmUtils::DrawPoint(FVector _point, FColor _color, bool _persistance)
{
	if (AMainCont::INS->DebugMode < 2) return;
	DrawDebugPoint(
		AMainCont::INS->GetWorld(),
		_point,
		1,
		_color,
		_persistance, -1, 0
	);
}



OmCommandParam UOmUtils::ParseCommand(FString _command)
{
	OmCommandParam _result;
	FString _func;
	TArray<FString> _params;



	if (_command.Len() > 0)
	{
		TArray<FString> _parseArr;
		_command.ParseIntoArrayWS(_parseArr, TEXT(":"));
		if (_parseArr.Num() > 0)
		{
			_func = _parseArr[0];
			_result.Key = _func;
			if (_parseArr.Num() > 1)
			{
				
				TArray<FString> _arrParams;
				_parseArr[1].ParseIntoArrayWS(_arrParams, TEXT(","));
				_result.Value = _arrParams;
				
			}
		}
	}



	return _result;

}

FTransform UOmUtils::GetSubtractTrans(FTransform _trans1, FTransform _trans2)
{
	FTransform _result;
	_result.SetLocation(_trans2.GetLocation() - _trans1.GetLocation());
	_result.SetRotation(_trans2.GetRotation() - _trans1.GetRotation());
	return _result;
}

FTransform UOmUtils::GetAdditionTrans(FTransform _trans1, FTransform _trans2)
{
	FTransform _result;
	_result.SetLocation(_trans1.GetLocation() + _trans2.GetLocation());
	_result.SetRotation(_trans1.GetRotation() + _trans2.GetRotation());
	return _result;
}


FString UOmUtils::FVectorToOmValueStr(FVector _vec) 
{
	FString _x = FString::FromInt(FMath::RoundToInt(_vec.X));
	FString _y = FString::FromInt(FMath::RoundToInt(_vec.Y));
	FString _z = FString::FromInt(FMath::RoundToInt(_vec.Z));
	FString _result = _x + "," + _y + "," + _z;
	return _result;
}
FVector UOmUtils::OmValueStrToFVector(FString _strData, FString _valueName)
{
	FVector v = FVector::ZeroVector;
	FOmValue omV = FOmValue(_strData);
	if (omV.GetValue(_valueName).IsValid)
		v = omV.GetValue(_valueName).GetFVector();
	return v;
}
float UOmUtils::OmValueStrToFloat(FString _strData, FString _valueName)
{
	float v = 0;
	FOmValue omV = FOmValue(_strData);
	if (omV.GetValue(_valueName).IsValid)
		v = omV.GetValue(_valueName).GetFloat(0);
	return v;
}

FRotator UOmUtils::GetAverageRot(FRotator _min, FRotator _max)
{
	FRotator _result;
	_result.Roll = _min.Roll + ((_max.Roll -_min.Roll) / 2);
	_result.Pitch = _min.Pitch + ((_max.Pitch -_min.Pitch) / 2);
	_result.Yaw = _min.Yaw + ((_max.Yaw -_min.Yaw) / 2);
	return _result;
}
FRotator UOmUtils::GetLimitedRotator(FRotator _origin, FRotator _limit)
{
	FRotator _result;
	_result.Pitch = FMath::Clamp(_origin.Pitch, -_limit.Pitch, _limit.Pitch);
	_result.Yaw = FMath::Clamp(_origin.Yaw, -_limit.Yaw, _limit.Yaw);
	_result.Roll = FMath::Clamp(_origin.Roll, -_limit.Roll, _limit.Roll);
	return _result;
}

TArray<FString> UOmUtils::ParseString(FString _str, FString _delim, bool _isCulling)
{
	TArray<FString> _parseArr;
	_str.ParseIntoArray(_parseArr, *_delim, _isCulling);
	return _parseArr;
}



/* _str example  -> "(key1==value1)&&(key1==value1)"*/
TArray<FString> UOmUtils::ParseComparison(FString _str, bool _printErr/*=true*/)
{
	TArray < FString > r;
	if (_str.IsValidIndex(0))
	{

		const TCHAR* Delims[] = { TEXT("("), TEXT(")") };
		TArray<FString> Parsed;
		_str.ParseIntoArray(Parsed, Delims, 2);
		for (int8 i = 0; i < Parsed.Num(); i++)
		{
			FString s = Parsed[i];
			if (i % 2 == 1)
			{
				if (!(s == "&&" || s == "||"))
				{
					//to-do error log
				}
			}
		
		
			r.Add(s);
			
				
					
		}
		
	}
	return r;
}

int UOmUtils::ToInt(FString _intStr)
{
	return (int)FCString::Atoi(*_intStr);
}

float UOmUtils::ToFloat(FString _intStr)
{
	return (float)FCString::Atof(*_intStr);
}


TArray<int8> UOmUtils::ToIntArray(TArray<FString> _arrStr)
{
	TArray<int8> _arr;
	for (FString s : _arrStr)
	{
		TArray<FString> listRange = ParseString(s, "/");
		if (listRange.Num() > 0)
		{
			_arr.Add(ToInt(listRange[0]));
			_arr.Add(ToInt(listRange[1]));
		}
		else
		{
			_arr.Add(ToInt(s));
		}
		
	}
	return _arr;
}


TArray<float> UOmUtils::ToFloatArray(TArray<FString> _arrStr)
{
	TArray<float> _arr;
	for (FString s : _arrStr)
	{
		TArray<FString> listRange = ParseString(s, "/");
		if (listRange.Num() > 0)
		{
			_arr.Add(ToFloat(listRange[0]));
			_arr.Add(ToFloat(listRange[1]));
		}
		else
		{
			_arr.Add(ToFloat(s));
		}

	}
	return _arr;
}

FRotator UOmUtils::FromNormalToOperationRot(FVector v)
{
	float y = FMath::Acos(FVector::DotProduct(FVector(0, v.Y, v.Z).GetSafeNormal(), FVector(0, 1, 0).GetSafeNormal()));
	y = FMath::RadiansToDegrees(y) - 90;
	float x = FMath::Acos(FVector::DotProduct(FVector(v.X, 0, v.Z).GetSafeNormal(), FVector(1, 0, 0).GetSafeNormal())) * -1 ;
	x = FMath::RadiansToDegrees(x) ;
	return FRotator(x, 0, y);
}

FRotator UOmUtils::FromNormalToOperationRot(FRotator r)
{
	r.Pitch -= 90;
	return r;
}
FRotator UOmUtils::FromOperationToUeRot(FRotator r)
{
	r.Pitch += 90;
	return r;
}



/* ------------------------ parse omvalues ---------------------- */
FString UOmUtils::GetStringFromOmValues(FString _omValues, FString _name, int _element, bool _toLower)
{
	FString r;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
		r = omValues.GetOmValue(_element).GetValue(_name).GetString(0);
	r = _toLower ? r.ToLower() : r;
	return r;
}
FString UOmUtils::GetCommandStrFromOmValues(FString _omValues,  int _element)
{
	FString r;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
		r = GetStringFromOmValues(_omValues, "func", _element, true);
	return r;
}
bool UOmUtils::GetBoolFromOmValues(FString _omValues, FString _name, int _element)
{
	bool r = false;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
		r = omValues.GetOmValue(_element).GetValue(_name).GetBool(0);
	return r;
}
float UOmUtils::GetFloatFromOmValues(FString _omValues, FString _name, int _element)
{
	float r = 0;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
		r = omValues.GetOmValue(_element).GetValue(_name).GetFloat(0);
	return r;
}

FVector UOmUtils::GetVectorFromOmValues(FString _omValues, FString _name, int _cmdIndex)
{
	FVector r = FVector::ZeroVector;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
		r = omValues.GetOmValue(_cmdIndex).GetValue(_name).GetFVector(false);
	return r;
}


AItem* UOmUtils::GetItemFromOmValues(FString _omValues, FString _name, int _element)
{
	AItem* r = 0;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
	{
		FString idItem = GetStringFromOmValues(_omValues, _name, _element);
		r = UResource::INS->GetItem(idItem);
	}
	return r;
}

UOmComp* UOmUtils::GetCompFromOmValues(FString _omValues, FString _name, int _element)
{
	UOmComp* r = 0;
	FOmValues omValues = FOmValues(_omValues);
	if (omValues.IsValid)
	{
		FString idComp = GetStringFromOmValues(_omValues, _name, _element);
		r = UResource::INS->GetOmComp(idComp);
	}
	return r;
}


FString UOmUtils::FromVectorToString(std::vector<FString> _vec)
{
	FString str = "";
	for (int8 i = 0; i < (int)_vec.size(); i++)
	{
		str.Append(_vec[i]);
		if (i < _vec.size() - 1) str.Append(",");
	}
	return str;
}


FString UOmUtils::FromArrayToString(TArray<FString> _arrStr)
{
	FString str = "";
	for (int8 i = 0; i < _arrStr.Num(); i++)
	{
		str.Append(_arrStr[i]);
		if (i < _arrStr.Num() - 1) str.Append(",");
	}
	return str;
}

FString UOmUtils::IntToString(TArray<int8> _arr)
{
	FString str = "";
	int8 _total = _arr.Num();
	for (int8 i = 0; i < _total; i++)
	{
		str.Append(FString::FromInt(_arr[i]));
		if (i < _total - 1) str.Append(",");
	}
	return str;
}

FString UOmUtils::FloatToString(TArray<float> _arr)
{
	FString str = "";
	int8 _total = _arr.Num();
	for (int8 i = 0; i < _total; i++)
	{
		str.Append(FString::SanitizeFloat(_arr[i]));
		if (i < _total - 1) str.Append(",");
	}
	return str;
}

FString UOmUtils::FloatToString(float _float, int8 _precision)
{
	FNumberFormattingOptions NumberFormat;					//Text.h
	NumberFormat.MinimumIntegralDigits = 1;
	NumberFormat.MaximumIntegralDigits = 10000;
	if (_precision > -1)
	{
		NumberFormat.MinimumFractionalDigits = _precision;
		NumberFormat.MaximumFractionalDigits = _precision;
	}
	
	FString str = FText::AsNumber(_float, &NumberFormat).ToString();


	//FString str =  FString::SanitizeFloat(_float);
	return str;
}


FRotator UOmUtils::FVectorToFRotator(FVector _vec)
{
	FRotator r = FRotator(_vec.Y, _vec.Z, _vec.X);
	return r;
}

class UOmComp* FContactResult::GetFromComp()
{
	return Cast< UOmComp>(fromComp);
}

AItem* FContactResult::GetFromItem()
{
	return Cast<AItem>(fromActor);
}

class UOmComp* FContactResult::GetToComp()
{
	return Cast<UOmComp>(toComp);
}

AItem* FContactResult::GetToItem()
{
	return Cast<AItem>(toActor);
}





template <typename StructType>
void UOmUtils::GetJsonStringFromStruct(StructType FilledStruct, FString& StringOutput) {
	FJsonObjectConverter::UStructToJsonObjectString(StructType::StaticStruct(), &FilledStruct, StringOutput, 0, 0);

}

template <typename StructType>
void UOmUtils::GetStructFromJsonString(FHttpResponsePtr Response, StructType& StructOutput) {
	StructType StructData;
	FString JsonString = Response->GetContentAsString();
	FJsonObjectConverter::JsonObjectStringToUStruct<StructType>(JsonString, &StructOutput, 0, 0);
}
