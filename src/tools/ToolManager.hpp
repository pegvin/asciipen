#ifndef ASCIIPEN_TOOLS_TOOLMANAGER_HPP_INCLUDED_
#define ASCIIPEN_TOOLS_TOOLMANAGER_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "doc/tilemap.hpp"
#include <climits>

struct Document;

namespace Tool {
	enum Type {
		BRUSH = 0,
		LINE,
		ERASE,
		PAN
	};

	inline const char* ToolTypeToString(Tool::Type t) {
		switch (t) {
			case BRUSH: return "Brush";
			case LINE:  return "Line";
			case ERASE: return "Eraser";
			case PAN:   return "Pan";
			default:    return "Unknown";
		}
	}

	struct Manager {
		i32 BrushSize = 5;
		bool isBrushCircle = true;
		Tool::Type ToolType = Tool::Type::BRUSH;
		Tile selectedTile = { 0 };

		f32       ViewPortScale = 1;
		VectorI32 ViewPortSize;
		RectF32   ViewPort; // viewport position & scaled size
		void UpdateViewportScale();

		RectI32 OnMouseDown(i32 x, i32 y, Document& doc);
		RectI32 OnMouseMove(i32 x, i32 y, Document& doc);
		RectI32 OnMouseUp(i32 x, i32 y, Document& doc);

	private:
		VectorI32 MousePosWhenDown = { INT_MIN, INT_MIN };
		VectorI32 MousePosLast = { INT_MIN, INT_MIN };
	};
};

#endif // ASCIIPEN_TOOLS_TOOLMANAGER_HPP_INCLUDED_

