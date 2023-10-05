#ifndef ASCIIPEN_TOOLS_TOOL_HPP_INCLUDED_
#define ASCIIPEN_TOOLS_TOOL_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "doc/tilemap.hpp"

namespace Tool {
	RectI32 Draw(i32 x, i32 y, TileMap& map, Tile t, u16 StrokeSize, bool isCircle);
};

#endif // ASCIIPEN_TOOLS_TOOL_HPP_INCLUDED_

