#ifndef ASCIIPEN_DOC_TILESET_HPP_INCLUDED_
#define ASCIIPEN_DOC_TILESET_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"

struct TileSet {
	Pixel* Pixels = nullptr; // pixel data of TileSet
	u32 TileWidth = 0;
	u32 TileHeight = 0;
	u32 TileSetWidth = 0;
	u32 TileSetHeight = 0;

	inline u32 GetWidthPixels() { return TileWidth * TileSetWidth; };
	inline u32 GetHeightPixels() { return TileHeight * TileSetHeight; };
	inline u32 GetTotalPixels() { return (TileWidth * TileHeight) * (TileSetWidth * TileSetHeight); };

	bool Create_FromFile(const String& filePath, u32 tWidth, u32 tHeight, u32 tSetWidth, u32 tSetHeight);
	void CopyTile(
		u32 tX, u32 tY,
		Pixel* destBuffer,
		u32 dBuffX, u32 dBuffY,
		u32 dBuffWidth
	);
	inline void CopyTile(
		u32 idx, Pixel* destBuffer,
		u32 dBuffX, u32 dBuffY, u32 dBuffWidth
	) {
		this->CopyTile(idx % TileSetWidth, idx / TileSetHeight, destBuffer, dBuffX, dBuffY, dBuffWidth);
	};
	void Destroy();
};

#endif // ASCIIPEN_DOC_TILESET_HPP_INCLUDED_

