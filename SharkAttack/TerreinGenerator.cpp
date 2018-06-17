#include "TerreinGenerator.h"
#include "GameObject.h"

std::vector<std::vector<bool>>area;

TerreinGenerator::TerreinGenerator(std::string tfile, std::string wfile)
{

	grasm = new ObjModel("res\\models\\grass\\grass.obj");
	this->terreinTexture = new Texture(tfile);
	this->waterTexture = new Texture(wfile);
	waterLevel = 1.2f;
}

TerreinGenerator::~TerreinGenerator()
{
	delete terreinTexture;
	delete grasm;
	terreinVertices.clear();
}

float TerreinGenerator::getHeight(int x, int y, stbi_uc * imgData, int width)
{
	return imgData[(x + width * y) * 4];
}


void TerreinGenerator::generateHeightMap(std::string hmap)
{
	int width, height, bpp;
	stbi_uc* imgData = stbi_load(hmap.c_str(), &width, &height, &bpp, 4);

	float textureScale = 1;


	for (int x = 0; x < width - 1; x++)
	{
		std::vector<bool> col;
		for (int y = 0; y < height - 1; y++)
		{
			if (getHeight(x, y, imgData, width) > waterLevel) {
				Vec3f a(x, (getHeight(x, y, imgData, width) / 10) - 20, y);
				Vec3f b(x + 1, (getHeight(x + 1, y, imgData, width) / 10) - 20, y);
				Vec3f c(x + 1, (getHeight(x + 1, y + 1, imgData, width) / 10) - 20, y + 1);

				Vec3f normal = (c - a).cross(b - a).normalize();

				terreinVertices.push_back(Vert{ a, normal, Vec2f(x / textureScale,y / textureScale) });
				terreinVertices.push_back(Vert{ b, normal, Vec2f(x / textureScale,(y + 1) / textureScale) });
				terreinVertices.push_back(Vert{ c, normal, Vec2f((x + 1) / textureScale,(y + 1) / textureScale) });

				a = Vec3f(x, (getHeight(x, y, imgData, width) / 10) - 20, y);
				b = Vec3f(x, (getHeight(x, y + 1, imgData, width) / 10) - 20, y + 1);
				c = Vec3f(x + 1, (getHeight(x + 1, y + 1, imgData, width) / 10) - 20, y + 1);

				normal = Vec3f(b - a).cross(c - a).normalize();

				terreinVertices.push_back(Vert{ a, normal, Vec2f(x / textureScale,y / textureScale) });
				terreinVertices.push_back(Vert{ b, normal, Vec2f((x + 1) / textureScale,y / textureScale) });
				terreinVertices.push_back(Vert{ c, normal, Vec2f((x + 1) / textureScale,(y + 1) / textureScale) });

				if (getHeight(x, y, imgData, width) < waterLevel + 15.0f && getHeight(x, y, imgData, width) > waterLevel + 8.0f) {
					float v1 = rand() % 80;
					if (v1 == 0) {
						GameObject* grass = new GameObject(grasm);
						grass->position = Vec3f(x, (getHeight(x, y, imgData, width) / 10) - 20, y);
						grass->scale = Vec3f(.5f, .5f, .5f);
					}
				}
			}

			if (getHeight(x, y, imgData, width) > waterLevel + 8.0f) {
				col.push_back(true);
			}
			else {
				col.push_back(false);
			}

			Vec3f a(x, waterLevel - 20, y);
			Vec3f b(x + 1, waterLevel - 20, y);
			Vec3f c(x + 1, waterLevel - 20, y + 1);

			Vec3f normal = (c - a).cross(b - a).normalize();

			waterVertices.push_back(Vert{ a, normal, Vec2f(x / textureScale,y / textureScale) });
			waterVertices.push_back(Vert{ b, normal, Vec2f(x / textureScale,(y + 1) / textureScale) });
			waterVertices.push_back(Vert{ c, normal, Vec2f((x + 1) / textureScale,(y + 1) / textureScale) });

			a = Vec3f(x, waterLevel - 20, y);
			b = Vec3f(x, waterLevel - 20, y + 1);
			c = Vec3f(x + 1, waterLevel - 20, y + 1);

			normal = Vec3f(b - a).cross(c - a).normalize();

			waterVertices.push_back(Vert{ a, normal, Vec2f(x / textureScale,y / textureScale) });
			waterVertices.push_back(Vert{ b, normal, Vec2f((x + 1) / textureScale,y / textureScale) });
			waterVertices.push_back(Vert{ c, normal, Vec2f((x + 1) / textureScale,(y + 1) / textureScale) });

		}
		area.push_back(col);
	}

	delete imgData;
}

void TerreinGenerator::drawTerrein()
{

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	terreinTexture->Bind();
	glVertexPointer(3, GL_FLOAT, sizeof(Vert), ((float*)terreinVertices.data()) + 0);
	glNormalPointer(GL_FLOAT, sizeof(Vert), ((float*)terreinVertices.data()) + 3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vert), ((float*)terreinVertices.data()) + 6);
	glDrawArrays(GL_TRIANGLES, 0, terreinVertices.size());

	waterTexture->Bind();
	glVertexPointer(3, GL_FLOAT, sizeof(Vert), ((float*)waterVertices.data()) + 0);
	glNormalPointer(GL_FLOAT, sizeof(Vert), ((float*)waterVertices.data()) + 3);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vert), ((float*)waterVertices.data()) + 6);
	glDrawArrays(GL_TRIANGLES, 0, waterVertices.size());

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

bool TerreinGenerator::isFree(Vec2f xz)
{
	return area.at(xz.x).at(xz.y);
}
