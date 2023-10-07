#ifndef CSP_TOOLS_TOOLMANAGER_HPP_INCLUDED_
#define CSP_TOOLS_TOOLMANAGER_HPP_INCLUDED_ 1
#pragma once

#include "types.hpp"
#include "imgui/imgui.h"
#include "doc/doc.hpp"

namespace Manager {
	bool CreateNew(u16 tileMapWidth, u16 tileMapHeight, const char* tileSetFilePath);
	Document& GetDocument();
	ImTextureID GetDocTex();
	ImTextureID GetTileSetTex();

	void ProcessFrame();
	void Release();
}

#endif // CSP_TOOLS_TOOLMANAGER_HPP_INCLUDED_

