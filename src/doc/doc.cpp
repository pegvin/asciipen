#include "doc/doc.hpp"

void Document::Render(const RectI32& dirtyArea, Pixel *pixBuff, u32 pixBuffWidth, u32 pixBuffHeight) {
	for (i32 y = 0; y < dirtyArea.h; ++y) {
		for (i32 x = 0; x < dirtyArea.w; ++x) {
			i32 gridX = dirtyArea.x + x;
			i32 gridY = dirtyArea.y + y;

			u32 scaledX = gridX * tileSet.TileWidth;
			u32 scaledY = gridY * tileSet.TileHeight;
			if (scaledX > pixBuffWidth || scaledY > pixBuffHeight)
				continue;

			i32 index = tileMap.Tiles[(gridY * tileMap.Width) + gridX].Index;
			if (index > -1) {
				tileSet.CopyTile(
					index,
					pixBuff, scaledX, scaledY,
					pixBuffWidth
				);
			}
		}
	}
}

