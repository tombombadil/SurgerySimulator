#pragma once

#include "Resource.h"
#include "OmValue.generated.h"

struct FOmValue;
struct FOmPair;
struct FOmChar;
using namespace std;

/* <---------- ValueType -------------> */
UENUM(BlueprintType)
enum class EOmValueType : uint8
{
	STRING,
	INT,
	FLOAT,
	RANGE,
	PAIR
};




USTRUCT(BlueprintType)
struct OMCEM_API FOmChar
{
	GENERATED_USTRUCT_BODY()
public:

	bool IsValid = false;
	EOmValueType Type;
	FOmChar() { IsValid = false; };
	FOmChar(FString _str);
	

	void Reset()
	{
		ch.Empty();
		charRange.Empty();
		IsValid = false;
	}


	FString ToString()
	{
		return  ch;
	}



	FString GetString()
	{ 
		return ch;
	}
	int8 GetInt();
	bool GetBool();
	float GetFloat();
	TArray<float> GetRange()
	{
		return charRange;
	}
	float GetRange(int8 _index)
	{
		return charRange[_index];
	}
	float GetRangeAvarage()
	{
		return GetRange(0) + ((GetRange(1) - GetRange(0) ) / 2);
	}
	bool IsNumeric()
	{
		return (Type == EOmValueType::FLOAT || Type == EOmValueType::INT);
	}



	bool GetCompare(FOmChar _other)
	{
		bool _result = false;
		FOmChar _this = *this;
		if (Type == _other.Type)
		{
			_result = (ch == _other.ch);
		}
		else
		{
			if (_this.IsNumeric() && _other.IsNumeric())
			{
				float c1 = GetFloat();
				float c2 = _other.GetFloat();
				_result = (c1 == c2);
			}
			else if (Type == EOmValueType::RANGE && _other.IsNumeric())
			{
				_result = getRangeComparison(_other.GetFloat(), GetRange());
			}
			else if (IsNumeric() && _other.Type == EOmValueType::RANGE)
			{
				_result = getRangeComparison(GetFloat(), _other.GetRange());
			}
		
		}
		return _result;
	}

	bool getRangeComparison(float _char, TArray<float> _range)
	{
		float _min = _range[0];
		float _max = _range[1];
		return (_char >= _min && _char <= _max);
	}


private:
	FString ch;
	TArray<float> charRange;

	EOmValueType getType();

	



};





USTRUCT(BlueprintType)
struct OMCEM_API FOmCharList
{
	GENERATED_USTRUCT_BODY()
public:

	bool IsValid = false;
	int8 CountOfChar;
	EOmValueType Type;
	FOmCharList() { CountOfChar = 0; IsValid = false; };
	FOmCharList(FString _data);
	void Update(FString _data);


	void Reset();
	FOmChar GetChar(int8 _index);
	FString ToString();

	TArray<FString> GetStringList();
	FString GetString();
	FString GetString(int8 _index);
	TArray<int8> GetInt();
	int8 GetInt(int8 _index=0);
	bool GetBool(int8 _index = 0);
	TArray<float> GetFloat();
	float GetFloat(int8 _index, bool _getAvarage=true);
	FVector GetFVector(bool _getAvarage = true);
	FRotator GetRotator();
	TPair<FVector, FVector> GetFVectorRanges();
	TPair<float, float> GetFloatRanges(int8 _index);
	bool GetCompare(FOmCharList _other);


private:

	TArray<FOmChar> listChar;

	void setType();



};





USTRUCT(BlueprintType)
struct OMCEM_API FOmValue 
{
	GENERATED_USTRUCT_BODY()
public:

	bool IsValid = false;
	int8 CountOfPair;
	FOmValue() {};
	FOmValue(FString _data);
	void Reset();
	static FOmValue CreateFromVector(vector<FString> _value);
	FString ToString();
	
	void UpdateValue(FString _key, FString _data, bool _createPairIfNoExist);
	void Add(FOmValue _other, bool _createPairIfNoExist);
	void Add(FString _valueStr, bool _createPairIfNoExist);
	FOmCharList GetValue(FString _key);
	FOmCharList GetValue();
	TArray<FOmCharList> GetValues();
	TArray<FString> GetKeys();

	FOmValue GetCompare(FOmValue _other);
	bool ContainsKey(FString _key);

	


private:
	
	TMap<FString, FOmCharList> listPairs;



};





USTRUCT(BlueprintType)
struct OMCEM_API FOmValues
{
	GENERATED_USTRUCT_BODY()

	TArray<FOmValue> list;
	bool IsValid;


	FOmValues()
	{
		IsValid = false;
	};
	FOmValues(FString _str)
	{
		Add(_str);
	};

	FOmValue Add(FString _str, int _index=-1);;

	static FOmValues CreateFromList(vector<FString> _list);;





	FOmValue GetOmValue(int _index);
	/* get params */
	//FString GetFunc(int _index = 0);
	//class AItem* GetItem(int _index = 0, FString _paramName = "item");
	//class UOmComp* GetComp(int _index = 0, FString _paramName = "comp");

	FString ToString();
};























//
//USTRUCT(BlueprintType)
//struct OMCEM_API FOmValue2
//{
//	GENERATED_USTRUCT_BODY()
//public:
//	TSharedPtr<FJsonObject> json;
//	bool IsValid = false;
//	int8 CountOfPair;
//	FOmValue2();
//	FOmValue2(FString _data);
//	void Reset();
//	//FString ToString();
//
//	void UpdateValue(FString _key, FString _data, bool _createPairIfNoExist);
//	//void Add(FOmValue _other, bool _createPairIfNoExist);
//	//void Add(FString _valueStr, bool _createPairIfNoExist);
//	//FOmCharList GetValue(FString _key);
//	//FOmCharList GetValue();
//	//TArray<FOmCharList> GetValues();
//	//TArray<FString> GetKeys();
//
//	//FOmValue GetCompare(FOmValue _other);
//	//bool ContainsKey(FString _key);
//
//
//private:
//
//	//TMap<FString, FOmCharList> listPairs;
//
//
//
//};