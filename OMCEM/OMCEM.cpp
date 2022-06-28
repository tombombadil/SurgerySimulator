// Fill out your copyright notice in the Description page of Project Settings.

#include "OMCEM.h"
#include "OmEngine/Controllers/OmHUD.h"
#include "OmEngine/Controllers/MainCont.h"
#include <ModuleManager.h>

IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, OMCEM, "OMCEM");
Debug::Debug(FString _msg, int8 _level)
{
	if (AMainCont::INS->DebugLogLevel >= _level)
		UE_LOG(LogTemp, Warning, TEXT("%s"), *_msg);
}
Debug::Debug(FString _label, float _float, int8 _level)
{
	FString _msg = _label.Len() > 0 ? _label + "-> " : "";
	Debug(_msg + *FString::SanitizeFloat(_float), _level);
}

Debug::Debug(FString _label, int _int, int8 _level)
{
	FString _msg = _label.Len() > 0 ? _label + "-> " : "";
	Debug(_msg + *FString::FromInt(_int), _level);
}

Debug::Debug(FString _label, bool _bool, int8 _level)
{
	FString _msg = _label.Len() > 0 ? _label + "-> " : "";
	FString _strBool = _bool ? "true" : "false";
	Debug(_msg + _strBool, _level);
}



void Debug::Warning(FString _msg)
{
	FString m = "#OMCEMERR -> " + _msg;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *m);
	//EventOnWarnig.Broadcast(_msg);
	AMainCont::INS->Hud->ShowMessage(_msg);
}
