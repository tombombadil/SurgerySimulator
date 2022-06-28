#include "OMCEMGameModeBase.h"
#include "OMCEM.h"
#include "OmEngine/Utils/Config.h"
#include "OmEngine/Controllers/MainCont.h"

AOMCEMGameModeBase::AOMCEMGameModeBase()
{
	//HUDClass = AUICont::StaticClass();
	//DefaultPawnClass = AInputs::StaticClass();
	


}


void AOMCEMGameModeBase::BeginPlay()
{

	Super::BeginPlay();
	//APlayerController* playerCont = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//playerCont->SetViewTarget(ACamCont::INS);
	

}









