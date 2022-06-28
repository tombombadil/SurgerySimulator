#include "HapticDevice.h"
#include "OMCEM.h"
#include <string>
#include "../Controllers/MainCont.h"


UHapticDevice* UHapticDevice::INS;

using namespace std;


typedef int(*_getInit)(bool _isMultipleDevice);
typedef int(*_applyForce)(int _device, float* forceVec, int index);
typedef int(*_setJointForceArea)(int _device, bool isEnable, float* jointPos, float* area, float influance, float stifness);
typedef int(*_setEnableVib)(int _device, bool isEnable, float amplitude, float freq);
typedef int*(*_getStatus)(int device);
typedef float*(*_getArray)(int _device, char* valueName);
typedef char*(*_getString)(int _device, char* valueName);
typedef string*(*_getStr)(int _device, string* valueName);
typedef int(*_getDestroy)();

_getInit m_getInit;
_applyForce m_applyForce;
_setJointForceArea m_setJointForceArea;
_setEnableVib m_setEnableVib;
_getStatus m_getStatus;
_getArray m_getArray;
_getString m_getString;
_getStr m_getStr;
_getDestroy m_getDestroy;

void *v_dllHandle;


UHapticDevice::UHapticDevice():Super()
{
	PositionUnitScale = FVector(0.35, 0.35, 0.35);
}


FVector UHapticDevice::GetLoc(int _idDevice)
{

	return convertToFVector(dll_GetArray(_idDevice, "position", 3));
}
FVector UHapticDevice::GetRot(int _idDevice)
{
	return convertToFVector(dll_GetArray(_idDevice, "rotation", 3));
}
bool UHapticDevice::GetIsEnableDevice(int _idDevice)
{
	bool r = 0;
	if (dll_GetStatus(_idDevice).IsValidIndex(4))
		 r = (bool)dll_GetStatus(_idDevice)[4];
	return r;
}


bool UHapticDevice::UpdatePivotOfHaptic(int _idDevice, FVector _startPosition)
{
	if (!IsEnable) return false;
	PivotOfHaptic = convertToFVector(dll_GetArray(_idDevice, "position", 3));
	if (startPositions.Contains(_idDevice))
	{
		startPositions[_idDevice] = _startPosition;
	}
	else
	{
		startPositions.Add(_idDevice, _startPosition); 
	}
	
	
	//isInited = true;
	return true;

}

void UHapticDevice::Init(bool _isMultipleDevice)
{
	UHapticDevice::INS = this; 
	startPositions.Add(0, FVector::ZeroVector);
	startPositions.Add(1, FVector::ZeroVector);
	isEnableDll = importDLL("Haptic", "HapticApi.dll");
	if (isEnableDll)
	{
		importMethod("GetString");
		bool isImportMet = importMethod("Init");
		if (isImportMet)
		{
			importMethod("GetStatus");
			importMethod("GetArray");
			
			//importMethod("GetStr");
			importMethod("ApplyForce");
			importMethod("SetJointForceArea");
			importMethod("SetEnableVib");
			importMethod("Destroy");
			int initStatus = dll_init(_isMultipleDevice);
			if (initStatus > 0) {
				//AMainCont::INS->HapticCenterPos -= convertToFVector(dll_GetArray(0, "position", 3));
				IsEnable = true;
			}


		}

	}
	
}

void UHapticDevice::SetEnable(bool _enable)
{
	if (_enable)
	{
		
	}
	else
	{
		
	}
}





void UHapticDevice::Update(float DeltaTime)
{

	

}

void UHapticDevice::SetJoint(int _idDevice, bool _isEnable, FVector _jointPos, FVector _boxExtends, float _influence, float _stiffness)
{
	int r = 0;

	if (m_setJointForceArea != NULL)
	{
		//Debug("to dll SetJoint ->  _isEnable("+FString::FromInt(_isEnable)+"),_jointPos:("+ _jointPos.ToString()+"),_boxExtends:("+ _boxExtends.ToString()+"),_influence:("+ FString::FromInt(_influence) +"),_stiffness:("+ FString::FromInt(_stiffness) +")" );
		float* pos = convertToCArray(_jointPos);
		float* box = convertToCArray(_boxExtends);
		r = m_setJointForceArea(_idDevice, _isEnable, pos, box, _influence, _stiffness);
	}
}







FVector UHapticDevice::ConvertHapticToUnreal(int8 _idDevice, FVector _hapticWorldLoc)
{
	FVector v = FVector::ZeroVector;
	 v =  startPositions[_idDevice] + ((_hapticWorldLoc - PivotOfHaptic)* PositionUnitScale);
	return  v;
}
FVector UHapticDevice::ConvertUnrealToHaptic(int8 _idDevice, FVector _unrealWorldLoc)
{
	FVector v = FVector::ZeroVector;
	 v = ((_unrealWorldLoc - startPositions[_idDevice]) / PositionUnitScale) + PivotOfHaptic;
	return  v;
}




void UHapticDevice::Destroy()
{
	if (IsEnable)
	{
		int s = dll_destroy();
	}
}



FVector  UHapticDevice::convertToFVector(TArray<float> _array)
{
	FVector v = FVector(0, 0, 0);
	if (_array.Num() > 0) 	v.X = _array[0];
	if (_array.Num() > 1)	v.Z = _array[1];
	if (_array.Num() > 2) 	v.Y = _array[2];
	return v;

}

float*  UHapticDevice::convertToCArray(FVector _fvector)
{
	float* f = new float[3];
	f[0] = _fvector.X;
	f[1] = _fvector.Z;
	f[2] = _fvector.Y;
	return f;
}



#pragma region Load DLL

// Method to import a DLL.
bool UHapticDevice::importDLL(FString folder, FString name)
{
	FString filePath = *FPaths::ProjectPluginsDir() + folder + "/" + name;

	if (FPaths::FileExists(filePath))
	{
		v_dllHandle = FPlatformProcess::GetDllHandle(*filePath); // Retrieve the DLL.
		if (v_dllHandle != NULL)
		{
			return true;
		}
	}
	return false;	// Return an error.
}
#pragma endregion Load DLL

#pragma region Import Methods


// Import the method init from the DLL.
bool UHapticDevice::importMethod(FString _methodNameFromDll)
{
	if (v_dllHandle != NULL)
	{
		if (_methodNameFromDll == "Init")
		{
			m_getInit = NULL;
			m_getInit = (_getInit)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_getInit != NULL)
			{

				return true;
			}
		}


		else if (_methodNameFromDll == "ApplyForce")
		{
			m_applyForce = NULL;
			m_applyForce = (_applyForce)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_applyForce != NULL)
			{

				return true;
			}
		}
		else if (_methodNameFromDll == "SetJointForceArea")
		{
			m_setJointForceArea = NULL;
			m_setJointForceArea = (_setJointForceArea)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_setJointForceArea != NULL)
			{

				return true;
			}
		}
		else if (_methodNameFromDll == "SetEnableVib")
		{
			m_setEnableVib = NULL;
			m_setEnableVib = (_setEnableVib)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_setEnableVib != NULL)
			{

				return true;
			}
		}

		else if (_methodNameFromDll == "GetArray")
		{
			m_getArray = NULL;
			m_getArray = (_getArray)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_getArray != NULL)
			{

				return true;
			}
		}

		else if (_methodNameFromDll == "GetStatus")
		{
			m_getStatus = NULL;
			m_getStatus = (_getStatus)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_getStatus != NULL)
			{
				return true;
			}
		}

		else if (_methodNameFromDll == "GetString")
		{
			m_getString = NULL;
			m_getString = (_getString)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_getString != NULL)
			{
				return true;
			}

		}
		else if (_methodNameFromDll == "GetStr")
		{
			m_getStr = NULL;
			m_getStr = (_getStr)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_getStr != NULL)
			{
				return true;
			}
		}

		else if (_methodNameFromDll == "Destroy")
		{
			m_getDestroy = NULL;
			m_getDestroy = (_getDestroy)FPlatformProcess::GetDllExport(v_dllHandle, *_methodNameFromDll);
			if (m_getDestroy != NULL)
			{
				return true;
			}
		}

	}
	return false;	// Return an error.
}


int UHapticDevice::dll_init(bool _isMultipleDevice)
{
	if (m_getInit != NULL)
	{
		int out = int(m_getInit(_isMultipleDevice)); // Call the DLL method with arguments corresponding to the exact signature and return type of the method.
		return out;
	}
	return -1;
}


TArray<float> UHapticDevice::dll_GetArray(int _device,FString _charValueName, int _sizeOfArr)
{
	TArray<float> tarr;
	tarr.Init(0, _sizeOfArr);
	if (m_getArray != NULL)
	{
		char* parameterChar = TCHAR_TO_ANSI(*_charValueName);
		float* returnFloatArr = m_getArray(_device, parameterChar);
		tarr = toTArray(returnFloatArr, _sizeOfArr);
	}
	return tarr;
}


int UHapticDevice::dll_ApplyForce(int _device, int _index, FVector _force)
{
	int r = 0;
	if (m_applyForce != NULL)
	{
		//Debug("to dll force -> " + _force.ToString());
		float* parameterForce = convertToCArray(_force);
		r = m_applyForce(_device, parameterForce, _index);
	}
	return r;
}
int UHapticDevice::dll_SetEnableVib(int _device, bool _isEnable, float _amplitude, float _freq)
{
	int r = 0;
	if (m_setEnableVib != NULL)
	{

		r = m_setEnableVib(_device, _isEnable, _amplitude, _freq);
	}
	return r;
}


TArray<int> UHapticDevice::dll_GetStatus(int _device)
{
	TArray<int> tarr;
	if (m_getStatus != NULL)
	{
		int* arr = m_getStatus(_device);
		tarr = toTArray(arr, 16);
	}
	return tarr;
}


FString UHapticDevice::dll_GetStr(int _device, FString _charValueName)
{
	FString s = "";
	if (m_getStr != NULL)
	{
		string param(TCHAR_TO_UTF8(*_charValueName));
		string returnStr = *m_getStr(_device, &param);
		FString returnFStr(returnStr.c_str());
		s = returnFStr;
	}
	return s;
}
FString UHapticDevice::dll_GetString(int _device, FString _charValueName)
{
	FString s = "";
	if (m_getString != NULL)
	{
		char* parameterChar = TCHAR_TO_ANSI(*_charValueName);

		char* returnChar = m_getString(_device, parameterChar);

		s = (ANSI_TO_TCHAR(returnChar));
	}
	return s;
}

TArray<float> UHapticDevice::toTArray(float* _arr, int8 _sizeOfArray)
{
	TArray<float> tarr;
	tarr.Init(0, _sizeOfArray);
	for (int8 i = 0; i < _sizeOfArray; i++)
	{
		tarr[i] = _arr[i];
	}

	return tarr;
}

TArray<int> UHapticDevice::toTArray(int* _arr, int8 _sizeOfArray)
{
	TArray<int> tarr;
	tarr.Init(0, _sizeOfArray);
	for (int8 i = 0; i < _sizeOfArray; i++)
	{
		tarr[i] = _arr[i];
	}

	return tarr;
}

int UHapticDevice::dll_destroy()
{
	if (m_getDestroy != NULL)
	{

		int out = int(m_getDestroy()); 
		return out;
	}
	return -1;
}








#pragma endregion Import Methods



#pragma region Unload DLL

// If you love something  set it free.
void UHapticDevice::freeDLL()
{
	if (v_dllHandle != NULL)
	{
		//m_getInvertedBoolFromDll = NULL;
		//m_getIntPlusPlusFromDll = NULL;
		////m_getCircleAreaFromDll = NULL;
		//m_getCharArrayFromDll = NULL;
		//m_getVector4FromDll = NULL;

		FPlatformProcess::FreeDllHandle(v_dllHandle);
		v_dllHandle = NULL;
	}
}
#pragma endregion Unload DLL