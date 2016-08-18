/*




#include "Enemy.h"

#include <time.h>
#include <functional>

#include <iostream>
#include <GL/glew.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


glm::vec3 Enemy::GenStartPos()
{
std::uniform_real_distribution<double> dist(0.0, 1.0);
float x = dist(_rng)*2.0f - 1.0f;
float y = 1.2f;
float z = 0.0f;
return glm::vec3(x, y, z);
}

glm::vec3 Enemy::CalculateDirection(EnemyType type, const glm::vec3& enemy_pos, const glm::vec3& player_pos)
{
glm::vec3 direction;
std::uniform_real_distribution<double> dist(0.0, 1.0);

switch (type)
{
case EN_0:
direction = player_pos - enemy_pos;
if (direction.x > 0.0f)
{
direction.x += dist(_rng); //or just += 0.05f maybe
}
else
{
direction.x -= dist(_rng);
}
break;
default:
return glm::vec3(0.0f, 0.0f, 0.0f);
}

glm::normalize(direction);
return direction;
}

Enemy::Enemy()
{
// Empty - std::vector constructor initializes _enemyArray and _textureArray for us.
}


Enemy::~Enemy()
{
// Empty - Use manual Delete();
}

void Enemy::Init()
{
_rng = std::mt19937(time(0));
_weapon.Init(ENEMY);

/// Index 0 - EN_0
_textureArray.push_back(Texture2D("../deps/textures/Gepard.png", PNG_RGBA));
}

void Enemy::Add(EnemyType type, const glm::vec3& player_pos)
{
SpriteMap newEnemy;
switch (type) {
case EN_0:
newEnemy = SpriteMap(_textureArray[0], 0.22, 0.27, GenStartPos(), 0.002, SHIP_ENEMY);

newEnemy.SetDirection(CalculateDirection(EN_0, newEnemy.GetPosition(), player_pos));
break;
}

_enemyArray.push_back(newEnemy);
}

void Enemy::Move()
{
for (int i = 0; i < _enemyArray.size(); i++)
{
_enemyArray[i].UpdatePosition();
}
}

// Approximation amount specified in here: x_DIST
void Enemy::CheckForShot(const glm::vec3& player_pos)
{
float x_DIST = 0.4;

static int delay = 0;
for (int i = 0; i < _enemyArray.size(); i++)
{
if (abs(_enemyArray[i].GetPosition().x - player_pos.x) <= x_DIST && delay <= 0)
{
_weapon.Add(_enemyArray[i].GetPosition() + glm::vec3(0.0f, -0.12f, 0.0f), GREEN_PROJ);
delay = 100;
}
else
{
delay--;
}
}
}

void Enemy::Draw(GLuint shader_id)
{
GLuint modelLoc = glGetUniformLocation(shader_id, "model");
for (int i = 0; i < _enemyArray.size(); i++)
{
glm::mat4 modelEnemy;
modelEnemy = glm::translate(modelEnemy, _enemyArray[i].GetPosition());
glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelEnemy));

_enemyArray[i].Draw();
}

_weapon.Draw(shader_id);
}

// Remove from given index
void Enemy::Remove(int index)
{
// Finish this! will be needed for collision
}

void Enemy::DeleteAll()
{
for (int i = 0; i < _enemyArray.size(); i++)
{
_enemyArray[i].Delete();
}
for (int i = 0; i < _textureArray.size(); i++)
{
_textureArray[i].Delete();
}
_weapon.Delete();
}

glm::vec3 Enemy::GetPos(int index)
{
return _enemyArray[index].GetPosition();
}





--------------------------------------------------------------------------------------------------------------------------





#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <Windows.h>

#include "Shader.h"
#include "Window.h"
#include "Texture2D.h"
#include "SpriteMap.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Player.h"

//You can attribute to Bonsaiheldin | http://bonsaiheld.org

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void MovePlayer(glm::vec3 position);
void AddPlayerShots(SpriteMap player, Texture2D texture, std::vector<Weapon>* array); ///< will change later
float CalculateFPS();
void LimitFPS(float target, float startFrame);

// Window parameters
const GLuint WIDTH = 800, HEIGHT = 650;
const char* TITLE = "Space Game";
const double FPS = 120.0;

// Key callback array
bool keys[1024] = { false };

// Player movement variables
glm::vec3 input_direction;
float moveSpeed = 0.0001f;

// Shot variables
float shotSpeed = 0.02f;

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;	///< Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

// Background texture scrolling speed
const float scrollSpeed = 0.002f;

int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	Window window = Window(WIDTH, HEIGHT, TITLE);
	window.DefineViewport();

	//glfwSetInputMode(window.getWindowPtr(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);			
	/// ^(Maybe use this later)

	// Callback functions
	glfwSetKeyCallback(window.getWindowPtr(), key_callback);
	glfwSetCursorPosCallback(window.getWindowPtr(), mouse_callback);

	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Enable OpenGL rendering depth test
	//glEnable(GL_DEPTH_TEST);

	// Enable alpha channel transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load and compile shaders to a program
	Shader ShaderProgram = Shader("../deps/shaders/shadervert.vs", "../deps/shaders/shaderfrag.fs");


	// Load texture objects
	Texture2D texture_background1 = Texture2D("../deps/textures/backgroundSpace_01.1.png", PNG_RGB);
	Texture2D texture_player = Texture2D("../deps/textures/ship0.png", PNG_RGBA);
	Texture2D texture_shot_player = Texture2D("../deps/textures/spr_bullet_green.png", PNG_RGBA);


	// Load game objects
	SpriteMap Background = SpriteMap(texture_background1, 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, BACKGROUND);
	SpriteMap _Player = SpriteMap(texture_player, 0.22f, 0.22f, glm::vec3(0.0f, -0.75f, 0.0f), moveSpeed, SHIP_PLAYER);
	///^ _Player will be unused soon!

	std::vector<Weapon> Shot;

	//Player PlayerShip;
	//PlayerShip.Init(moveSpeed);

	Enemy Enemies;
	Enemies.Init();


	// Projection matrix: ortho for 2D
	glm::mat4 proj = glm::ortho(0, window.getWidth(), 0, window.getHeight());


	// Game loop
	while (!window.ShouldClose())
	{
		double startFrame = glfwGetTime();  ///< for FPS limiting

											// Check if any events have been activiated and call callback function (via GLFW)
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.6f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//	   Drawing	    //

		ShaderProgram.Use();

		// Background position and drawing calculations - just identity matrix
		glm::mat4 model;
		GLint modelLoc = glGetUniformLocation(ShaderProgram.GetProgramID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Background.BackgroundScroll(scrollSpeed);
		Background.Draw();


		// Shot stuff - Check for update then draw
		AddPlayerShots(_Player, texture_shot_player, &Shot);

		for (int i = 0; i < Shot.size(); i++)
		{
			Shot[i].Move();

			glm::mat4 shotModel;
			modelLoc = glGetUniformLocation(ShaderProgram.GetProgramID(), "model");
			shotModel = glm::translate(shotModel, Shot[i].GetPosition());
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(shotModel));

			Shot[i].Draw();
			if (Shot[i].GetLifetime() <= 0)
			{
				Shot.erase(Shot.begin() + i);
			}
		}

		// Player position and drawing calculations
		MovePlayer(_Player.GetDirection());
		_Player.UpdatePosition(input_direction);

		modelLoc = glGetUniformLocation(ShaderProgram.GetProgramID(), "model");
		model = glm::translate(model, _Player.GetPosition());
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		_Player.Draw();


		//Enemies.Add(EN_0, Player.GetPosition());
		Enemies.Move();
		Enemies.Draw(ShaderProgram.GetProgramID());


		// FPS Calculation/Limiting

		float fps = CalculateFPS();
		static int printFPS = 0;
		if (printFPS == 100) {
			Enemies.Add(EN_0, _Player.GetPosition());
			std::cout << startFrame << " " << glfwGetTime() - startFrame << std::endl;
			std::cout << fps << std::endl;
			printFPS = 0;
		}
		else {
			printFPS++;
		}


		LimitFPS(FPS, startFrame);

		// Swap the screen buffers
		window.SwapBuffers();
	}

	Background.Delete();
	_Player.Delete();
	Enemies.DeleteAll();

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	switch (action) {
	case GLFW_PRESS:
		keys[key] = true;
		break;
	case GLFW_RELEASE:
		keys[key] = false;
		break;
	}
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void MovePlayer(glm::vec3 curr_direction)
{
	input_direction = glm::vec3(0.0f, 0.0f, 0.0f);

	if (keys[GLFW_KEY_W]) {
		input_direction += glm::vec3(0.0, 1.0, 0.0);
	}
	else if (curr_direction.y > 0) {
		input_direction -= glm::vec3(0.0, 1.0, 0.0) / 2.0f;
	}

	if (keys[GLFW_KEY_S]) {
		input_direction -= glm::vec3(0.0, 1.0, 0.0);
	}
	else if (curr_direction.y < 0) {
		input_direction += glm::vec3(0.0, 1.0, 0.0) / 2.0f;
	}

	if (keys[GLFW_KEY_A]) {
		input_direction -= glm::vec3(1.0, 0.0, 0.0);
	}
	else if (curr_direction.x < 0) {
		input_direction += glm::vec3(1.0, 0.0, 0.0) / 2.0f;
	}

	if (keys[GLFW_KEY_D]) {
		input_direction += glm::vec3(1.0, 0.0, 0.0);
	}
	else if (curr_direction.x > 0) {
		input_direction -= glm::vec3(1.0, 0.0, 0.0) / 2.0f;
	}
}

void AddPlayerShots(SpriteMap player, Texture2D texture, std::vector<Weapon>* array)
{
	static bool fire;
	if ((keys[GLFW_KEY_SPACE] || keys[GLFW_KEY_UP]) && fire == true)
	{
		array->push_back(Weapon(player.GetPosition() + glm::vec3(0.0f, 0.12f, 0.0f), shotSpeed, 10000, texture, PLAYER));
		fire = false;

	}
	if (!keys[GLFW_KEY_SPACE] && !keys[GLFW_KEY_UP])
	{
		fire = true;
	}
}

float CalculateFPS()
{
	static float startFrame = 0;
	static int frameCount = 0;

	float endFrame = glfwGetTime();
	float thisFrame = FPS;

	const int SAMPLES = 100;
	static float FrameBuffer[SAMPLES];

	float ms_per_frame = endFrame - startFrame;
	if (ms_per_frame > 0) {
		thisFrame = 1.0f / ms_per_frame;
	} /// ^ else just report 60 fps

	FrameBuffer[frameCount % SAMPLES] = thisFrame;

	frameCount++;
	int count = 0; ///< Used for averaging correct number of frames when frameBuffer is not accurately filled
	if (frameCount < SAMPLES) {
		count = frameCount;
	}
	else {
		count = SAMPLES;
	}

	float fps_avg = 0.0f;
	for (int i = 0; i < count; i++) {
		fps_avg += FrameBuffer[i];
	}
	fps_avg /= count;

	startFrame = endFrame;
	return fps_avg;
}

void LimitFPS(float target, float startFrame)
{
	float frameTime = glfwGetTime() - startFrame;
	if (1.0 / target > frameTime)
	{
		Sleep((1.0 / target - frameTime) * 1000.0);
	}
}
*/