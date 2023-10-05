#include <iostream>

#include "imgui/imgui.h"
#include "imbase/window.hpp"
#include "imbase/texture.hpp"
#include "tools/Manager.hpp"
#include "types.hpp"

int main(void) {
	if (ImBase::Window::Init(700, 500, "asciipen") != 0) {
		return 1;
	}
	ImBase::Window::SetMaxFPS(60);

	Manager::CreateNew(
		40, 25, 16, 16, 16, 16,
		"./data/tilesets/Commodore_64.png"
	);
	Manager::SetViewPortScale(1);

	ImGuiIO& io = ImGui::GetIO();

	const ImVec4& ViewPort = Manager::GetViewPort();
	const Document& Doc = Manager::GetDocument();
	const u8 BgColor[] = { 0x48, 0x3A, 0xAA, 0xFF };
	ImBase::Texture BgTex(1, 1, (u8*)BgColor);

	while (!ImBase::Window::ShouldClose()) {
		ImBase::Window::NewFrame();
		Manager::ProcessFrame();

		ImVec2 MainMenuSize;
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					std::cout << "New File!" << std::endl;
				}
				ImGui::EndMenu();
			}
			MainMenuSize = ImGui::GetWindowSize();
			ImGui::EndMainMenuBar();
		}

		// ViewPort Rendering
		ImDrawList* bgDrwList = ImGui::GetBackgroundDrawList();
		bgDrwList->AddRect(
			{ ViewPort.x - 1, ViewPort.y - 1 },
			{ ViewPort.z + ViewPort.x + 1, ViewPort.w + ViewPort.y + 1 },
			ImGui::GetColorU32(ImGuiCol_Border), 0.0f, 0, 1.0f
		);
		bgDrwList->AddImage(
			reinterpret_cast<ImTextureID>(BgTex.id),
			{ ViewPort.x, ViewPort.y },
			{ ViewPort.z + ViewPort.x, ViewPort.w + ViewPort.y }
		);
		bgDrwList->AddImage(
			Manager::GetDocTex(),
			{ ViewPort.x, ViewPort.y },
			{ ViewPort.z + ViewPort.x, ViewPort.w + ViewPort.y }
		);

		ImGui::SetNextWindowPos({
			io.DisplaySize.x - 405.0f, MainMenuSize.y
		});
		ImGui::SetNextWindowSize({
			405.0f, io.DisplaySize.y - MainMenuSize.y
		});
		if (ImGui::Begin("TileSet Palette", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus)) {
			static float TileSetPaletteScale = 1.5;
			ImGui::SliderFloat("Scale", &TileSetPaletteScale, 0.25f, 2.5f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

			ImGui::Image(
				Manager::GetTileSetTex(),
				{ 256.0f * TileSetPaletteScale, 256.0f * TileSetPaletteScale }
			);
			auto _ImageRectMin = ImGui::GetItemRectMin();
			auto _ImageIsHovered = ImGui::IsItemHovered() && ImGui::IsWindowHovered();
			ImVec2 tilePos = {
				(f32)(Manager::GetSelectedTile() % Doc.tileSet.TileSetWidth),
				(f32)(Manager::GetSelectedTile() / Doc.tileSet.TileSetHeight)
			};
			ImVec2 iRectMin = {
				_ImageRectMin.x + (tilePos.x * Doc.tileSet.TileWidth * TileSetPaletteScale),
				_ImageRectMin.y + (tilePos.y * Doc.tileSet.TileHeight * TileSetPaletteScale)
			};
			ImVec2 iRectMax = {
				iRectMin.x + (Doc.tileSet.TileWidth * TileSetPaletteScale),
				iRectMin.y + (Doc.tileSet.TileHeight * TileSetPaletteScale)
			};
			ImGui::GetWindowDrawList()->AddRect(iRectMin, iRectMax, 0xFF0000AE, 0.0f, 0, 1.2f);

			ImGui::Text("Selected: %d, %d (%d)", (i32)tilePos.x, (i32)tilePos.y, Manager::GetSelectedTile());

			if (_ImageIsHovered) {
				ImVec2 _HoveredTilePos = {
					(io.MousePos.x - _ImageRectMin.x) / (Doc.tileSet.TileWidth * TileSetPaletteScale),
					(io.MousePos.y - _ImageRectMin.y) / (Doc.tileSet.TileHeight * TileSetPaletteScale)
				};
				i32 _HoveredTilePos1D = ((i32)_HoveredTilePos.y * Doc.tileSet.TileSetWidth) + (i32)_HoveredTilePos.x;

				ImGui::Text("Hovering: %d, %d (%d)", (i32)_HoveredTilePos.x, (i32)_HoveredTilePos.y, _HoveredTilePos1D);
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
					Manager::SetSelectedTile(_HoveredTilePos1D);
				}
			}

			ImGui::End();
		}

		ImBase::Window::EndFrame();
	}

	Manager::Release();
	ImBase::Window::Destroy();
	return 0;
}

