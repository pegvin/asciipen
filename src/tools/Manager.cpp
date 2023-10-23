#include "imgui/imgui.h"
#include "pixel/pixel.hpp"
#include "imbase/texture.hpp"
#include "doc/doc.hpp"
#include "tools/Manager.hpp"
#include "types.hpp"

static Document Doc;
static Pixel*           DocRender = nullptr;
static ImBase::Texture* DocTex = nullptr;
static ImBase::Texture* TileSetTex = nullptr;

Document& Manager::GetDocument() {
	return Doc;
}

ImTextureID Manager::GetDocTex() {
	return reinterpret_cast<ImTextureID>(DocTex->id);
}

ImTextureID Manager::GetTileSetTex() {
	return reinterpret_cast<ImTextureID>(TileSetTex->id);
}

bool Manager::CreateNew(i32 _tileMapWidth, i32 _tileMapHeight, const char* tileSetFilePath) {
	if (_tileMapWidth < 1 || _tileMapHeight < 1 || tileSetFilePath == NULL) return false;
	if (!Doc.tileSet.Create_FromFile(tileSetFilePath)) return false;

	Doc.tileMap.Create(_tileMapWidth, _tileMapHeight);
	DocRender = new Pixel[Doc.TileMapWidthPixels() * Doc.TileMapHeightPixels()];
	DocTex = new ImBase::Texture(
		Doc.TileMapWidthPixels(),
		Doc.TileMapHeightPixels(),
		nullptr
	);
	TileSetTex = new ImBase::Texture(
		Doc.tileSet.GetWidthPixels(),
		Doc.tileSet.GetHeightPixels(),
		nullptr
	);

	RectI32 _dirtyArea = { 0, 0, _tileMapWidth, _tileMapHeight };
	Doc.Render(
		_dirtyArea, DocRender,
		Doc.TileMapWidthPixels(),
		Doc.TileMapHeightPixels()
	);
	TileSetTex->Update((u8*)Doc.tileSet.Pixels);
	DocTex->Update((u8*)DocRender);

	Doc.toolManager.ViewPortSize.x = Doc.TileMapWidthPixels();
	Doc.toolManager.ViewPortSize.y = Doc.TileMapHeightPixels();

	ImGuiIO& io = ImGui::GetIO();
	Doc.toolManager.ViewPort.x = (io.DisplaySize.x / 2) - (Doc.toolManager.ViewPort.x / 2);
	Doc.toolManager.ViewPort.y = (io.DisplaySize.y / 2) - (Doc.toolManager.ViewPort.y / 2);

	return true;
}

void Manager::ProcessFrame() {
	ImGuiIO& io = ImGui::GetIO();
	static Tool::Type LastTool = Doc.toolManager.ToolType;

	static ImVec2 MPosRel2Pixel; // mouse position relative to viewport
	static ImVec2 MPosRel2Tile;  // mouse position relative to tiles
	MPosRel2Pixel = {
		io.MousePos.x - Doc.toolManager.ViewPort.x,
		io.MousePos.y - Doc.toolManager.ViewPort.y,
	};
	MPosRel2Tile = {
		(f32)i32((MPosRel2Pixel.x / Doc.tileSet.TileWidth) / Doc.toolManager.ViewPortScale),
		(f32)i32((MPosRel2Pixel.y / Doc.tileSet.TileHeight) / Doc.toolManager.ViewPortScale)
	};

	if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
		LastTool = Doc.toolManager.ToolType;
		Doc.toolManager.ToolType = Tool::Type::PAN;
	} else if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
		Doc.toolManager.ToolType = LastTool;
	} else if (ImGui::IsKeyPressed(ImGuiKey_B, false)) {
		Doc.toolManager.ToolType = Tool::Type::BRUSH;
	} else if (ImGui::IsKeyPressed(ImGuiKey_E, false)) {
		Doc.toolManager.ToolType = Tool::Type::ERASE;
	} else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (!ImGui::IsAnyItemFocused() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
			if (ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
				if (io.MouseWheel > 0)
					Doc.toolManager.BrushSize++;
				else if (io.MouseWheel < 0 && Doc.toolManager.BrushSize > 1)
					Doc.toolManager.BrushSize--;
			} else {
				if (io.MouseWheel > 0)
					Doc.toolManager.ViewPortScale += 0.25;
				else if (io.MouseWheel < 0)
					Doc.toolManager.ViewPortScale -= 0.25;

				Doc.toolManager.UpdateViewportScale();
			}
		}
	}

	bool MouseIsInBounds = MPosRel2Pixel.x >= 0 && MPosRel2Pixel.y >= 0 &&
	                       MPosRel2Pixel.x < Doc.toolManager.ViewPort.w &&
	                       MPosRel2Pixel.y < Doc.toolManager.ViewPort.h;

	if (MouseIsInBounds) {
		// ImGui::SetMouseCursor(ImGuiMouseCursor_None);
		ImVec2 TopLeft = {
			Doc.toolManager.ViewPort.x + (MPosRel2Tile.x * Doc.tileSet.TileWidth * Doc.toolManager.ViewPortScale),
			Doc.toolManager.ViewPort.y + (MPosRel2Tile.y * Doc.tileSet.TileHeight * Doc.toolManager.ViewPortScale)
		};
		ImVec2 BottomRight = {
			TopLeft.x + (Doc.tileSet.TileWidth * Doc.toolManager.ViewPortScale),
			TopLeft.y + (Doc.tileSet.TileHeight * Doc.toolManager.ViewPortScale)
		};
		ImGui::GetForegroundDrawList()->AddRect(TopLeft, BottomRight, 0xFFFFFFFF);

		RectI32 dirty;
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
			dirty = Doc.toolManager.OnMouseDown(MPosRel2Pixel.x, MPosRel2Pixel.y, Doc);
			goto doRender;
		}
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && io.MouseDelta.x != 0 && io.MouseDelta.y != 0) {
			dirty = Doc.toolManager.OnMouseMove(MPosRel2Pixel.x, MPosRel2Pixel.y, Doc);
			goto doRender;
		}
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
			dirty = Doc.toolManager.OnMouseUp(MPosRel2Pixel.x, MPosRel2Pixel.y, Doc);
			goto doRender;
		}
		goto skipRender;

doRender:
			Doc.Render(dirty, DocRender, Doc.TileMapWidthPixels(), Doc.TileMapHeightPixels());
			DocTex->Update((u8*)DocRender);
skipRender:
		void();
	}
}

void Manager::Release() {
	if (DocRender) delete[] DocRender;
	if (DocTex) delete DocTex;
	if (TileSetTex) delete TileSetTex;
}

