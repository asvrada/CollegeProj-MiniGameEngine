#include "ModelInfo.h"

int ModelInfo::LoadModel(char *fileName, LPCWSTR texturename) {
	HBITMAP bmp_texture = (HBITMAP)LoadImage(NULL, texturename, IMAGE_BITMAP, 512, 512, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (bmp_texture == NULL) {
		return ERROR;
	}
	hdc_texture = CreateCompatibleDC(NULL);
	SelectObject(hdc_texture, bmp_texture);

	ifstream file(fileName);
	if (!file.is_open()) {
		file.close();
		return ERROR;
	}
	float texture_coord_x = -1, texture_coord_y = -1, texture_coord_z = -1;
	float vertice_x = -1, vertice_y = -1, vertice_z = -1;
	int indice_x = -1, indice_y = -1, indice_z = -1;
	int u = -1, v = -1, w = -1;

	while (!file.eof()) {
		string head;
		file >> head;
		//vertices and indices
		if (head.size() == 1) {
			switch (*(head.begin())) {
			case 'v':
				file >> vertice_x >> vertice_y >> vertice_z;
				vertices.push_back(Vector3(vertice_x, vertice_y, vertice_z));
				break;
			case 'f':
				file >> indice_x;
				file.ignore(1, '/');
				file >> u >> indice_y;
				file.ignore(1, '/');
				file >> v >> indice_z;
				file.ignore(1, '/');
				file >> w;
				indices.push_back(Vector2<int>(indice_x - 1, u - 1));
				indices.push_back(Vector2<int>(indice_y - 1, v - 1));
				indices.push_back(Vector2<int>(indice_z - 1, w - 1));
				break;
			default:
				file.ignore(100, '\n');
				break;
			}
		}
		//texture coord
		else if (head.size() == 2) {
			file >> texture_coord_x >> texture_coord_y >> texture_coord_z;
			uv.push_back(Vector2<float>(texture_coord_x, texture_coord_y));
		}
	}
	file.close();
	return OK;
}
