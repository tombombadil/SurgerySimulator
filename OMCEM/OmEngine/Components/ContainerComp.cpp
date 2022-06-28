// Copyright 2015 - 2020 SharpSurgeon 
#include "ContainerComp.h"
#include "OMCEM.h"


bool UContainerComp::CanPutItem(FString _idCat)
{
	bool r = false;
	if (FilterOnlyCats.IsValidIndex(0))
	{
		r = FilterOnlyCats.Contains(_idCat);
	}
	else
	{
		r = true;
	}

	if (FilterExceptCats.IsValidIndex(0))
	{
		r = !FilterExceptCats.Contains(_idCat);
	}
	return r;
}
