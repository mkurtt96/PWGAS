// copyright mkurtt96

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogAbility, Log, All);

#define CUR_CLASS_FUNC (FString(__FUNCTION__))
#define pwlogability(Verbosity, Format, ...) UE_LOG(LogAbility, Verbosity, TEXT("%s >> %s"), *CUR_CLASS_FUNC, *FString::Printf(TEXT(Format), ##__VA_ARGS__))