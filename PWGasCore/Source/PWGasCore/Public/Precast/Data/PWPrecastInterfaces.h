// copyright mkurtt96

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PWPrecastInterfaces.generated.h"

UINTERFACE(BlueprintType)
class UPWPrecastVisualizer : public UInterface { GENERATED_BODY() };
class IPWPrecastVisualizer
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void Ensure(class APlayerController* PC, float PreviewRadius, FName OriginSocket);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void Update(const FPWPrecastVisualUpdate& Data);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) void Hide();
};
