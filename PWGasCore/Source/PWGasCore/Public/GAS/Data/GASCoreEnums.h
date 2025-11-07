#pragma once


UENUM(BlueprintType)
enum class EInputEventType : uint8
{
	Pressed  UMETA(DisplayName="Pressed"),
	Released UMETA(DisplayName="Released")
};
