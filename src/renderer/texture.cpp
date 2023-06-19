#include "glad/glad.h"
#include "renderer/texture.hpp"

Texture::Texture(u16 _w, u16 _h) {
	w = _w;
	h = _h;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	Pixel* data = new Pixel[w * h]{{0, 0, 0, 0}};
	// initially upload blank data so GPU has allocated space for the image.
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA, w, h,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data
	);
	delete[] data;
	data = nullptr;

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
	glDeleteTextures(1, &id);
	id = 0;
}

void Texture::Update(Pixel* pixelData) {
	glBindTexture(GL_TEXTURE_2D, id);
	// glTexSubImage2D is better to upload the pixels since glTexImage2D basically
	// deletes the buffer on GPU, reallocates it, and copies the data, and in glTexSubImage2D
	// the data is just copied onto the already existing texture.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
}
