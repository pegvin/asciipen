#ifndef CSP_SRC_PIXEL_PIXEL_HPP_INCLUDED_
#define CSP_SRC_PIXEL_PIXEL_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"

#ifndef TARGET_IS_BIGENDIAN
	#define PIXEL_RED_MASK   0xFF000000
	#define PIXEL_GREEN_MASK 0x00FF0000
	#define PIXEL_BLUE_MASK  0x0000FF00
	#define PIXEL_ALPHA_MASK 0x000000FF

	#define PIXEL_RED_SHIFT   24
	#define PIXEL_GREEN_SHIFT 16
	#define PIXEL_BLUE_SHIFT  8
	#define PIXEL_ALPHA_SHIFT 0
#else
	#define PIXEL_RED_MASK   0x000000FF
	#define PIXEL_GREEN_MASK 0x0000FF00
	#define PIXEL_BLUE_MASK  0x00FF0000
	#define PIXEL_ALPHA_MASK 0xFF000000

	#define PIXEL_RED_SHIFT   0
	#define PIXEL_GREEN_SHIFT 8
	#define PIXEL_BLUE_SHIFT  16
	#define PIXEL_ALPHA_SHIFT 24
#endif

struct Pixel {
	u8 r = 0, g = 0, b = 0, a = 0;

	inline Pixel& operator = (const u32 RGBA_Color) {
		r = (RGBA_Color & PIXEL_RED_MASK)   >> PIXEL_RED_SHIFT;
		g = (RGBA_Color & PIXEL_GREEN_MASK) >> PIXEL_GREEN_SHIFT;
		b = (RGBA_Color & PIXEL_BLUE_MASK)  >> PIXEL_BLUE_SHIFT;
		a = (RGBA_Color & PIXEL_ALPHA_MASK) >> PIXEL_ALPHA_SHIFT;
		return *this;
	}

	inline bool operator == (const Pixel& rhs) const {
		return (r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a);
	}

	inline bool operator == (const u32 rhs) const {
		Pixel pixel_rhs;
		pixel_rhs = rhs;
		return (*this == pixel_rhs);
	}

	inline bool operator != (const Pixel& rhs) const {
		return !(*this == rhs);
	}

	inline bool operator != (const u32 rhs) const {
		return !(*this == rhs);
	}
};

#endif // CSP_SRC_PIXEL_PIXEL_HPP_INCLUDED_

