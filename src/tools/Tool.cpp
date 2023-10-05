#include "tools/Tool.hpp"

RectI32 Tool::Draw(i32 x, i32 y, TileMap& map, Tile t, u16 StrokeSize, bool isCircle) {
	RectI32 dirty = { 0, 0, 0, 0 };
	bool firstTime = true;

	for (i32 dirY = -StrokeSize / 2; dirY < StrokeSize / 2 + 1; dirY++) {
		for (i32 dirX = -StrokeSize / 2; dirX < StrokeSize / 2 + 1; dirX++) {
			if (
				x + dirX < 0          ||
				x + dirX >= map.Width ||
				y + dirY < 0          ||
				y + dirY > map.Height
			) continue;

			if (isCircle && (dirX * dirX + dirY * dirY) > (StrokeSize / 2 * StrokeSize / 2))
				continue;

			u16 affectedX = x + dirX, affectedY = y + dirY;
			if (affectedX < map.Width && affectedY < map.Height) {
				if (firstTime) {
					dirty.x = affectedX;
					dirty.y = affectedY;
					dirty.w = 1;
					dirty.h = 1;
					firstTime = false;
				}

				if (affectedX < dirty.x) dirty.x = affectedX;
				if (affectedY < dirty.y) dirty.y = affectedY;
				if (affectedX > dirty.w) dirty.w = affectedX;
				if (affectedY > dirty.h) dirty.h = affectedY;

				if (dirty.x < 0) dirty.x = 0;
				if (dirty.y < 0) dirty.y = 0;
				if ((dirty.x + dirty.w) > map.Width) dirty.w = map.Width - dirty.x;
				if ((dirty.y + dirty.h) > map.Height) dirty.h = map.Height - dirty.y;
				if ((dirty.x - affectedX) > dirty.w) dirty.w = dirty.x - affectedX;
				if ((dirty.y - affectedY) > dirty.h) dirty.h = dirty.y - affectedY;

				map.Tiles[(affectedY * map.Width) + affectedX] = t;
			}
		}
	}

	return dirty;
}

