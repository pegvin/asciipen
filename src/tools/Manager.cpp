#include "imgui/imgui.h"
#include "pixel/pixel.hpp"
#include "imbase/texture.hpp"
#include "tileset/tilemap.hpp"
#include "tileset/tileset.hpp"
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

static TileMap*         tMap = nullptr;
static Pixel*           DocRender = nullptr;
static ImBase::Texture* DocTex = nullptr;
static ImBase::Texture* TileSetTex = nullptr;
static i16              SelectedTile = 0;

i16 Manager::GetSelectedTile() {
	return SelectedTile;
}

void Manager::SetSelectedTile(i16 idx) {
	SelectedTile = idx;
}

ImTextureID Manager::GetDocTex() {
	return reinterpret_cast<ImTextureID>(DocTex->id);
}

ImTextureID Manager::GetTileSetTex() {
	return reinterpret_cast<ImTextureID>(TileSetTex->id);
}

const TileMap& Manager::GetTileMap() {
	return *tMap;
}

void Manager::CreateNew(
	u16 _row, u16 _col,
	u16 _tRows, u16 _tCols,
	u16 _tWidth, u16 _tHeight,
	const char* tileSetFilePath
) {
	if (_row < 1 || _col < 1 || tileSetFilePath == NULL) return;

	Manager::Release();

	tMap = new TileMap(_row, _col, tileSetFilePath, _tRows, _tCols, _tWidth, _tHeight);
	DocRender = new Pixel[tMap->GetTotalPixels()];
	DocTex = new ImBase::Texture(tMap->GetWidthPixels(), tMap->GetHeightPixels(), nullptr);
	TileSetTex = new ImBase::Texture(
		tMap->tSet.tileWidth * tMap->tSet.tileSetWidth,
		tMap->tSet.tileHeight * tMap->tSet.tileSetHeight,
		nullptr
	);

	RectI32 _dirtyArea = { 0, 0, _row, _col };
	tMap->Render(_dirtyArea, DocRender, tMap->GetWidthPixels());
	TileSetTex->Update((u8*)tMap->tSet.tilesPixels);
	DocTex->Update((u8*)DocRender);

	Manager::SetViewPortSize(tMap->GetWidthPixels(), tMap->GetHeightPixels());

	ImGuiIO& io = ImGui::GetIO();
	ViewPort.x = (io.DisplaySize.x / 2) - (ViewPort.x / 2);
	ViewPort.y = (io.DisplaySize.y / 2) - (ViewPort.y / 2);
}

void Manager::ProcessFrame() {
	ImGuiIO& io = ImGui::GetIO();
	static ImVec2 MousePosRel;
	MousePosRel = {
		(f32)i32(((io.MousePos.x - ViewPort.x) / tMap->tSet.tileWidth) / ViewPortScale),
		(f32)i32(((io.MousePos.y - ViewPort.y) / tMap->tSet.tileHeight) / ViewPortScale)
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
		if (MousePosRel.x >= 0 && MousePosRel.y >= 0 && MousePosRel.x < tMap->tMapRows && MousePosRel.y < tMap->tMapCols) {
			tMap->tiles[(i32)((MousePosRel.y * tMap->tMapRows) + MousePosRel.x)].TileIndex = SelectedTile;

			static RectI32 _dirtyArea = { 0, 0, 1, 1 };
			_dirtyArea.x = (i32)MousePosRel.x;
			_dirtyArea.y = (i32)MousePosRel.y;
			tMap->Render(_dirtyArea, DocRender, tMap->GetWidthPixels());
			DocTex->Update((u8*)DocRender);
		}
	}
}

void Manager::Release() {
	if (DocRender) delete[] DocRender;
	if (DocTex) delete DocTex;
	if (TileSetTex) delete TileSetTex;
	if (tMap) delete tMap;
}
