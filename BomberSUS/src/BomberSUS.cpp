#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "raylib.h"

//Si no encuentra Raylib: Proyecto --> Propiedades --> General --> Poner carpeta de includes
//Si no encuentra Raylib: Proyecto --> Propiedades --> Vinculadores --> Poner carpeta de .lib

using namespace std;

//Control Vars
const float m_version = 0.2f;

//Game Vars 
string m_title = "none :'(";
int m_texturesNum = 0;
map<string, string> m_textures;
map<string, Texture2D> m_textures2D;

//Map
int m_bgWidth = 0, m_bgHeight = 0;
int m_fgWidth = 0, m_fgHeight = 0;
int m_objWidth = 0, m_objHeight = 0;
string** m_background;
string** m_foreground;
string** m_objects;

float m_marginX, m_marginZ;
float m_offsetX, m_offsetZ;

Vector3 m_drawPosition = { 0.0f, 0.0f, 0.0f };
float cubeSize[3] = { 1.0f, 1.0f, 1.0f }; //Width Height Length

//Player
Vector3 m_player1Pos = { 0, 1, 0 };
bool m_bombPlaced = false;

void ImportFile() {
	ifstream file("level.sus", ios::in);
	if (!file.is_open()) {
		cout << "\n\nERROR <1>: \"level.sus\" not found!\n" << endl;
		system("pause");
		exit(1);
	}
	else { cout << "file opened!" << endl; }
	string line;

	//Extension Control
	getline(file, line, ';');
	if (line != "SUS") {
		cout << "\n\nERROR <2>: Wrong extension detected!\n" << endl;
		system("pause");
		exit(2);
	}
	cout << "\nExtension: " << line << endl;
	//Version Control
	getline(file, line, ';');
	if (stof(line) < m_version) {
		cout << "\n\nERROR <3>: Old version detected!, upgrade to " << m_version << "\n" << endl;
		system("pause");
		exit(3);
	}
	cout << "Version: " << line << endl;
	getline(file, line); // End Line

	//Title Control
	getline(file, line, ';');
	if (line != "TITLE") {
		cout << "\n\nERROR <4>: Title missing!\n" << endl;
		system("pause");
		exit(4);
	}
	//Set Title
	getline(file, line, ';');
	m_title = line;
	cout << "Game Title: " << m_title << endl;
	getline(file, line); // End Line

	//Textures Control
	getline(file, line, ';');
	if (line != "TEXTURES") {
		cout << "\n\nERROR <5>: Textures missing!\n" << endl;
		system("pause");
		exit(5);
	}
	//Set Textures number
	getline(file, line, ';');
	m_texturesNum = stoi(line);
	cout << "\nTextures to save: " << m_texturesNum << endl;
	getline(file, line); // End Line

	//Save textures
	for (int i = 0; i < m_texturesNum; i++) {
		getline(file, line, ';');
		string texId = line;
		getline(file, line, ';');
		string texPath = line;
		m_textures[texId] = texPath;

		cout << "Texture " << texId << " -> " << texPath << endl;
		getline(file, line); // End Line
	}

	//Background Control
	getline(file, line, ';');
	if (line != "BACKGROUND") {
		cout << "\n\nERROR <6>: Background missing!\n" << endl;
		system("pause");
		exit(6);
	}
	//Set Backrgound size
	getline(file, line, ';');
	m_bgWidth = stoi(line);
	getline(file, line, ';');
	m_bgHeight = stoi(line);
	cout << "\nBackground size: " << m_bgWidth << "x" << m_bgHeight << endl;
	getline(file, line); // End Line
	//Set bg level
	m_background = new string * [m_bgHeight]; //El doble puntero crea strings de la altura del lvl
	for (int i = 0; i < m_bgHeight; i++) {
		m_background[i] = new string[m_bgWidth]; //El doble puntero crea en cada altura su longitud
	}
	for (int i = 0; i < m_bgHeight; i++) {
		for (int j = 0; j < m_bgWidth; j++) {
			getline(file, line, ';');
			m_background[i][j] = line;
			cout << m_background[i][j] << " ";
		}
		if (i != m_bgHeight - 1) { cout << endl; }
		getline(file, line); // End Line
	}

	//Foreground Control
	getline(file, line, ';');
	if (line != "FOREGROUND") {
		cout << "\n\nERROR <7>: Foreground missing!\n" << endl;
		system("pause");
		exit(7);
	}
	//Set Forergound size
	getline(file, line, ';');
	m_fgWidth = stoi(line);
	getline(file, line, ';');
	m_fgHeight = stoi(line);
	cout << "\nForeground size: " << m_fgWidth << "x" << m_fgHeight << endl;
	getline(file, line); // End Line
	//Set fg level
	m_foreground = new string * [m_fgHeight]; //El doble puntero crea strings de la altura del lvl
	for (int i = 0; i < m_fgHeight; i++) {
		m_foreground[i] = new string[m_fgWidth]; //El doble puntero crea en cada altura su longitud
	}
	for (int i = 0; i < m_fgHeight; i++) {
		for (int j = 0; j < m_fgWidth; j++) {
			getline(file, line, ';');
			m_foreground[i][j] = line;
			cout << m_foreground[i][j] << " ";
		}
		if (i != m_fgHeight - 1) { cout << endl; }
		getline(file, line); // End Line
	}

	//Objects Control
	getline(file, line, ';');
	if (line != "OBJECTS") {
		cout << "\n\nERROR <8>: Objects missing!\n" << endl;
		system("pause");
		exit(8);
	}
	//Set Objects size
	getline(file, line, ';');
	m_objWidth = stoi(line);
	getline(file, line, ';');
	m_objHeight = stoi(line);
	cout << "\nObjects size: " << m_objWidth << "x" << m_objHeight << endl;
	getline(file, line); // End Line

	//Set Objects level
	m_objects = new string * [m_objHeight]; //El doble puntero crea strings de la altura del lvl
	for (int i = 0; i < m_objHeight; i++) {
		m_objects[i] = new string[m_objWidth]; //El doble puntero crea en cada altura su longitud
	}
	for (int i = 0; i < m_objHeight; i++) {
		for (int j = 0; j < m_objWidth; j++) {
			getline(file, line, ';');
			m_objects[i][j] = line;
			cout << m_objects[i][j] << " ";
		}
		cout << endl;
		getline(file, line); // End Line
	}

	file.close();
}

void LoadTextures() {
	cout << "\nTextures Loaded:" << endl;
	map<string, string>::iterator it;
	for (it = m_textures.begin(); it != m_textures.end();it++) {
		cout << "\n" << it->first << ": " << it->second << endl;
		m_textures2D[it->first]= LoadTextureFromImage(LoadImage(it->second.c_str()));
	}
}

void DrawLevel() {
	//Draw Background
	for (int i = 0; i < m_bgHeight; i++) {
		for (int j = 0; j < m_bgWidth; j++) {
			m_drawPosition = { (float)j +(-m_offsetX+m_marginX) ,0.0f, (float)i + (-m_offsetZ + m_marginZ) };

			if (m_background[i][j] != "X") {
				DrawCubeTexture(m_textures2D[m_background[i][j]], m_drawPosition, cubeSize[0], cubeSize[1], cubeSize[2], WHITE);
				//DrawCubeWires(m_drawPosition, cubeSize[0], cubeSize[1], cubeSize[2], BLACK);
			}
		}
	}
	//Draw Foreground
	for (int i = 0; i < m_fgHeight; i++) {
		for (int j = 0; j < m_fgWidth; j++) {
			m_drawPosition = { (float)j + (-m_offsetX + m_marginX) ,1.0f, (float)i + (-m_offsetZ + m_marginZ) };

			if (m_foreground[i][j] != "0") {
				DrawCubeTexture(m_textures2D[m_foreground[i][j]], m_drawPosition, cubeSize[0], cubeSize[1], cubeSize[2], WHITE);
				//DrawCubeWires(m_drawPosition, cubeSize[0], cubeSize[1], cubeSize[2], BLACK);
			}
		}
	}
	//Draw Grid
	if (m_bgHeight > m_bgWidth) { DrawGrid(m_bgHeight + 1, 1.0f); }
	else { DrawGrid(m_bgWidth + 1, 1.0f); }
}
void DrawObjects() {
	for (int i = 0; i < m_objHeight; i++) {
		for (int j = 0; j < m_objWidth; j++) {
			m_drawPosition = { (float)j + (-m_offsetX + m_marginX) ,1.0f, (float)i + (-m_offsetZ + m_marginZ) };

			if (m_objects[i][j] == "1") {
				m_player1Pos = { (float)i ,1, (float)j };
				DrawSphere(m_drawPosition, cubeSize[0] / 2, WHITE);
			}
			if (m_objects[i][j] == "2") {
				DrawSphere(m_drawPosition, cubeSize[0] / 2, YELLOW);
			}
			else if (m_objects[i][j] == "3") {
				DrawSphere(m_drawPosition, cubeSize[0] / 2, RED);
			}
			else if (m_objects[i][j] == "4") {
				DrawSphere(m_drawPosition, cubeSize[0] / 2, BLUE);
			}

			if (m_objects[i][j] == "B") {
				if (!m_bombPlaced) { DrawSphere(m_drawPosition, cubeSize[0] / 3.0f, BLACK); }
				else { DrawCubeWires(m_drawPosition, cubeSize[0], cubeSize[1], cubeSize[2], BLACK); }
			}
		}
	}
}

void PlayerMovement() {
	//OPPOSITE AXIS
	//HEIGHT IS X AXIS and WIDTH IS Z AXIS

	//RIGHT
	if (IsKeyPressed(KEY_RIGHT) && m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z + 1] == "0") {
		if (!m_bombPlaced) { m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z] = "0"; }
		m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z + 1] = "1";
		m_bombPlaced = false;
	}
	//LEFT
	else if (IsKeyPressed(KEY_LEFT) && m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z - 1] == "0") {
		if (!m_bombPlaced) { m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z] = "0"; }
		m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z - 1] = "1";
		m_bombPlaced = false;
	}
	//UP
	else if (IsKeyPressed(KEY_UP) && m_objects[(int)m_player1Pos.x - 1][(int)m_player1Pos.z] == "0") {
		if (!m_bombPlaced) { m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z] = "0"; }
		m_objects[(int)m_player1Pos.x - 1][(int)m_player1Pos.z] = "1";
		m_bombPlaced = false;
	}
	//DOWN
	else if (IsKeyPressed(KEY_DOWN) && m_objects[(int)m_player1Pos.x + 1][(int)m_player1Pos.z] == "0") {
		if (!m_bombPlaced) { m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z] = "0"; }
		m_objects[(int)m_player1Pos.x + 1][(int)m_player1Pos.z] = "1";
		m_bombPlaced = false;
	}
}
void BombPlacement() {
	if (IsKeyPressed(KEY_RIGHT_CONTROL)) {
		m_objects[(int)m_player1Pos.x][(int)m_player1Pos.z] = "B";
		m_bombPlaced = true;
	}
}

int main(void) {
	ImportFile();

	// Initialization
	const int screenWidth = 1600;
	const int screenHeight = 900;

	//Map Offsets
	m_marginX = m_bgWidth % 2 == 0 ? .5 : 0;
	m_marginZ = m_bgHeight % 2 == 0 ? .5 : 0;
	m_offsetX = m_bgWidth / 2;
	m_offsetZ = m_bgHeight / 2;

	InitWindow(screenWidth, screenHeight, m_title.c_str());
	LoadTextures();

	//Define the camera to look into our 3d world
	Camera3D camera = { 0 };
	camera.position = { 0.0f, 10.0f, 2.0f };	//Camera position
	camera.target = { 0.0f, 0.0f, 0.0f };		//Camera looking at point
	camera.up = { 0.0f, 1.0f, 0.0f };			//Camera up vector (rotation towards target)
	camera.fovy = 45.0f;						//Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;     //Camera mode type

	SetTargetFPS(60);               //Set our game to run at 60 frames-per-second

	//Main game loop
	while (!WindowShouldClose())    //Detect window close button or ESC key
	{
		//Update

		//Variables Update
		PlayerMovement();
		BombPlacement();

		//Draw Things
		BeginDrawing();

		ClearBackground(RAYWHITE);
		BeginMode3D(camera);

		DrawLevel();
		DrawObjects();

		EndMode3D();
		DrawText("Bomb them!!", 10, 40, 20, DARKGRAY);
		DrawFPS(10, 10);

		EndDrawing();
	}

	// De-Initialization
	CloseWindow();
	return 0;
}