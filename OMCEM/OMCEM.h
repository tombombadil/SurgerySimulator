#pragma once
#include <UnrealString.h>
#include <CoreGlobals.h>


class  Debug
{
public:

	//static FEventOnWarning EventOnWarnig;
	Debug(FString _msg, int8 _level = 0);

	Debug(FString _label, float _float, int8 _level = 0);
	Debug(FString _label, int _int, int8 _level = 0);
	Debug(FString _label, bool _bool, int8 _level = 0);



	static void Warning(FString _msg);





};

//
//class  Log
//{
//public:
//
//	/****************[ UE_LOG ]*********************/
//
//	Log(FString _str)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("%s"), *_str);
//	}
//
//
//	Log(FString _label, float _float)
//	{
//		Debug(_label, _float);
//	}
//	Log(FString _label, int _int)
//	{
//		Debug(_label, _int);
//	}
//	Log(FString _label, bool _bool)
//	{
//		Debug(_label, _bool);
//	}
//
//
//
//
//
//
//
//};
