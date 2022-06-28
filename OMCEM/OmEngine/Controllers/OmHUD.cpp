#include "OmHUD.h"
#include "OMCEM.h"
#include "MainCont.h"

void AOmHUD::ShowMessage(FString _msg, bool _append /*= true*/)
{
	if (AMainCont::INS->DebugMode < 1) return;
	listMessages.Add(_msg);
	FString msg = "";
	for (FString s : listMessages)
		msg.Append("\n" + s);

	BpEventRun("func:(ShowMessage),msg:("+msg+")");
}
void AOmHUD::HideMessage()
{
	listMessages.Empty(0);
	BpEventRun("func:(HideMessage)");
}
