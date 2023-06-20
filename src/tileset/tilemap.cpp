#include "tileset/tilemap.hpp"

TileMap::TileMap(
	u16 rows, u16 cols,
	const char* tileSetPath, u16 tileSetRows, u16 tileSetCols,
	u16 tileSetWidth, u16 tileSetHeight
) {
	tMapRows = rows;
	tMapCols = cols;
	tSet.LoadFrom(tileSetPath, tileSetRows, tileSetCols, tileSetWidth, tileSetHeight);
	tiles = new Tile[rows * cols];
}

TileMap::~TileMap() {
	delete[] tiles;
}

void TileMap::Render(const RectI32& dirtyArea, Pixel* pixBuff, const u32 pixBuffWidth) {
	for (i32 y = 0; y < dirtyArea.h; ++y) {
		for (i32 x = 0; x < dirtyArea.w; ++x) {
			i32 scaledX = (x + dirtyArea.x) * tSet.tileWidth;
			i32 scaledY = (y + dirtyArea.y) * tSet.tileHeight;
			tSet.CopyTile(
				tiles[((y + dirtyArea.y) * tMapRows) + dirtyArea.x].TileIndex,
				pixBuff, scaledX, scaledY,
				pixBuffWidth
			);
		}
	}
}
