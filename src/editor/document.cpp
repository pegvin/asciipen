#include "editor/document.hpp"

bool EditorDocument::CreateNew(i32 DocWidth, i32 DocHeight, const char* tileSetPath) {
	if (DocWidth < 1 || DocHeight < 1 || tileSetPath == NULL) return false;
	if (doc.CreateNew(DocWidth, DocHeight, tileSetPath) == false) return false;

	render = new Pixel[doc.TileMapWidthPixels() * doc.TileMapHeightPixels()];
	if (render == nullptr) goto exit_failure;

	renderTex = new ImBase::Texture(
		doc.TileMapWidthPixels(),
		doc.TileMapHeightPixels(),
		(u8*)render
	);
	if (renderTex == nullptr) goto exit_failure;

	tileSetTex = new ImBase::Texture(
		doc.tileSet.GetWidthPixels(),
		doc.tileSet.GetHeightPixels(),
		(u8*)doc.tileSet.Pixels
	);
	if (tileSetTex == nullptr) goto exit_failure;

	goto exit_success;

exit_failure:
	if (render != nullptr) delete[] render;
	if (renderTex != nullptr) delete renderTex;
	if (tileSetTex != nullptr) delete tileSetTex;
	doc.Destroy();
	return false;

exit_success:
	return true;
}

void EditorDocument::Destroy() {
	doc.Destroy();
	if (render != nullptr) delete[] render;
	if (renderTex != nullptr) delete renderTex;
	if (tileSetTex != nullptr) delete tileSetTex;
}

