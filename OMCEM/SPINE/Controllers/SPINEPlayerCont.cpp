#include "SPINEPlayerCont.h"



ASPINEPlayerCont * ASPINEPlayerCont::INS = NULL;



ASPINEPlayerCont::ASPINEPlayerCont():APlayerCont()
{
	
	ASPINEPlayerCont::INS = this;
	PrimaryActorTick.bCanEverTick = true;
	
	//APlayerCont();

}
