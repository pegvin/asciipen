#ifndef CSP_TILESET_TILEMAP_HPP_INCLUDED_
#define CSP_TILESET_TILEMAP_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"
#include "tileset/tileset.hpp"

struct Tile {
	i16 TileIndex = -1;
	Pixel FgColor = { 255, 255, 255, 255 };
	Pixel BgColor = { 0, 0, 0, 255 };
};

struct TileMap {
	Tile*   tiles = nullptr;
	TileSet tSet;
	u16 tMapRows = 0;
	u16 tMapCols = 0;

	TileMap(u16 rows, u16 cols, const char* tileSetPath, u16 tileSetRows, u16 tileSetCols, u16 tileSetWidth, u16 tileSetHeight);
	~TileMap();

	inline u32 GetTotalPixels() { return (tMapRows * tSet.tileWidth) * (tMapCols * tSet.tileHeight); };
	inline u32 GetWidthPixels() { return (tMapRows * tSet.tileWidth); };
	inline u32 GetHeightPixels() { return (tMapCols * tSet.tileHeight); };

	/*
		Function Renders The TileMap To The Pixel Buffer (pixBuff) Of Your Choice
		Notes:
			- Make Sure The Pixel Buffer Is Atleast
				- tMapRows * tSet.tileWidth Wide
				- tMapCols * tSet.tileHeight Tall
	*/
	void Render(const RectI32& dirtyArea, Pixel* pixBuff, const u32 pixBuffWidth);
};

#endif // CSP_TILESET_TILEMAP_HPP_INCLUDED_
