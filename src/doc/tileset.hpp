#ifndef ASCIIPEN_DOC_TILESET_HPP_INCLUDED_
#define ASCIIPEN_DOC_TILESET_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"

struct TileSet {
	Pixel* Pixels = nullptr; // pixel data of TileSet
	i32 TileWidth = 0;
	i32 TileHeight = 0;
	i32 TileSetWidth = 0;
	i32 TileSetHeight = 0;

	inline i32 GetWidthPixels() { return TileWidth * TileSetWidth; };
	inline i32 GetHeightPixels() { return TileHeight * TileSetHeight; };
	inline i32 GetTotalPixels() { return (TileWidth * TileHeight) * (TileSetWidth * TileSetHeight); };

	bool Create_FromFile(const String& filePath);
	void CopyTile(
		i32 tX, i32 tY,
		Pixel* destBuffer,
		i32 dBuffX, i32 dBuffY, i32 dBuffWidth,
		Pixel foregroundColor, Pixel backgroundColor
	);
	inline void CopyTile(
		i32 idx, Pixel* destBuffer,
		i32 dBuffX, i32 dBuffY, i32 dBuffWidth,
		Pixel foregroundColor, Pixel backgroundColor
	) {
		this->CopyTile(idx % TileSetWidth, idx / TileSetHeight, destBuffer, dBuffX, dBuffY, dBuffWidth, foregroundColor, backgroundColor);
	};
	void Destroy();
};

#endif // ASCIIPEN_DOC_TILESET_HPP_INCLUDED_

