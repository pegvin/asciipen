#include "doc/tilemap.hpp"

bool TileMap::Create(u32 tilemapWidth, u32 tilemapHeight) {
	if (tilemapWidth == 0 || tilemapHeight == 0) return false;

	Tiles = new Tile[tilemapWidth * tilemapHeight];
	if (Tiles == nullptr) {
		return false;
	}

	Width = tilemapWidth;
	Height = tilemapHeight;

	return true;
}

void TileMap::Destroy() {
	Width = 0;
	Height = 0;
	if (Tiles != nullptr) {
		delete[] Tiles;
	}
}

