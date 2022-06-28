#include "OmValue.h"
#include "OMCEM.h"
#include "OmUtils.h"
#include "OmEngine/Items/Item.h"


/* ______________ OM VALUE ______________ *
 
 unit types :
	- int : ('2')
	- range : ('2/1')
	- range : ('2.3/0.12')
	- float : ('2.3')
	- string : ('test')
	- string : ('2/a')
	- string : ('b.1')
*/
/* ______________________________________________ OmChar ______________________________________________ */
FOmChar::FOmChar(FString _str)
{
	IsValid = true;
	ch = _str;
	Type = getType();
	if (Type == EOmValueType::RANGE)
	{
		TArray<FString> listStrRange = UOmUtils::ParseString(_str, "/");
		charRange.Init(0, 2);
		float float1 = UOmUtils::ToFloat(listStrRange[0]);
		float float2 = UOmUtils::ToFloat(listStrRange[1]);
		float min = FMath::Min<float>(float1, float2);
		float max = FMath::Max<float>(float1, float2);
		charRange[0] = min;
		charRange[1] = max;
	}
}


int8 FOmChar::GetInt()
{
	return UOmUtils::ToInt(ch);
}

bool FOmChar::GetBool()
{
	return (UOmUtils::ToInt(ch) != 0);
}

float FOmChar::GetFloat()
{
	return UOmUtils::ToFloat(ch);
}

EOmValueType FOmChar::getType()
{
	EOmValueType _type = EOmValueType::STRING;
	if (ch.Len() > 0)
	{
		if (ch.Contains("/"))
		{
			TArray<FString> _list = UOmUtils::ParseString(ch, "/");
			if (_list.Num() > 1)
			{
				if (_list[0].IsNumeric() && _list[1].IsNumeric())
				{
					_type = EOmValueType::RANGE;
				}
			}


		}
		else if (ch.IsNumeric())
		{

			if (ch.Contains("."))
			{
				_type = EOmValueType::FLOAT;
			}
			else {
				_type = EOmValueType::INT;
			}
		}

	}
	return _type;
}

/* ______________________________________________ OmCharList ______________________________________________ */
FOmCharList::FOmCharList(FString _data)
{
	CountOfChar = 0;
	Update(_data);
	
	


}

void FOmCharList::Update(FString _data)
{
	if (_data.Len() > 0)
	{
		TArray<FString> listStrChar = UOmUtils::ParseString(_data, ",");
		listChar.Empty(0);
		//listChar.Reserve(listStrChar.Num());
		IsValid = true;
		for (FString s : listStrChar)
		{
			FOmChar c = FOmChar(s);
			listChar.Add(c);
		}
		CountOfChar = listChar.Num();
		setType();
	}
}

void FOmCharList::Reset()
{

	for (FOmChar c : listChar)
	{
		c.Reset();
	}
}


FOmChar FOmCharList::GetChar(int8 _index)
{
	FOmChar c;
	if (_index < CountOfChar)
		c = listChar[_index];
	return c;
}


void FOmCharList::setType()
{
	if (!listChar.IsValidIndex(0))
	{
		Type = EOmValueType::STRING;
	}
	else
	{
		int countStr = 0;
		FOmChar lastChar = listChar[0];
		EOmValueType lastType = lastChar.Type;
		for (FOmChar c : listChar)
		{
			EOmValueType cType = c.Type;
			if (cType != lastType)
			{
				if (lastChar.IsNumeric() && c.IsNumeric())
				{
					cType = EOmValueType::FLOAT;
				}
				else {
					lastType = EOmValueType::STRING;
					break;
				}

			}
			lastType = cType;
			

		}

		Type = lastType;





		
	}

	
}

bool FOmCharList::GetCompare(FOmCharList _other)
{
	bool _result = false;
	for (int8 i = 0; i < listChar.Num(); i++)
	{
		FOmChar c = GetChar(i);
		FOmChar cOther = _other.GetChar(i);
		bool boolCompare = false;
		if (cOther.IsValid)
		{
			boolCompare = c.GetCompare(cOther);
			_result = boolCompare;
		}
		if (!boolCompare)
		{
			return false;
		}
		
	}

	return _result;
}




FString FOmCharList::GetString()
{
	FString s = "";

	for (int8 i = 0; i < CountOfChar; i++)
	{
		FOmChar c = GetChar(i);
		s.Append(c.GetString());
		if (i < CountOfChar - 1) s.Append(",");
	}

	return s;
}
FString FOmCharList::GetString(int8 _index)
{
	FString v;
	if (_index < CountOfChar)
		v = listChar[_index].GetString();
	return v;
}

TArray<FString> FOmCharList::GetStringList()
{
	TArray<FString> arr;
	for (FOmChar c : listChar)
	{
		arr.Add(c.GetString());
	}
	return arr;
}

TArray<int8> FOmCharList::GetInt()
{
	TArray<int8> arr;
	for (FOmChar c : listChar)
	{
		arr.Add(c.GetInt());
	}
	return arr;
}
int8 FOmCharList::GetInt(int8 _index)
{
	int8 v = 0;
	if (_index < CountOfChar)
		v = listChar[_index].GetInt();
	return v;
}


bool FOmCharList::GetBool(int8 _index /*= 0*/)
{
	bool v = false;
	if (_index < CountOfChar)
		v = listChar[_index].GetBool();
	return v;
}

TArray<float> FOmCharList::GetFloat()
{
	TArray<float> arr;
	for (FOmChar c : listChar)
	{
		arr.Add(c.GetFloat());
	}
	return arr;
}
float FOmCharList::GetFloat(int8 _index, bool _getAvarage)
{
	float v = 0.0;
	if (_index < CountOfChar)
	{
		v = listChar[_index].GetFloat();
		if (_getAvarage)
		{
			if (GetChar(0).Type == EOmValueType::RANGE)
				v = GetChar(0).GetRangeAvarage();
		}
	}
		
	return v;
}

FVector FOmCharList::GetFVector(bool _getAvarage)
{
	FVector v = FVector::ZeroVector;
	if (CountOfChar >=  3)
	{
		v.X = GetFloat(0);
		v.Y = GetFloat(1);
		v.Z = GetFloat(2);
		if (_getAvarage)
		{
			if (GetChar(0).Type == EOmValueType::RANGE)
				v.X = GetChar(0).GetRangeAvarage();
			if (GetChar(1).Type == EOmValueType::RANGE)
					v.Y = GetChar(1).GetRangeAvarage();
			if (GetChar(2).Type == EOmValueType::RANGE)
				v.Z = GetChar(2).GetRangeAvarage();
		}
	}
	return v;
}


FRotator FOmCharList::GetRotator()
{
	FRotator r = FRotator::ZeroRotator;
	if (CountOfChar >= 3)
	{
		r.Roll = GetFloat(0);
		r.Pitch= GetFloat(1);
		r.Yaw = GetFloat(2);
		
	}
	return r;
}

/* return param -> TPair<min,max> */
TPair<FVector,FVector> FOmCharList::GetFVectorRanges()
{
	TPair<FVector, FVector> p;
	if (CountOfChar >= 3)
	{
		FVector min;
		FVector max;
			min.X = GetChar(0).GetRange(0);
			min.Y = GetChar(1).GetRange(0);
			min.Z = GetChar(2).GetRange(0);
			max.X = GetChar(0).GetRange(1);
			max.Y = GetChar(1).GetRange(1);
			max.Z = GetChar(2).GetRange(1);
			p.Key = min;
			p.Value = max;
	}
	return p;
}

TPair<float, float> FOmCharList::GetFloatRanges(int8 _index )
{
	TPair<float, float> p;
	float min = 0;
	float max = 0;

	if (_index < CountOfChar)
	{
		min = GetChar(0).GetRange(0);
		max = GetChar(0).GetRange(1);
		p.Key = min;
		p.Value = max;
	}

	return p;
}


FString FOmCharList::ToString()
{
	FString _str;
	FString _type = "null";
	FString _values = "";
	
	_type = FString::FromInt((int)Type);

	for (int8 i = 0; i < CountOfChar; i++)
	{
		FOmChar c = GetChar(i);
		_values.Append( c.ToString());
		if (i < CountOfChar - 1) _values.Append(",");
	}
	




	_str.Append("("+_values + ")");
	return _str;

}





/* _____________________________________________________________________________________________________ */
/* ______________________________________________ OmValue ______________________________________________ */
/* _____________________________________________________________________________________________________ */


FOmValue::FOmValue(FString _data)
{
	TMap<FString, FString> listStrPairs;

	if (_data.EndsWith(",")) _data.RemoveAt(_data.Len()-1, 1); //delete ","
	if( _data.Contains("(") && _data.Contains(")") )
	{
		TArray<FString> listFirst = UOmUtils::ParseString(_data, ")");
		for (int8 i = 0; i < listFirst.Num(); i++)
		{
			FString k;
			FString v;
			FString s = listFirst[i];
			if (s.StartsWith(",")) s.RemoveAt(0, 1); //delete ","
			TArray<FString> listStr;
			if (_data.Contains(":"))
			{
				listStr = UOmUtils::ParseString(s, ":(", true);
			}
			else
			{
				listStr = UOmUtils::ParseString(s, "(", true);
			}

			//if(listStr.IsValidIndex(0))
			if (listStr.Num() > 1)
			{
				k = listStr[0];
				v = listStr[1];
			}
			else
			{
				k = FString::FromInt(i);
				v = listStr[0];
			}

			listStrPairs.Add(k, v);

		}
	}
	else
	{
		listStrPairs.Add("0", _data);
	}



	for (TPair<FString, FString> pairStr : listStrPairs)
	{
		FOmCharList v = FOmCharList(pairStr.Value);
		listPairs.Add(pairStr.Key, v);
	}

	


		IsValid = true;

		CountOfPair = listPairs.Num();
	//}
	

}




void FOmValue::Reset()
{
	for (FOmCharList cl : GetValues())
	{
		cl.Reset();
	}
}
FOmValue FOmValue::CreateFromVector(vector<FString> _vec)
{
	FString str;
	int8 tot = (int8)_vec.size();
	for (int8 i = 0; i < tot; i++)
	{
		FString s = _vec[i];
		str.Append(s);
		if (i < tot - 1) str.Append(",");
	}
	return FOmValue(str);
}

void FOmValue::UpdateValue(FString _key, FString _data, bool _createPairIfNoExist)
{
	FOmCharList v = GetValue(_key);

	if (v.IsValid)
	{
		v.Update(_data);
		listPairs[_key] = v;
	}
	else
	{
		if (_createPairIfNoExist)
		{
			FOmCharList vNew = FOmCharList(_data);
			listPairs.Add(_key, vNew);
		}
		
	}
}


void FOmValue::Add(FOmValue _other, bool _createPairIfNoExist)
{
	int8 t = _other.CountOfPair;
	for (int8 i = 0; i < t; i++)
	{
		UpdateValue(_other.GetKeys()[i], _other.GetValues()[i].GetString(), _createPairIfNoExist);
	}
}
void FOmValue::Add(FString _valueStr, bool _createPairIfNoExist)
{
	FOmValue v = FOmValue(_valueStr);
	int8 t = v.CountOfPair;
	for (int8 i = 0; i < t; i++)
	{
		UpdateValue(v.GetKeys()[i], v.GetValues()[i].GetString(), _createPairIfNoExist);
	}
}

FOmCharList FOmValue::GetValue(FString _key)
{
	FOmCharList v;
	if(listPairs.Contains(_key))
		 v = listPairs[_key];
	return v;
}
FOmCharList FOmValue::GetValue()
{
	FOmCharList v = GetValue("0");
	return v;
}

TArray<FString> FOmValue::GetKeys()
{
	TArray<FString> arr;
	for (TPair<FString, FOmCharList> p : listPairs)
	{
		arr.Add(p.Key);
	}
	return arr;
}



TArray<FOmCharList> FOmValue::GetValues()
{
	TArray<FOmCharList> arr;
	for (TPair<FString, FOmCharList> p: listPairs)
	{
		arr.Add(p.Value);
	}
	return arr;
}




FOmValue FOmValue::GetCompare(FOmValue _other)
{
	FString s = "";
	int _total = listPairs.Num();
	int8 i = 0;
	for (TPair<FString, FOmCharList> p : listPairs)
	{
		FString k = p.Key;
		FOmCharList charList = p.Value;
		FOmCharList otherCharList = _other.GetValue(k);
		if (otherCharList.IsValid)
		{
			bool boolCompare = charList.GetCompare(otherCharList);
			FString strCompare = FString::FromInt((int)boolCompare);
			s.Append(p.Key + ":(" + strCompare + ")");
			if (i < _total - 1) s.Append(",");
		}
		i++;
	}

	return FOmValue(s);
}



bool FOmValue::ContainsKey(FString _key)
{
	bool r = GetKeys().Contains(_key);
	return r;
}



FString FOmValue::ToString()
{
	FString _str;
	FString _values = "";
	
	int _total = listPairs.Num();
	int8 i = 0;
	for(TPair<FString, FOmCharList> p : listPairs)
	{
		FString _key = p.Key;
		FOmCharList _value = p.Value;
		_str.Append(p.Key + ":" + _value.ToString());
		if (i < _total - 1) _str.Append(",");
		i++;

	}

	return _str;
	

}



/* _____________________________________________________________________________________________________ */
/* ______________________________________________ FOmValues ______________________________________________ */
/* _____________________________________________________________________________________________________ */

FOmValue FOmValues::GetOmValue(int _index)
{
	FOmValue c;
	if (_index < list.Num())
		c = list[_index];
	return c;
}

FOmValue FOmValues::Add(FString _str, int _index/*=-1*/)
{
	FOmValue ov = FOmValue(_str);
	
		if (ov.IsValid)
		{
			if (_index >= 0)
			{
				if (_index < list.Num()) list[_index].Add(_str, true);
			}
			else
			{
				list.Add(ov);
			}
			IsValid = true;
		}
		
	return ov;
}

FOmValues FOmValues::CreateFromList(vector<FString> _list)
{
	FOmValues omValues = FOmValues();
	if (_list.size() > 0) omValues.IsValid = true;
	for (FString str : _list)
	{
		if (!omValues.Add(str).IsValid)
		{
			omValues.IsValid = false;
		}
	}
	return omValues;
}

FString FOmValues::ToString()
{
	FString s = "";
	for (FOmValue c : list)
	{
		if (c.IsValid)
		{
			s.Append(c.ToString());
			s.Append(",");

		}

	}
	return s;
}

//AItem* FOmValues::GetItem(int _index /*= 0*/, FString _paramName /*= "item"*/)
//{
//	AItem* i = 0;
//	if (_index < list.Num())
//	{
//		FString idItem = list[_index].GetValue(_paramName).GetString(0);
//		if (idItem.IsValidIndex(0))
//			i = Cast<AItem>(UResource::INS->GetItem(idItem));
//	}
//		
//	return i;
//}
//
//UOmComp* FOmValues::GetComp(int _index /*= 0*/, FString _paramName /*= "comp"*/)
//{
//	UOmComp* i = 0;
//	if (_index < list.Num())
//		i = UResource::INS->GetOmComp(list[_index].GetValue(_paramName).GetString(0));
//	return i;
//}


//FString FOmValues::GetFunc(int _index /*= 0*/)
//{
//	FString f;
//	if (_index < list.Num())
//		f = list[_index].GetValue("func").GetString(0);
//	return f;
//}




















