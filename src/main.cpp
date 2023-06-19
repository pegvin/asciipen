#include <iostream>

#include "app/app.hpp"
#include "imgui/imgui.h"
#include "tools/Manager.hpp"
#include "types.hpp"

int main(int argc, char **argv) {
	if (App::Init(700, 500) != 0) {
		return 1;
	}

	ImGuiIO& io = ImGui::GetIO();

	Manager::CreateNew(
		40, 25, 16, 16, 16, 16,
		"./data/tilesets/Commodore_64.png"
	);
	Manager::SetViewPortScale(1);
	const ImVec4& vp = Manager::GetViewPort();

	while (!App::ShouldClose()) {
		App::NewFrame();
		Manager::ProcessFrame();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					std::cout << "New File!" << std::endl;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		App::EndFrame();
	}

	Manager::Release();
	App::Release();
	return 0;
}
