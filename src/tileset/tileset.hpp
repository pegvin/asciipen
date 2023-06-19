#ifndef CSP_TILESET_TILESET_HPP_INCLUDED_
#define CSP_TILESET_TILESET_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"

struct TileSet {
	Pixel* tiles;
	u16 tileWidth = 0;     // number of pixels in a tile horizontally
	u16 tileHeight = 0;    // number of pixels in a tile vertically
	u16 tileSetWidth = 0;  // number of tiles horizontally
	u16 tileSetHeight = 0; // number of tiles vertically

	~TileSet();

	void LoadFrom(const char* filePath, u16 rows, u16 cols, u16 _tWidth, u16 _tHeight);
	void CopyTile(
		u16 x, u16 y,
		Pixel* destBuffer,
		u16 dBuffX, u16 dBuffY,
		u16 dBuffWidth, u16 dBuffHeight
	);

	Pixel* TileAt(u16 i);
};

#endif // CSP_TILESET_TILESET_HPP_INCLUDED_
