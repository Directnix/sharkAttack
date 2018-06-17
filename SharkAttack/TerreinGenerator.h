#ifndef TERREIN_H
#define TERREIN_H

#include <string>
#include <vector>
#include "ObjModel.h"

#include "stb_image.h"


class TerreinGenerator {
public:
	TerreinGenerator(std::string, std::string);
	~TerreinGenerator();

	void generateHeightMap(std::string);
	void drawTerrein();

	bool isFree(Vec2f);

private:
	Texture*  terreinTexture;
	Texture*  waterTexture;
	std::vector<Vert> terreinVertices;
	std::vector<Vert> waterVertices;

	ObjModel* grasm;

	float waterLevel;

	float getHeight(int x, int y, stbi_uc * imgData, int width);
};

#endif // !
