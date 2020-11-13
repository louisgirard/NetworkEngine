#pragma once

#include <cassert>
#include <nano_engine/common/logger.hpp>

namespace nano_engine
{
#ifndef NDEBUG
#   define NANO_ASSERT(Expr, Msg) \
    ___Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define NANO_ASSERT(Expr, Msg) ;
#endif

	void ___Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg);
}