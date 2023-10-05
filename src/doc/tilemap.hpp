#ifndef ASCIIPEN_DOC_TILEMAP_HPP_INCLUDED_
#define ASCIIPEN_DOC_TILEMAP_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "pixel/pixel.hpp"

struct Tile {
	i32   Index = -1;                          // Index of the Tile in a Tile Set
	Pixel Foreground = { 255, 255, 255, 255 }; // Foreground Color of the Tile
	Pixel Background = { 0, 0, 0, 0 };         // Background Color of the Tile
};

struct TileMap {
	u32   Width  = 0;       // Width of the Tile Map
	u32   Height = 0;       // Height of the Tile Map
	Tile* Tiles  = nullptr; // Array of Tiles of the Tile Map

	bool Create(u32 tilemapWidth, u32 tilemapHeight);
	void Destroy();
};

#endif // ASCIIPEN_DOC_TILEMAP_HPP_INCLUDED_

