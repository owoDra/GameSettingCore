// Copyright (C) 2023 owoDra

#pragma once

#include "Logging/LogMacros.h"

GSCORE_API DECLARE_LOG_CATEGORY_EXTERN(LogGSC, Log, All);

#if !UE_BUILD_SHIPPING

#define GSCLOG(FormattedText, ...) UE_LOG(LogGSC, Log, FormattedText, __VA_ARGS__)

#define GSCENSURE(InExpression) ensure(InExpression)
#define GSCENSURE_MSG(InExpression, InFormat, ...) ensureMsgf(InExpression, InFormat, __VA_ARGS__)
#define GSCENSURE_ALWAYS_MSG(InExpression, InFormat, ...) ensureAlwaysMsgf(InExpression, InFormat, __VA_ARGS__)

#define GSCCHECK(InExpression) check(InExpression)
#define GSCCHECK_MSG(InExpression, InFormat, ...) checkf(InExpression, InFormat, __VA_ARGS__)

#else

#define GSCLOG(FormattedText, ...)

#define GSCENSURE(InExpression) InExpression
#define GSCENSURE_MSG(InExpression, InFormat, ...) InExpression
#define GSCENSURE_ALWAYS_MSG(InExpression, InFormat, ...) InExpression

#define GSCCHECK(InExpression) InExpression
#define GSCCHECK_MSG(InExpression, InFormat, ...) InExpression

#endif