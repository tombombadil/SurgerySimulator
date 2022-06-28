#include "Inputs.h"
#include "OMCEM.h"
#include "OmEngine/Utils/Resource.h"
#include "OmEngine/Controllers/UICont.h"
#include "OmEngine/Controllers/MainCont.h"
#include "OmEngine/Controllers/PlayerCont.h"
#include "OmEngine/Controllers/Process.h"
#include "../Controllers/Observer.h"


UInputs::UInputs():Super()
{


	MoveInputDelta = FVector(0, 0, 0);
	MoveInput = FVector(0, 0, 0);
	RawLoc = FVector(0, 0, 0);
	RotInput = FRotator::ZeroRotator;
	RotInputDelta = FRotator::ZeroRotator;
}

void UInputs::SetEnable(bool _enable)
{
	if (_enable == IsEnable) return;
	IsEnable = _enable;
	if (_enable)
	{
		//APlayerCont::INS->EventKeyAndMouse.AddDynamic(this, &UInputs::onKeyMouseHandler);
			APlayerCont::INS->InputListener->EventAction.AddDynamic(this, &UInputs::onKeyMouseHandler);
	}
	else
	{
		//APlayerCont::INS->EventKeyAndMouse.RemoveDynamic(this, &UInputs::onKeyMouseHandler);
		APlayerCont::INS->InputListener->EventAction.RemoveDynamic(this, &UInputs::onKeyMouseHandler);
	}
}
void UInputs::onKeyMouseHandler(bool _status, FString _key)
{
	
		onButtonHandler(_key, _status);
	}

void UInputs::onButtonHandler(FString btnId, bool status)
{
	if (!AMainCont::INS->IsAppInited) return;



	if (btnId == "BtnHaptic1" && IsHaptic())
	{
		BtnHaptic1 = status;
		UpdateKeyPreset("KeySelectTool", status);
		UpdateKeyPreset("KeyProcess", status);

	}
	if (btnId == "BtnHaptic2" && IsHaptic())
	{
		BtnHaptic2 = status;
		UpdateKeyPreset("KeySelectTool", status);
		UpdateKeyPreset("KeyProcess", status);
	}
	if (btnId == "VrLeftTrigger")
	{
		
		UpdateKeyPreset("KeyProcessSubmit", status);
	}

	if (btnId == "VrRightButtonB")
	{
		if (IsVr())
		{
			UpdateKeyPreset("KeySelectTool", status);
		}

	}
	if (btnId == "VrRightTrigger")
	{
		if (IsVr())
		{
			UpdateKeyPreset("KeyProcess", status);
		}

	}

	if (btnId == "VrRightGrip")
	{



	}


	if (btnId == "MouseLeftButton")
	{
		UpdateKeyPreset("KeyProcess", status);
		UpdateKeyPreset("KeySelectTool", status);
	}
	

	if (btnId == "TestSendAllMetrics")
	{
		//UServer::INS->SendAllMetrics();

	}



	FString btn1Status = BtnHaptic1 ? "1" : "0";
	FString btn2Status = BtnHaptic2 ? "1" : "0";




}

/* create key presets from  onButtonHandler */
void UInputs::UpdateKeyPreset(FString _keyPreset, bool _status)
{
	if (!listKeyPresets.Contains(_keyPreset)) listKeyPresets.Add(_keyPreset, _status);
	else listKeyPresets[_keyPreset] = _status;

	FInputInfo info;
	info.Source =inputSource;
	info.KeyPreset = _keyPreset;
	info.Value = _status ? 1 : 0;
	EventInput.Broadcast(info);


	
}

bool  UInputs::GetKeyPreset(FString _keyPreset)
{
	bool r = false;
	if(listKeyPresets.Contains(_keyPreset))
		r = listKeyPresets[_keyPreset];
	return  r;
}

/* return -> "0" or "1" */
FString  UInputs::GetKeyPresetString(FString _keyPreset)
{
	return FString::FromInt((int)GetKeyPreset(_keyPreset));
}

TMap<FString, bool>  UInputs::GetAllKeyPreset()
{
	
	
	return listKeyPresets;
}

void UInputs::SetSource(EInputSource _source)
{
	
	inputSource = _source;
	if (inputSource == EInputSource::HAPTIC_RIGHT) IdHapticDevice = 0;
	else if (inputSource == EInputSource::HAPTIC_LEFT) IdHapticDevice = 1;
}

EInputSource UInputs::GetSource()
{
	return inputSource;
}




void UInputs::Run(FOmValue cmd)
{
	FString strCmd = cmd.ToString();
	FString func = cmd.GetValue("func").GetString(0);
}

FVector UInputs::getLoc()
{
	return MoveInput;
}

FVector UInputs::getDeltaLoc()
{
	FVector v = FVector::ZeroVector;
	v = MoveInputDelta;
	MoveInputDelta = FVector::ZeroVector;
	return v;
}


FRotator UInputs::getRot()
{

	return RotInput;
}
FRotator UInputs::getDeltaRot()
{
	FRotator v = FRotator::ZeroRotator;
	v = RotInputDelta;
	RotInputDelta = FRotator::ZeroRotator;
	return v;
}

void UInputs::Init()
{
	
	device = UHapticDevice::INS;
	IsEnableHaptic = device->GetIsEnableDevice(IdHapticDevice);
	isInited = true;
	
}
void UInputs::Update(float DeltaTime)
{
	if(IsVr())
		updateVr(DeltaTime);
	else if(IsHaptic())
		updateHaptic(DeltaTime);
	else  if (inputSource == EInputSource::MOUSE)
		updateMouse(DeltaTime);
}

void UInputs::updateMouse(float DeltaTime)
{
	
	


	if (APlayerCont::GetKeyAction("KeySpace")) return;

	float mouseMoveX = APlayerCont::GetKeyAxis("MouseX");
	float mouseMoveY = APlayerCont::GetKeyAxis("MouseY");
	float mouseMoveZ = APlayerCont::GetKeyAxis("MouseZ");


	if (APlayerCont::GetKeyAction("KeyAlt"))
	{
		RotInputDelta.Roll = mouseMoveY * -1;
		RotInputDelta.Pitch = mouseMoveX * -1;
		RotInput += RotInputDelta;
	}
	else
	{
		float speedZ = BtnHaptic1 ? 0.1 : 1;
		MoveInputDelta.Y = FMath::Clamp<float>(mouseMoveY, -1.0f, 1.0f) ;
		MoveInputDelta.X = FMath::Clamp<float>(mouseMoveX, -1.0f, 1.0f) ;
		MoveInputDelta.Z = FMath::Clamp<float>(mouseMoveZ, -1.0f, 1.0f)  * speedZ;
		float speed = APlayerCont::GetKeyAction("KeyShift") ? 2 : 1;
		MoveInputDelta *= speed;
	}


	if (APlayerCont::GetKeyAction("KeyCtrl")) StickRotation -= 1.2f * mouseMoveX;
	else
	{
		
		MoveInput += MoveInputDelta ;
		RawLoc = MoveInput;
	}

	
}



void UInputs::updateHaptic(float DeltaTime)
{
	TArray<int> dllStatus = device->dll_GetStatus(IdHapticDevice);
	if (dllStatus[0] > 0)
	{
		FString printMsg = device->dll_GetString(IdHapticDevice, "print");
		Debug("____________ [ Haptic HLog ]-> " + printMsg,1);
	}
	if (!IsEnableHaptic || !isInited) return;
	bool _pressBtn1 = (dllStatus[2] == 1);
	bool _pressBtn2 = (dllStatus[3] == 1);
	FString _keyIdBtn1 = FString("HapticBtn1");
	FString _keyIdBtn2 = FString("HapticBtn2");
	if (!BtnHaptic1 && _pressBtn1)
	{
		onButtonHandler("BtnHaptic1", true);
	}
	if (BtnHaptic1 && !_pressBtn1)
	{
		onButtonHandler("BtnHaptic1", false);
	}


	if (!BtnHaptic2 && _pressBtn2)
	{
		onButtonHandler("BtnHaptic2", true);
	}
	if (BtnHaptic2 && !_pressBtn2)
	{
		onButtonHandler("BtnHaptic2", false);
	}



	RawLoc = device->GetLoc(IdHapticDevice);
	FVector rawRot = device->GetRot(IdHapticDevice);
	TArray<float> arrTransform = device->dll_GetArray(IdHapticDevice, "rotation", 16);

	StickRotation = arrTransform[2];
	if (isInited)
	{
		//MoveInput = AMainCont::INS->HapticCenterPos + ((RawLoc - pivotOfHaptic)* device->PositionUnitScale);
		FVector newLoc = UHapticDevice::INS->ConvertHapticToUnreal(IdHapticDevice, RawLoc);
			MoveInputDelta = MoveInput - newLoc;
			MoveInput = newLoc;

		FRotator newRot = FRotator::ZeroRotator;
		newRot.Pitch = rawRot.Z * -1;
		newRot.Roll = (rawRot.X) + 90;
		RotInputDelta = (newRot - RotInput);
		RotInput = newRot;

	}


}


void UInputs::updateVr(float DeltaTime)
{
	if (!VrMotinComp) return;
	FVector loc = VrMotinComp->GetComponentLocation();
	FRotator rot = VrMotinComp->GetComponentRotation();
	RotInputDelta = (rot - RotInput);
	RotInput = rot;


	{
		
		MoveInput = loc;
		RawLoc = MoveInput;
	}


}




void UInputs::AddForce(EForceType _type, FVector _force)
{
	if (!AMainCont::INS->IsAppStarted || !IsHaptic() || !IsEnableHaptic) return;
	int _index = (int)_type;
	int resultForce = device->dll_ApplyForce(IdHapticDevice, _index, _force);
	
}
void UInputs::AddForce(EForceType _type, FVector _direction, float _magnitude)
{
	int _index = (int)_type;
	FVector f = _direction.GetSafeNormal()  *  _magnitude;
	AddForce(_type, f);
}


//void UInputs::SetJointForceArea(bool _isEnable, FVector _jointPos, FVector _boxExtends, float _influence, float _stiffness)
//{
//	if (!AMainCont::INS->IsAppStarted || !AMainCont::INS->IsHapticMode) return;
//	FVector jointPos = device->ConvertUnrealToHaptic(IdHapticDevice, _jointPos);
//	device->SetJoint(IdHapticDevice, _isEnable, jointPos, _boxExtends, _influence, _stiffness);
//	
//}
/* vibrate */
void UInputs::SetVibrate(bool _isEnable, float _amplitude, float _freq)
{
	if(!AMainCont::INS->IsAppStarted || !IsHaptic() || !IsEnableHaptic) return;
	int resultForce = device->dll_SetEnableVib(IdHapticDevice, _isEnable, _amplitude, _freq);

}




bool UInputs::IsHaptic()
{
	return (inputSource == EInputSource::HAPTIC_RIGHT || inputSource == EInputSource::HAPTIC_LEFT);

}

bool UInputs::IsVr()
{
	return (inputSource == EInputSource::VR_LEFT || inputSource == EInputSource::VR_RIGHT);

}





