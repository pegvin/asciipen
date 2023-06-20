#include "pixel/pixel.hpp"
#include "types.hpp"
#include "stb_image.h"
#include "tileset/tileset.hpp"
#include <cstdio>
#include <cstring>

void TileSet::LoadFrom(const char* filePath, u16 rows, u16 cols, u16 _tWidth, u16 _tHeight) {
	i32 w, h, channels;
	u8* pixels = stbi_load(filePath, &w, &h, &channels, 4);

	if (w < 1 || h < 1 || channels < 4) {
		printf("Failed to read TileSet from: %s\n", filePath);
		return;
	}

	tileSetWidth = rows;
	tileSetHeight = cols;
	tileWidth = _tWidth;
	tileHeight = _tHeight;

	if (tiles) delete[] tiles;

	tiles = new Pixel[w * h];
	for (i32 y = 0; y < h; ++y) {
		for (i32 x = 0; x < w; ++x) {
			u8* _src = pixels + (y * w + x) * 4;
			tiles[(y * w) + x] = {
				*(_src + 0),
				*(_src + 1),
				*(_src + 2),
				*(_src + 3)
			};
		}
	}
	stbi_image_free(pixels);
	pixels = nullptr;
}

void TileSet::CopyTile(
	i16 tX, i16 tY,
	Pixel* destBuffer,
	u16 dBuffX, u16 dBuffY,
	u16 dBuffWidth
) {
	for (u32 y = 0; y < tileHeight; ++y) {
		if ((tY * tileSetWidth) + tX < 0) {
			for (u32 x = dBuffX; x < dBuffWidth; ++x) {
				destBuffer[((dBuffY + y) * dBuffWidth) + x] = { 0, 0, 0, 255 };
			}
		} else {
			u32 scaledX = tX * tileWidth;
			u32 scaledY = (tY * tileHeight) + y;
			Pixel* srcBuffer = tiles + (scaledY * tileSetWidth * tileWidth) + scaledX;
			memcpy(
				destBuffer + ((dBuffY + y) * dBuffWidth) + dBuffX,
				srcBuffer, sizeof(Pixel) * tileWidth
			);
		}
	}
}

Pixel* TileSet::TileAt(u16 tileIdx) const {
	return (tiles + (tileWidth * tileHeight * tileIdx));
}

TileSet::~TileSet() {
	if (tiles) {
		delete[] tiles;
		tiles = nullptr;
	}
}
