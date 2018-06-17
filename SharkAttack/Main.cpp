#include <GL/freeglut.h>
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <stdio.h>

#define SPEED 5

#include "stb_image.h"

#include "Vec.h"
#include "ObjModel.h"
#include "GameObject.h"
#include "Player.h"
#include "TerreinGenerator.h"
#include "Paddle.h"
#include "Shark.h"

int width, height;
float lastFrameTime = 0;

int game_danger = 0;
bool game_eaten = false;
bool game_stuck = false;

TerreinGenerator* tg;

Vec3f cameraOffset;

std::vector<GameObject*> objects;
Player* player;
Shark* shark;

bool keys[255];

bool toggleleft = true, toggleright =true;


void drawText(std::string s, const Vec2f& pos) {
	glPushMatrix();
	glTranslatef(pos.x, pos.y, 0);
	glScalef(.3f, -.3f, 1);
	unsigned char label[30] = " ";
	std::copy(s.begin(), s.end(), label);
	glutStrokeString(GLUT_STROKE_ROMAN, label);
	glPopMatrix();
}

void drawHUD()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, -1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_CULL_FACE);

	glClear(GL_DEPTH_BUFFER_BIT);

	glEnable(GL_COLOR);
	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_TEXTURE_2D);

	if (game_eaten) {
		drawText("The shark tore you to shreds.", Vec2f(width / 3 - 60, height / 2));
		drawText("Press R to retart lol.", Vec2f(width / 3 + 20, height / 2 + 40));
		glColor3f(.0f, .0f, .0f);
		glBegin(GL_QUADS);
		glVertex3f(0, 0, 0);
		glVertex3f(width, 0, 0);
		glVertex3f(width, height, 0);
		glVertex3f(0, height, 0);
		glEnd();
		glColor3f(.8f, .8f, .8f);
	}

	glColor3f(.8f, .8f, .8f);
	if (game_danger == 0)
		drawText("Shark danger: None", Vec2f(30, 40));
	if (game_danger == 1)
		drawText("Shark danger: Low", Vec2f(30, 40));
	if (game_danger == 2)
		drawText("Shark danger: High", Vec2f(30, 40));
	if (game_danger == 3)
		drawText("Shark danger: VERY HIGH!", Vec2f(30, 40));

	if (game_stuck && !game_eaten)
		drawText("Stranded... R to restart", Vec2f(width / 3, height / 2));

	drawText("Paddle left:  A", Vec2f(30, height - 120));
	drawText("Paddle right: D", Vec2f(30, height - 80));
	drawText("Look back:  L", Vec2f(30, height - 40));

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE_2D);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display()
{
	glClearColor(0.17f, 0.14f, 0.13f, 1);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, (float)width / height, 0.1, 20);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_FOG);

	float fogColor[3] = { 0.17f,0.14f,0.13f };
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 2.f);
	glFogf(GL_FOG_END, 20.f);
	glFogf(GL_FOG_DENSITY, 5.f);

	gluLookAt(player->position.x + cameraOffset.x, player->position.y + cameraOffset.y + 0.8f, player->position.z + cameraOffset.z,
		player->position.x, player->position.y + 0.5f, player->position.z,
		0, 1, 0);

	if (player != nullptr) {
		float pos[4] = {player->position.x, player->position.y, player->position.z, 0.3f};
		glLightfv(GL_LIGHT0, GL_SPOT_EXPONENT, pos);
	}

	tg->drawTerrein();

	for (GameObject* o : objects)
		o->draw();

	drawHUD();

	glutSwapBuffers();
}

void idle()
{
	float frameTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	float deltaTime = frameTime - lastFrameTime;
	lastFrameTime = frameTime;

	const float speed = SPEED;
	if (keys['a'] || keys['A']) {
		if (toggleleft) {
			player->paddleLeft(speed);
			toggleleft = false;
		}
	}
	else {
		toggleleft = true;
	}
	if (keys['d'] || keys['D']) {
		if (toggleright) {
			player->paddleRight(speed);
			toggleright = false;
		}
	}
	else {
		toggleright = true;
	}

	if (keys['r'] || keys['R']) {
		if (game_eaten || game_stuck) {
		
			player->position = Vec3f(61.0f, -18.5f, 21.0f);
			player->rotation = Vec3f(0, 180, 0);
			shark->position = Vec3f(61.0f, -20.0f, 0.0f);

			shark->state = SharkState::IDLE;
			game_danger = 0;
			game_stuck = false;
			game_eaten = false;
		}
	}

	float z = sin(player->rotation.y * (M_PI / 180) + M_PI / 2);
	float y = cos(player->rotation.y * (M_PI / 180) + M_PI / 2);
	cameraOffset.x = y;
	cameraOffset.z = z;

	if (keys['l'] || keys['L']) {
		cameraOffset.x = -y;
		cameraOffset.z = -z;
	}

	for (GameObject* o : objects)
		o->update(deltaTime);

	glutPostRedisplay();
}

bool justMoved = false;
void mousePassiveMotion(int x, int y)
{
}

void keyboard(unsigned char key, int, int)
{
	if (key == 27)
		exit(0);
	keys[key] = true;
}

void keyboardUp(unsigned char key, int, int)
{
	keys[key] = false;
}

void initGameObjects() {
	ObjModel* paddleLM = new ObjModel("res\\models\\paddle\\paddle.obj");
	Paddle* paddleL = new Paddle(paddleLM, Vec3f(0, 0, 0), 270);
	paddleL->scale = Vec3f(-2.0f, 2.0f, -2.0f);

	ObjModel* paddleRM = new ObjModel("res\\models\\paddle\\paddle.obj");
	Paddle* paddleR = new Paddle(paddleRM, Vec3f(0, 0, 0), 270);
	paddleR->scale = Vec3f(2.0f, 2.0f, 2.0f);

	ObjModel* playerm = new ObjModel("res\\models\\boat\\obj_boat.obj");
	player = new Player(playerm, paddleL, paddleR, tg);
	player->position = Vec3f(61.0f, -18.5f, 21.0f);
	player->scale = Vec3f(0.5f, 0.5f, -0.5f);
	player->rotation = Vec3f(0, 180, 0);

	ObjModel* sharkm = new ObjModel("res\\models\\shark\\shark.obj");
	shark = new Shark(sharkm, player);
	shark->position = Vec3f(61.0f, -20.0f, 0.0f);
	shark->scale = Vec3f(-1, 1, -1);
}


int main(int argc, char* argv[]) {

	width = 600;
	height = 600;

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(width, height);
	glutInit(&argc, argv);
	glutCreateWindow("Shark Attack!");

	glutFullScreen();

	memset(keys, 0, sizeof(keys));
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glutSetCursor(GLUT_CURSOR_NONE);
	glutIdleFunc(idle);
	glutDisplayFunc(display);
	glutReshapeFunc([](int w, int h) { width = w; height = h; glViewport(0, 0, w, h); });
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboardUp);
	glutPassiveMotionFunc(mousePassiveMotion);

	tg = new TerreinGenerator("res\\textures\\tx_sand_16x.png", "res\\textures\\tx_water_16x.png");
	tg->generateHeightMap("res\\map_line.png");

	initGameObjects();

	stbi_set_flip_vertically_on_load(true);

	glutWarpPointer(width / 2, height / 2);
	glutMainLoop();

	return 0;
}