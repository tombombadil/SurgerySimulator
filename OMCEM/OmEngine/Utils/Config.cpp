#include "Config.h"
#include "OMCEM.h"
#include "Resource.h"
#include "OmEngine/Controllers/MainCont.h"




FVector UConfig::PAWN_AFTER_TOOL_POSITION_OFFSET = FVector(0, 0, 100);
vector<float> UConfig::INPUT_MOUSE_MOVE_SPEED_XY = { 1000,1400}; //normal,with shift
vector<float> UConfig::INPUT_MOUSE_MOVE_SPEED_Z = { 900, 5000, }; //normal,with shift
vector<float> UConfig::INPUT_CAM_ROTATION_OFFSET_LIMIT_X = { -90,90 };
vector<float> UConfig::INPUT_CAM_ROTATION_OFFSET_LIMIT_Y = { -20,100 };

float UConfig::OBSERVER_PVALUE_TOL = 1.0f;
float UConfig::CAM_MAIN_ZOOM_SPEED = 0.05;
float UConfig::INPUT_MOUSE_WHEEL_ZOOM_SPEED = 400;
FRotator UConfig::RIGHT_OP_TOOL_MOVE_ROT = FRotator(10,-90,0);
FRotator UConfig::LEFT_OP_TOOL_MOVE_ROT = FRotator(10,90,0);

FRotator UConfig::OPERATION_ROTATION_OFFSET = FRotator(90, -90, 0);





UConfig * UConfig::INS = NULL;


UConfig::UConfig()
{
	UConfig::INS = this;

	//listOperations.Add(EOperation::TLIF, FOperation("tlif"));
	//listOperations.Add(EOperation::CERVICAL, FOperation("cervical"));
	//listOperations.Add(EOperation::SCOLIOSIS, FOperation("scoliosis"));
}




void UConfig::Init()
{
	Super::Init();

	
}




FString UConfig::GET_MSG(FString _valueId)
{
	FString msg = UResource::INS->GetJsonString(UResource::INS->GetMainJsonObj("Messages"), _valueId);
	
	return msg;
}



AMainCont* UConfig::GetMainController()
{
	AMainCont* r = 0;
	if (AMainCont::INS)  r = AMainCont::INS;
	return r;
}

UResource* UConfig::GetResource()
{
	UResource* r = 0;
	if (UResource::INS) r = UResource::INS;
	return r;
}


//FOperation UConfig::GetOperation(EOperation _operation)
//{
//	return listOperations[_operation];
//}

void UConfig::AddDebugParam(FString _paramName, FString _param)
{
	if (listDebugParams.Contains(_paramName))
		listDebugParams[_paramName] = _param;
	else
		listDebugParams.Add(_paramName, _param);

	Debug(_paramName + " : " + _param,0);
}

FString UConfig::PrintDebugParams()
{
	FString s = "";
	for (auto& Elem : listDebugParams)
	{
		s.Append(Elem.Key + " -> " + Elem.Value);
		s.Append("\n");

	}
	
	return s;
}