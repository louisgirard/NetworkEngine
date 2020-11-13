#include <nano_engine/common/assert.hpp>

namespace nano_engine
{
	void ___Assert(const char* expr_str, bool expr, const char* file, int line, const char* msg)
	{
		if (!expr)
		{
			NANO_ERR("Assert failed : \t{}\nExpected:\t{}\nSource:\t\t{},line {}\n", msg, expr_str, file, line);
			abort();
		}
	}
}