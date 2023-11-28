#ifndef CSP_EDITOR_DOCUMENT_HPP_INCLUDED_
#define CSP_EDITOR_DOCUMENT_HPP_INCLUDED_ 1
#pragma once

#include "doc/doc.hpp"
#include "tools/ToolManager.hpp"
#include "imbase/texture.hpp"

struct EditorDocument {
	Document doc;
	Tool::Manager toolManager;
	Pixel* render = nullptr;
	ImBase::Texture* renderTex = nullptr;
	ImBase::Texture* tileSetTex = nullptr;

	bool CreateNew(i32 DocWidth, i32 DocHeight, const char* tileSetPath);
	void Destroy();
};

#endif // CSP_EDITOR_DOCUMENT_HPP_INCLUDED_

