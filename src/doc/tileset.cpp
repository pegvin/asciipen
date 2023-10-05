#include "doc/tileset.hpp"
#include "stb_image.h"

bool TileSet::Create_FromFile(const String& filePath, u32 tWidth, u32 tHeight, u32 tSetWidth, u32 tSetHeight) {
	i32 w, h, channels;
	u8* data = stbi_load(filePath.c_str(), &w, &h, &channels, 4);

	if (w < 1 || h < 1 || channels < 4) {
		printf("Failed to read TileSet from: %s\n", filePath.c_str());
		return false;
	}

	TileSetWidth = tSetWidth;
	TileSetHeight = tSetHeight;
	TileWidth = tWidth;
	TileHeight = tHeight;

	Pixels = new Pixel[(TileWidth * TileHeight) * (TileSetWidth * TileSetHeight)];

	for (i32 y = 0; y < h; ++y) {
		for (i32 x = 0; x < w; ++x) {
			u8* src = data + (y * w + x) * 4;
			Pixels[(y * w) + x] = *((u32*)src);
		}
	}

	stbi_image_free(data);
	data = nullptr;

	return true;
}

void TileSet::Destroy() {
	TileWidth = 0;
	TileHeight = 0;
	TileSetWidth = 0;
	TileSetHeight = 0;

	if (Pixels != nullptr) {
		delete[] Pixels;
		Pixels = nullptr;
	}
}

void TileSet::CopyTile(
	u32 tX, u32 tY,
	Pixel* destBuffer,
	u32 dBuffX, u32 dBuffY,
	u32 dBuffWidth
) {
	for (u32 y = 0; y < TileHeight; ++y) {
		for (u32 x = 0; x < TileWidth; ++x) {
			u32 scaledX = (tX * TileWidth) + x;
			u32 scaledY = (tY * TileHeight) + y;
			destBuffer[((dBuffY + y) * dBuffWidth) + (dBuffX + x)] = Pixels[(scaledY * this->GetWidthPixels()) + scaledX];
		}
	}
}

