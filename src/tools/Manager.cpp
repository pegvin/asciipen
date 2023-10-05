#include "imgui/imgui.h"
#include "pixel/pixel.hpp"
#include "imbase/texture.hpp"
#include "doc/doc.hpp"
#include "tools/Manager.hpp"
#include "types.hpp"

static ToolType tType = ToolType::PENCIL;
static ToolType LastToolType = tType;
static f32    ViewPortScale = 1;
static ImVec2 ViewPortSize = { 0, 0 };
static ImVec4 ViewPort = { 0, 0, 0, 0 };

void Manager::SetToolType(ToolType _t) {
	tType = _t;
}

ToolType Manager::GetToolType() {
	return tType;
}

void Manager::SetViewPortSize(u32 w, u32 h) {
	ViewPortSize = { (f32)w, (f32)h };
}

void Manager::SetViewPortScale(f32 _scale) {
	ViewPortScale = _scale > 1.0f ? _scale : 1.0f;

	// Ensures That The ViewPort is Centered From The Center
	ImVec2 CurrRectCenter = {
		(ViewPort.z / 2) + ViewPort.x, (ViewPort.w / 2) + ViewPort.y
	};
	ImVec2 NewRectCenter = {
		(ViewPortSize.x * ViewPortScale / 2) + ViewPort.x,
		(ViewPortSize.y * ViewPortScale / 2) + ViewPort.y
	};
	ViewPort.x -= NewRectCenter.x - CurrRectCenter.x;
	ViewPort.y -= NewRectCenter.y - CurrRectCenter.y;

	// Update The Size Of The ViewPort
	ViewPort.z = ViewPortSize.x * ViewPortScale;
	ViewPort.w = ViewPortSize.y * ViewPortScale;
}

f32 Manager::GetViewPortScale() {
	return ViewPortScale;
}

const ImVec4& Manager::GetViewPort() {
	return ViewPort;
}

static Document Doc;
static Pixel*           DocRender = nullptr;
static ImBase::Texture* DocTex = nullptr;
static ImBase::Texture* TileSetTex = nullptr;
static i32              SelectedTile = 0;

i16 Manager::GetSelectedTile() {
	return SelectedTile;
}

void Manager::SetSelectedTile(i16 idx) {
	SelectedTile = idx;
}

const Document& Manager::GetDocument() {
	return Doc;
}

ImTextureID Manager::GetDocTex() {
	return reinterpret_cast<ImTextureID>(DocTex->id);
}

ImTextureID Manager::GetTileSetTex() {
	return reinterpret_cast<ImTextureID>(TileSetTex->id);
}

void Manager::CreateNew(
	u16 _tileMapWidth, u16 _tileMapHeight,
	u16 _tileSetWidth, u16 _tileSetHeight,
	u16 _tileWidth, u16 _tileHeight,
	const char* tileSetFilePath
) {
	if (_tileMapWidth < 1 || _tileMapHeight < 1 || tileSetFilePath == NULL) return;

	Manager::Release();

	Doc.tileMap.Create(_tileMapWidth, _tileMapHeight);
	Doc.tileSet.Create_FromFile(tileSetFilePath, _tileSetWidth, _tileSetHeight, _tileWidth, _tileHeight);
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

	RectI32 _dirtyArea = { 0, 0, _tileMapWidth, _tileHeight };
	Doc.Render(
		_dirtyArea, DocRender,
		Doc.TileMapWidthPixels(),
		Doc.TileMapHeightPixels()
	);
	TileSetTex->Update((u8*)Doc.tileSet.Pixels);
	DocTex->Update((u8*)DocRender);

	Manager::SetViewPortSize(
		Doc.TileMapWidthPixels(),
		Doc.TileMapHeightPixels()
	);

	ImGuiIO& io = ImGui::GetIO();
	ViewPort.x = (io.DisplaySize.x / 2) - (ViewPort.x / 2);
	ViewPort.y = (io.DisplaySize.y / 2) - (ViewPort.y / 2);
}

void Manager::ProcessFrame() {
	ImGuiIO& io = ImGui::GetIO();
	static ImVec2 MousePosRel;
	MousePosRel = {
		(f32)i32(((io.MousePos.x - ViewPort.x) / Doc.tileSet.TileWidth) / ViewPortScale),
		(f32)i32(((io.MousePos.y - ViewPort.y) / Doc.tileSet.TileHeight) / ViewPortScale)
	};

	if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
		LastToolType = Manager::GetToolType();
		Manager::SetToolType(ToolType::PAN);
	} else if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
		Manager::SetToolType(LastToolType);
	} else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (!ImGui::IsAnyItemFocused() && !ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow)) {
			if (io.MouseWheel > 0)
				Manager::SetViewPortScale(Manager::GetViewPortScale() + 0.25);
			else if (io.MouseWheel < 0)
				Manager::SetViewPortScale(Manager::GetViewPortScale() - 0.25);
		}
	}

	if (tType == PAN) {
		ViewPort.x += io.MouseDelta.x;
		ViewPort.y += io.MouseDelta.y;
	} else if (tType == ToolType::PENCIL && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (
			MousePosRel.x >= 0 &&
			MousePosRel.y >= 0 &&
			MousePosRel.x < Doc.tileMap.Width &&
			MousePosRel.y < Doc.tileMap.Height
		) {
			i32 tileMapTileIdx = (i32)((MousePosRel.y * Doc.tileMap.Width) + MousePosRel.x);
			Doc.tileMap.Tiles[tileMapTileIdx].Index = SelectedTile;

			static RectI32 _dirtyArea = { 0, 0, 1, 1 };
			_dirtyArea.x = (i32)MousePosRel.x;
			_dirtyArea.y = (i32)MousePosRel.y;

			Doc.Render(_dirtyArea, DocRender, Doc.TileMapWidthPixels(), Doc.TileMapHeightPixels());
			DocTex->Update((u8*)DocRender);
		}
	}
}

void Manager::Release() {
	if (DocRender) delete[] DocRender;
	if (DocTex) delete DocTex;
	if (TileSetTex) delete TileSetTex;
}

