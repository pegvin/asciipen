#ifndef CSP_RENDERER_TEXTURE_HPP_INCLUDED_
#define CSP_RENDERER_TEXTURE_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"

struct Texture {
	using GLuint = unsigned int;
	GLuint id = 0;

	Texture(u16 w, u16 h);
	~Texture();

	void Update(Pixel* pixelData);

private:
	u16 w, h;
};

#endif // CSP_RENDERER_TEXTURE_HPP_INCLUDED_
