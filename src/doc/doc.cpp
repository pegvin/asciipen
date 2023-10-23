#include "doc/doc.hpp"

void Document::Render(const RectI32& dirtyArea, Pixel *pixBuff, i32 pixBuffWidth, i32 pixBuffHeight) {
	if (dirtyArea.x < 1 && dirtyArea.y < 1 && dirtyArea.w < 1 && dirtyArea.h < 1) return;

	for (i32 y = 0; y < dirtyArea.h; ++y) {
		for (i32 x = 0; x < dirtyArea.w; ++x) {
			i32 gridX = dirtyArea.x + x;
			i32 gridY = dirtyArea.y + y;

			i32 scaledX = gridX * tileSet.TileWidth;
			i32 scaledY = gridY * tileSet.TileHeight;
			if (scaledX > pixBuffWidth || scaledY > pixBuffHeight)
				continue;

			Tile t = tileMap.Tiles[(gridY * tileMap.Width) + gridX];
			if (t.Index > -1) {
				tileSet.CopyTile(
					t.Index,
					pixBuff, scaledX, scaledY,
					pixBuffWidth,
					t.Foreground,
					t.Background
				);
			} else { // clear the tile
				for (i32 y = 0; y < tileSet.TileHeight; ++y) {
					for (i32 x = 0; x < tileSet.TileWidth; ++x) {
						pixBuff[((scaledY + y) * pixBuffWidth) + (scaledX + x)] = 0x00U;
					}
				}
			}
		}
	}
}

