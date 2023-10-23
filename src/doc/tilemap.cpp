#include "doc/tilemap.hpp"

bool TileMap::Create(i32 tilemapWidth, i32 tilemapHeight) {
	if (tilemapWidth < 1 || tilemapHeight < 1) return false;

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

