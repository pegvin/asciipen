#ifndef CSP_TOOLS_TOOLMANAGER_HPP_INCLUDED_
#define CSP_TOOLS_TOOLMANAGER_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "imgui/imgui.h"
#include "doc/doc.hpp"

enum ToolType {
	NONE = 0,
	PENCIL,
	ERASER,
	PAN
};

namespace Manager {
	void CreateNew(
		u16 tileMapWidth, u16 tileMapHeight,
		u16 tileSetWidth, u16 tileSetHeight,
		u16 tileWidth, u16 tileHeight,
		const char* tileSetFilePath
	);
	const Document& GetDocument();
	ImTextureID GetDocTex();
	ImTextureID GetTileSetTex();

	const TileMap& GetTileMap();

	i16 GetSelectedTile();
	void SetSelectedTile(i16 idx);

	void SetToolType(ToolType _T);
	ToolType GetToolType();

	const ImVec4& GetViewPort();
	void SetViewPortSize(u32 w, u32 h); // Set Actual Size Of The ViewPort
	void SetViewPortScale(f32 _scale);  // Set How Much The ViewPort Is Scaled
	f32  GetViewPortScale();

	void ProcessFrame();

	void Release();
}

#endif // CSP_TOOLS_TOOLMANAGER_HPP_INCLUDED_
