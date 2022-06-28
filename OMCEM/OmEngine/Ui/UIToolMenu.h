#pragma once
 
#include "Blueprint/UserWidget.h"
#include "OmEngine/Ui/UIBase.h"
#include "Blueprint/UserWidget.h"
#include "Object.h"
#include "UIToolMenu.generated.h"


class UUIController;


 UCLASS()
class OMCEM_API UUIToolMenu : public UUIBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite) 	int SelectedTool = -1;

	virtual void Open() override;
	virtual void Close() override;
	UFUNCTION(BlueprintImplementableEvent, Category = "OMCEM LIBRARY")  void BpEventAddItem(int _id, UTexture2D* _texture2D, const FString &_label);
	UFUNCTION(BlueprintImplementableEvent, Category = "OMCEM LIBRARY") 	void BpEventOpen();
	UFUNCTION(BlueprintImplementableEvent, Category = "OMCEM LIBRARY") 	void BpEventClose();
	UPROPERTY(BlueprintReadWrite, Category = "OMCEM LIBRARY") int SelectedItemId;
private:
};

