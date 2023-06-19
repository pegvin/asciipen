#include "imgui/imgui.h"
#include "pixel/pixel.hpp"
#include "renderer/canvas.hpp"
#include "tileset/tileset.hpp"
#include "tools/Manager.hpp"

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

static u32      DocRows   = 0;
static u32      DocCols   = 0;
static u32      DocWidthPixels = 0;
static u32      DocHeightPixels = 0;
static Pixel*   DocRender = nullptr;
static Canvas*  DocTex = nullptr;
static TileSet* DocTSet = nullptr;

ImTextureID Manager::GetDocTex() {
	return reinterpret_cast<ImTextureID>(DocTex->id);
}

void Manager::CreateNew(
	u16 _row, u16 _col,
	u16 _tRows, u16 _tCols,
	u16 _tWidth, u16 _tHeight,
	const char* tileSetFilePath
) {
	if (_row < 1 || _col < 1 || tileSetFilePath == NULL) return;

	Manager::Release();

	DocRows = _row;
	DocCols = _col;
	DocTSet = new TileSet();
	DocTSet->LoadFrom(tileSetFilePath, _tRows, _tCols, _tWidth, _tHeight);
	DocWidthPixels = DocRows * DocTSet->tileWidth;
	DocHeightPixels = DocCols * DocTSet->tileHeight;
	DocRender = new Pixel[DocWidthPixels * DocHeightPixels];
	DocTex = new Canvas(DocWidthPixels, DocHeightPixels);

	for (u32 i = 0; i < DocWidthPixels * DocHeightPixels; i++) {
		DocRender[i] = { 0, 0, 0, 255 };
	}
	DocTex->Update(DocRender);

	Manager::SetViewPortSize(DocWidthPixels, DocHeightPixels);

	ImGuiIO& io = ImGui::GetIO();
	ViewPort.x = (io.DisplaySize.x / 2) - (ViewPort.x / 2);
	ViewPort.y = (io.DisplaySize.y / 2) - (ViewPort.y / 2);
}

void Manager::ProcessFrame() {
	ImGuiIO& io = ImGui::GetIO();
	static ImVec2 MousePosRel;
	MousePosRel = {
		(f32)i32(((io.MousePos.x - ViewPort.x) / DocTSet->tileWidth) / ViewPortScale),
		(f32)i32(((io.MousePos.y - ViewPort.y) / DocTSet->tileHeight) / ViewPortScale)
	};

	if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
		LastToolType = Manager::GetToolType();
		Manager::SetToolType(ToolType::PAN);
	} else if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
		Manager::SetToolType(LastToolType);
	} else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (io.MouseWheel > 0)
			Manager::SetViewPortScale(Manager::GetViewPortScale() + 0.25);
		else if (io.MouseWheel < 0)
			Manager::SetViewPortScale(Manager::GetViewPortScale() - 0.25);
	}

	if (tType == PAN) {
		ViewPort.x += io.MouseDelta.x;
		ViewPort.y += io.MouseDelta.y;
	} else if (tType == ToolType::PENCIL && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
		if (MousePosRel.x >= 0 && MousePosRel.y >= 0 && MousePosRel.x < DocRows && MousePosRel.y < DocCols) {
			DocTSet->CopyTile(
				1, 0, DocRender,
				MousePosRel.x * DocTSet->tileWidth, MousePosRel.y * DocTSet->tileHeight,
				DocWidthPixels, DocHeightPixels
			);
			DocTex->Update(DocRender);
		}
	}

	// ViewPort Rendering
	ImGui::GetBackgroundDrawList()->AddRect(
		{ ViewPort.x - 1, ViewPort.y - 1 },
		{ ViewPort.z + ViewPort.x + 1, ViewPort.w + ViewPort.y + 1 },
		ImGui::GetColorU32(ImGuiCol_Border), 0.0f, 0, 1.0f
	);
	ImGui::GetBackgroundDrawList()->AddImage(
		Manager::GetDocTex(),
		{ ViewPort.x, ViewPort.y },
		{ ViewPort.z + ViewPort.x, ViewPort.w + ViewPort.y }
	);
}

void Manager::Release() {
	if (DocRender) delete[] DocRender;
	if (DocTex) delete DocTex;
	if (DocTSet) delete DocTSet;
}
