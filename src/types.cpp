#include "types.hpp"

#define GEN_STRUCT_OVERLOAD_FUNCS(name, type) \
	inline bool name::operator == (const name& rhs) const { \
		return (x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h); \
	} \
	inline bool name::operator != (const name& rhs) const { \
		return !(*this != rhs); \
	}

GEN_STRUCT_OVERLOAD_FUNCS(RectI32, i32)
GEN_STRUCT_OVERLOAD_FUNCS(RectF32, f32)

#undef GEN_STRUCT_OVERLOAD_FUNCS
