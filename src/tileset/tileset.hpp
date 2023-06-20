#ifndef CSP_TILESET_TILESET_HPP_INCLUDED_
#define CSP_TILESET_TILESET_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"

struct TileSet {
	Vector<bool> tiles;
	// Pixel* tiles = nullptr;
	u16 tileWidth = 0;     // number of pixels in a tile horizontally
	u16 tileHeight = 0;    // number of pixels in a tile vertically
	u16 tileSetWidth = 0;  // number of tiles horizontally
	u16 tileSetHeight = 0; // number of tiles vertically

	~TileSet();

	void LoadFrom(const char* filePath, u16 rows, u16 cols, u16 _tWidth, u16 _tHeight);
	void CopyTile(
		i16 x, i16 y,
		Pixel* destBuffer,
		u16 dBuffX, u16 dBuffY,
		u16 dBuffWidth
	);
	inline void CopyTile(
		i16 idx, Pixel* destBuffer,
		u16 dBuffX, u16 dBuffY, u16 dBuffWidth
	) {
		this->CopyTile(idx % tileSetWidth, idx / tileSetHeight, destBuffer, dBuffX, dBuffY, dBuffWidth);
	};

	Pixel* TileAt(u16 i) const;
};

#endif // CSP_TILESET_TILESET_HPP_INCLUDED_
