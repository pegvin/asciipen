#include "doc/tileset.hpp"
#include "stb_image.h"

#include "microtar.h"
#include "SimpleIni.h"

bool TileSet::Create_FromFile(const String& filePath) {
	mtar_t tArchive;
	mtar_header_t tHeader_infoini;
	mtar_header_t tHeader_tilesetpng;

	if (mtar_open(&tArchive, filePath.c_str(), "r") != MTAR_ESUCCESS) {
		printf("Failed to open file: %s\n", filePath.c_str());
		return false;
	}

	if (mtar_find(&tArchive, "info.ini", &tHeader_infoini) != MTAR_ESUCCESS) {
		printf("Failed to find 'info.ini' inside archive\n");
		return false;
	}
	char* infoTxt = (char*)malloc(tHeader_infoini.size);
	if (mtar_read_data(&tArchive, infoTxt, tHeader_infoini.size) != MTAR_ESUCCESS) {
		printf("Failed to read 'info.ini' inside archive\n");
		return false;
	}

	if (mtar_find(&tArchive, "tileset.png", &tHeader_tilesetpng) != MTAR_ESUCCESS) {
		printf("Failed to find 'info.ini' inside archive\n");
		return false;
	}
	u8* tilesetPng = (u8*)malloc(tHeader_tilesetpng.size);
	if (mtar_read_data(&tArchive, tilesetPng, tHeader_tilesetpng.size) != MTAR_ESUCCESS) {
		printf("Failed to read 'info.ini' inside archive\n");
		return false;
	}

	if (mtar_close(&tArchive) != MTAR_ESUCCESS) {
		printf("Error\n");
		return false;
	}

	CSimpleIniA ini;
	ini.SetUnicode();

	SI_Error rc = ini.LoadData(infoTxt, tHeader_infoini.size);
	if (rc < 0) {
		printf("Error\n");
		return false;
	}

	TileWidth = ini.GetLongValue("info", "TileWidth");
	TileHeight = ini.GetLongValue("info", "TileHeight");
	TileSetWidth = ini.GetLongValue("info", "TileSetWidth");
	TileSetHeight = ini.GetLongValue("info", "TileSetHeight");

	free(infoTxt);

	i32 w, h, channels;
	u8* data = stbi_load_from_memory(tilesetPng, tHeader_tilesetpng.size, &w, &h, &channels, 4);

	free(tilesetPng);

	if (w < 1 || h < 1 || channels < 4 || data == NULL) {
		printf("Failed to read TileSet from: %s\n", filePath.c_str());
		return false;
	}

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
	u32 dBuffWidth,
	Pixel fgColor,
	Pixel bgColor
) {
	for (u32 y = 0; y < TileHeight; ++y) {
		for (u32 x = 0; x < TileWidth; ++x) {
			u32 scaledX = (tX * TileWidth) + x;
			u32 scaledY = (tY * TileHeight) + y;
			bool isPixelOn = Pixels[(scaledY * this->GetWidthPixels()) + scaledX] == 0xFFFFFFFF;
			destBuffer[((dBuffY + y) * dBuffWidth) + (dBuffX + x)] = isPixelOn ? fgColor : bgColor;
		}
	}
}

