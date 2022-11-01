// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define RETURN_IF_NULL(Object)		if(Object == nullptr)		{ return; }
#define RETURN_IF_TRUE(Expression)	if((Expression) == true)	{ return; }
#define RETURN_IF_FALSE(Expression) if((Expression) == false)	{ return; }

#define RETURN_IF_NULL_ENSURE(Object)		if(Object == nullptr)		{ ensure(false); return; }
#define RETURN_IF_TRUE_ENSURE(Expression)	if((Expression) == true)	{ ensure(false); return; }
#define RETURN_IF_FALSE_ENSURE(Expression)	if((Expression) == false)	{ ensure(false); return; }

#define RETURN_VALUE_IF_NULL(Object, Value)			if(Object == nullptr)		{ return Value; }
#define RETURN_VALUE_IF_TRUE(Expression, Value)		if((Expression) == true)	{ return Value; }
#define RETURN_VALUE_IF_FALSE(Expression, Value)	if((Expression) == false)	{ return Value; }

#define RETURN_VALUE_IF_NULL_ENSURE(Object, Value)		if(Object == nullptr)		{ ensure(false); return Value; }
#define RETURN_VALUE_IF_TRUE_ENSURE(Expression, Value)	if((Expression) == true)	{ ensure(false); return Value; }
#define RETURN_VALUE_IF_FALSE_ENSURE(Expression, Value)	if((Expression) == false)	{ ensure(false); return Value; }

DECLARE_LOG_CATEGORY_EXTERN(LogSCredits, Log, All);