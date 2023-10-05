#include "doc/doc.hpp"
#include "tools/ToolManager.hpp"
#include "tools/Tool.hpp"

using namespace Tool;

RectI32 Manager::OnMouseDown(i32 x, i32 y, Document& doc) {
	RectI32 dirty = { -1, -1, -1, -1 };
	MousePosWhenDown = { x, y };
	MousePosLast = { x, y };

	VectorI32 MousePosRel = {
		i32((x / doc.tileSet.TileWidth) / doc.toolManager.ViewPortScale),
		i32((y / doc.tileSet.TileHeight) / doc.toolManager.ViewPortScale)
	};

	switch (ToolType) {
		case BRUSH: {
			dirty = Tool::Draw(MousePosRel.x, MousePosRel.y, doc.tileMap, selectedTile, BrushSize, isBrushCircle);
			break;
		}
		case ERASE: {
			dirty = Tool::Draw(MousePosRel.x, MousePosRel.y, doc.tileMap, { -1 }, BrushSize, isBrushCircle);
			break;
		}
		case LINE: {
			break;
		}
		case PAN: {
			break;
		}
	}
	return dirty;
}

RectI32 Manager::OnMouseMove(i32 x, i32 y, Document& doc) {
	RectI32 dirty = { -1, -1, -1, -1 };
	VectorI32 MousePosRel = {
		i32((x / doc.tileSet.TileWidth) / doc.toolManager.ViewPortScale),
		i32((y / doc.tileSet.TileHeight) / doc.toolManager.ViewPortScale)
	};
	switch (ToolType) {
		case BRUSH: {
			dirty = Tool::Draw(MousePosRel.x, MousePosRel.y, doc.tileMap, selectedTile, BrushSize, isBrushCircle);
			break;
		}
		case ERASE: {
			dirty = Tool::Draw(MousePosRel.x, MousePosRel.y, doc.tileMap, { -1 }, BrushSize, isBrushCircle);
			break;
		}
		case PAN: {
			if (MousePosLast.x != INT_MIN && MousePosLast.y != INT_MAX) {
				ViewPort.x += x - MousePosLast.x;
				ViewPort.y += y - MousePosLast.y;
			}
			break;
		}
		default: break;
	}

	return dirty;
}

RectI32 Manager::OnMouseUp(i32 x, i32 y, Document& doc) {
	RectI32 dirty = { -1, -1, -1, -1 };

	VectorI32 MousePosRel = {
		i32((x / doc.tileSet.TileWidth) / doc.toolManager.ViewPortScale),
		i32((y / doc.tileSet.TileHeight) / doc.toolManager.ViewPortScale)
	};

	switch (ToolType) {
		case BRUSH: {
			dirty = Tool::Draw(MousePosRel.x, MousePosRel.y, doc.tileMap, selectedTile, BrushSize, isBrushCircle);
			break;
		}
		case ERASE: {
			dirty = Tool::Draw(MousePosRel.x, MousePosRel.y, doc.tileMap, { -1 }, BrushSize, isBrushCircle);
			break;
		}
		case LINE: {
			break;
		}
		default: break;
	}

	MousePosWhenDown = { INT_MIN, INT_MIN };
	MousePosLast = { INT_MIN, INT_MIN };
	return dirty;
}

void Manager::UpdateViewportScale() {
	ViewPortScale = ViewPortScale > 1.0f ? ViewPortScale : 1.0f;

	// Ensures That The ViewPort is Centered From The Center
	VectorF32 CurrRectCenter = {
		(ViewPort.w / 2) + ViewPort.x, (ViewPort.h / 2) + ViewPort.y
	};
	VectorF32 NewRectCenter = {
		(ViewPortSize.x * ViewPortScale / 2) + ViewPort.x,
		(ViewPortSize.y * ViewPortScale / 2) + ViewPort.y
	};
	ViewPort.x -= NewRectCenter.x - CurrRectCenter.x;
	ViewPort.y -= NewRectCenter.y - CurrRectCenter.y;

	// Update The Size Of The ViewPort
	ViewPort.w = ViewPortSize.x * ViewPortScale;
	ViewPort.h = ViewPortSize.y * ViewPortScale;
}

