#include "imgui/imgui.h"
#include "imbase/window.hpp"
#include "imbase/texture.hpp"
#include "editor/document.hpp"
#include "types.hpp"

void CenterDocument(EditorDocument& editorDoc) {
	const ImGuiIO& io = ImGui::GetIO();
	editorDoc.toolManager.ViewPortSize.x = editorDoc.doc.TileMapWidthPixels();
	editorDoc.toolManager.ViewPortSize.y = editorDoc.doc.TileMapHeightPixels();
	editorDoc.toolManager.ViewPort.x = (io.DisplaySize.x / 2) - (editorDoc.toolManager.ViewPort.w / 2);
	editorDoc.toolManager.ViewPort.y = (io.DisplaySize.y / 2) - (editorDoc.toolManager.ViewPort.h / 2);
}

int main(void) {
	if (ImBase::Window::Init(700, 500, "asciipen") != 0) {
		return 1;
	}
	ImBase::Window::NewFrame();
	ImBase::Window::EndFrame();
	ImBase::Window::SetMaxFPS(60);

	const ImGuiIO& io = ImGui::GetIO();

	EditorDocument editorDoc;
	if (editorDoc.CreateNew(40, 25, "./data/tilesets/Commodore_64.tar") == false) {
		ImBase::Window::Destroy();
		return 1;
	}

	editorDoc.toolManager.ViewPortSize.x = editorDoc.doc.TileMapWidthPixels();
	editorDoc.toolManager.ViewPortSize.y = editorDoc.doc.TileMapHeightPixels();
	editorDoc.toolManager.ViewPort.x = (io.DisplaySize.x / 2) - (editorDoc.toolManager.ViewPort.w / 2);
	editorDoc.toolManager.ViewPort.y = (io.DisplaySize.y / 2) - (editorDoc.toolManager.ViewPort.h / 2);

	Pixel BgColor = { 0x48, 0x3A, 0xAA, 0xFF };
	ImBase::Texture BgTex(1, 1, &BgColor.r);

	ImVec2 MouseRel2Pixel; // mouse position relative to viewport
	ImVec2 MouseRel2Tile;  // mouse position relative to tiles
	bool MouseIsInBounds = false;
	Tool::Type LastTool = editorDoc.toolManager.ToolType;

	while (!ImBase::Window::ShouldClose()) {
		ImBase::Window::NewFrame();

		MouseRel2Pixel = {
			io.MousePos.x - editorDoc.toolManager.ViewPort.x,
			io.MousePos.y - editorDoc.toolManager.ViewPort.y,
		};
		MouseRel2Tile = {
			(f32)i32((MouseRel2Pixel.x / editorDoc.doc.tileSet.TileWidth) / editorDoc.toolManager.ViewPortScale),
			(f32)i32((MouseRel2Pixel.y / editorDoc.doc.tileSet.TileHeight) / editorDoc.toolManager.ViewPortScale)
		};
		MouseIsInBounds = MouseRel2Pixel.x >= 0 && MouseRel2Pixel.y >= 0 && MouseRel2Pixel.x < editorDoc.toolManager.ViewPort.w && MouseRel2Pixel.y < editorDoc.toolManager.ViewPort.h;

		if (ImGui::IsKeyPressed(ImGuiKey_Space, false)) {
			LastTool = editorDoc.toolManager.ToolType;
			editorDoc.toolManager.ToolType = Tool::Type::PAN;
		} else if (ImGui::IsKeyReleased(ImGuiKey_Space)) {
			editorDoc.toolManager.ToolType = LastTool;
		} else if (ImGui::IsKeyPressed(ImGuiKey_B, false)) {
			editorDoc.toolManager.ToolType = Tool::Type::BRUSH;
		} else if (ImGui::IsKeyPressed(ImGuiKey_E, false)) {
			editorDoc.toolManager.ToolType = Tool::Type::ERASE;
		} else if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
			if (ImGui::IsKeyDown(ImGuiMod_Ctrl)) {
				if (io.MouseWheel > 0)
					editorDoc.toolManager.BrushSize++;
				else if (io.MouseWheel < 0 && editorDoc.toolManager.BrushSize > 1)
					editorDoc.toolManager.BrushSize--;
			} else {
				if (io.MouseWheel > 0)
					editorDoc.toolManager.ViewPortScale += 0.25;
				else if (io.MouseWheel < 0)
					editorDoc.toolManager.ViewPortScale -= 0.25;

				editorDoc.toolManager.UpdateViewportScale();
			}
		}

		if (MouseIsInBounds) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			ImVec2 TopLeft = {
				editorDoc.toolManager.ViewPort.x + (MouseRel2Tile.x * editorDoc.doc.tileSet.TileWidth * editorDoc.toolManager.ViewPortScale),
				editorDoc.toolManager.ViewPort.y + (MouseRel2Tile.y * editorDoc.doc.tileSet.TileHeight * editorDoc.toolManager.ViewPortScale)
			};
			ImVec2 BottomRight = {
				TopLeft.x + (editorDoc.doc.tileSet.TileWidth * editorDoc.toolManager.ViewPortScale),
				TopLeft.y + (editorDoc.doc.tileSet.TileHeight * editorDoc.toolManager.ViewPortScale)
			};
			ImGui::GetForegroundDrawList()->AddRect(TopLeft, BottomRight, 0xFFFFFFFF);

			RectI32 dirty;
			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
				dirty = editorDoc.toolManager.OnMouseDown(MouseRel2Pixel.x, MouseRel2Pixel.y, editorDoc.doc);
				goto doRender;
			}
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && io.MouseDelta.x != 0 && io.MouseDelta.y != 0) {
				dirty = editorDoc.toolManager.OnMouseMove(MouseRel2Pixel.x, MouseRel2Pixel.y, editorDoc.doc);
				goto doRender;
			}
			if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
				dirty = editorDoc.toolManager.OnMouseUp(MouseRel2Pixel.x, MouseRel2Pixel.y, editorDoc.doc);
				goto doRender;
			}
			goto skipRender;

doRender:
			if (!(dirty.x > 0 && dirty.y > 0 && dirty.w > 0 && dirty.h > 0)) goto skipRender;

			editorDoc.doc.Render(dirty, editorDoc.render, editorDoc.doc.TileMapWidthPixels(), editorDoc.doc.TileMapHeightPixels());
			editorDoc.renderTex->Update((u8*)editorDoc.render);
skipRender:
			void();
		}

		static ImVec2 MainMenuSize;
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					editorDoc.Destroy();
					if (editorDoc.CreateNew(40, 40, "./data/tilesets/Commodore_64.tar") == false) {
						return 1;
					}
					CenterDocument(editorDoc);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View")) {
				if (ImGui::BeginMenu("Viewport")) {
					if (ImGui::MenuItem("Center")) {
						CenterDocument(editorDoc);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			MainMenuSize = ImGui::GetWindowSize();
			ImGui::EndMainMenuBar();
		}

		static ImVec2 SideWinSize;
		ImGui::SetNextWindowPos({ 0, MainMenuSize.y });
		ImGui::SetNextWindowSize({ 405.0f, io.DisplaySize.y - MainMenuSize.y }, ImGuiCond_Once);
		ImGui::SetNextWindowSizeConstraints({ 120, -1 }, { io.DisplaySize.x / 2, -1 });
		if (ImGui::Begin("SideWindow", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
			const float TileSetPaletteScale = ImGui::GetContentRegionAvail().x / editorDoc.doc.tileSet.GetWidthPixels();

			ImGui::Image(
				reinterpret_cast<ImTextureID>(editorDoc.tileSetTex->id),
				{ editorDoc.doc.tileSet.GetWidthPixels() * TileSetPaletteScale, editorDoc.doc.tileSet.GetHeightPixels() * TileSetPaletteScale }
			);
			auto _ImageRectMin = ImGui::GetItemRectMin();
			auto _ImageIsHovered = ImGui::IsItemHovered() && ImGui::IsWindowHovered();
			f32 BorderPadding = 1.2f * TileSetPaletteScale;
			ImVec2 tilePos = {
				(f32)(editorDoc.toolManager.selectedTile.Index % editorDoc.doc.tileSet.TileSetWidth),
				(f32)(i32)((f32)editorDoc.toolManager.selectedTile.Index / editorDoc.doc.tileSet.TileSetHeight) // cursed casting
			};
			ImVec2 iRectMin = {
				_ImageRectMin.x + (tilePos.x * editorDoc.doc.tileSet.TileWidth * TileSetPaletteScale) - BorderPadding,
				_ImageRectMin.y + (tilePos.y * editorDoc.doc.tileSet.TileHeight * TileSetPaletteScale) - BorderPadding
			};
			ImVec2 iRectMax = {
				iRectMin.x + (editorDoc.doc.tileSet.TileWidth * TileSetPaletteScale) + (BorderPadding * 2),
				iRectMin.y + (editorDoc.doc.tileSet.TileHeight * TileSetPaletteScale) + (BorderPadding * 2)
			};
			ImGui::GetWindowDrawList()->AddRect(iRectMin, iRectMax, 0xFF0000FF, 0.0f, 0, 1.2f * TileSetPaletteScale);

			ImGui::Text("Selected: %d, %d (%d)", (i32)tilePos.x, (i32)tilePos.y, editorDoc.toolManager.selectedTile.Index);

			if (_ImageIsHovered) {
				ImVec2 _HoveredTilePos = {
					(io.MousePos.x - _ImageRectMin.x) / (editorDoc.doc.tileSet.TileWidth * TileSetPaletteScale),
					(io.MousePos.y - _ImageRectMin.y) / (editorDoc.doc.tileSet.TileHeight * TileSetPaletteScale)
				};
				i32 _HoveredTilePos1D = ((i32)_HoveredTilePos.y * editorDoc.doc.tileSet.TileSetWidth) + (i32)_HoveredTilePos.x;

				ImGui::Text("Hovering: %d, %d (%d)", (i32)_HoveredTilePos.x, (i32)_HoveredTilePos.y, _HoveredTilePos1D);
				if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
					editorDoc.toolManager.selectedTile.Index = _HoveredTilePos1D;
				}
			}

			ImGui::Text("Tool: %s", Tool::ToolTypeToString(editorDoc.toolManager.ToolType));
			ImGui::Text("Brush Size: %d", editorDoc.toolManager.BrushSize);
			ImGui::Text("Zoom: %.02fx", editorDoc.toolManager.ViewPortScale);

#ifdef DEBUG_BUILD
			ImGui::Text("FPS: %.1f (%.2f ms)", io.Framerate, io.DeltaTime * 1000);
#endif

			SideWinSize = ImGui::GetWindowSize();
			ImGui::End();
		}

		ImGui::SetNextWindowPos({ SideWinSize.x, MainMenuSize.y });
		ImGui::SetNextWindowSize({ io.DisplaySize.x - SideWinSize.x, io.DisplaySize.y - MainMenuSize.y });
		if (ImGui::Begin("MainWindow", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse)) {
			// ViewPort Rendering
			ImDrawList* winDrwList = ImGui::GetWindowDrawList();
			const RectF32& ViewPort = editorDoc.toolManager.ViewPort;
			winDrwList->AddRect(
				{ ViewPort.x - 1, ViewPort.y - 1 },
				{ ViewPort.w + ViewPort.x + 1, ViewPort.h + ViewPort.y + 1 },
				ImGui::GetColorU32(ImGuiCol_Border), 0.0f, 0, 1.0f
			);
			winDrwList->AddImage(
				reinterpret_cast<ImTextureID>(BgTex.id),
				{ ViewPort.x, ViewPort.y },
				{ ViewPort.w + ViewPort.x, ViewPort.h + ViewPort.y }
			);
			winDrwList->AddImage(
				reinterpret_cast<ImTextureID>(editorDoc.renderTex->id),
				{ ViewPort.x, ViewPort.y },
				{ ViewPort.w + ViewPort.x, ViewPort.h + ViewPort.y }
			);

			ImGui::End();
		}

		ImBase::Window::EndFrame();
	}

	editorDoc.Destroy();
	ImBase::Window::Destroy();
	return 0;
}

