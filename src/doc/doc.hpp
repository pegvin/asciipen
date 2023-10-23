#ifndef ASCIIPEN_DOC_DOC_HPP_INCLUDED_
#define ASCIIPEN_DOC_DOC_HPP_INCLUDED_ 1
#pragma once

// https://pegvin.github.io/tile-vs-tileset-vs-tilemap.html

#include "types.hpp"
#include "doc/tilemap.hpp"
#include "doc/tileset.hpp"
#include "tools/ToolManager.hpp"

struct Document {
	String filePath;
	TileMap tileMap;
	TileSet tileSet;
	Tool::Manager toolManager;

	void Render(const RectI32& dirty, Pixel* pixBuff, i32 pixBuffWidth, i32 pixBuffHeight);

	inline i32 TileMapWidthPixels() { return tileMap.Width * tileSet.TileWidth; };
	inline i32 TileMapHeightPixels() { return tileMap.Height * tileSet.TileHeight; };
};

#endif // ASCIIPEN_DOC_DOC_HPP_INCLUDED_

