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
#include "Collision.h"
#include "Explosion.h"

//You can attribute to Bonsaiheldin | http://bonsaiheld.org

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
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
float moveSpeed = 0.00015f;

// Shot variables
float shotSpeed = 0.02f;


// Background texture scrolling speed
const float scrollSpeed = 0.002f;

// Create gloabal explosion animation object
Explosion extern_Explosion;

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
	glfwSetKeyCallback(window.getWindowPtr() , key_callback);

	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	// Enable alpha channel transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load and compile shaders to a program
	Shader ShaderProgram = Shader("../deps/shaders/shadervert.vs", "../deps/shaders/shaderfrag.fs");

	// Load explosion graphics
	extern_Explosion.Init(ShaderProgram.GetProgramID());

	// Load texture/Game objects
	Texture2D texture_background1 = Texture2D("../deps/textures/backgroundSpace_01.1.png", PNG_RGB);
	SpriteMap Background = SpriteMap(texture_background1, 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, BACKGROUND);

	Player PlayerShip;
	PlayerShip.Init(moveSpeed);

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

		/*	   Drawing	    */

		ShaderProgram.Use();

		// Background position and drawing calculations - just identity matrix
		glm::mat4 model;
		GLint modelLoc = glGetUniformLocation(ShaderProgram.GetProgramID(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		Background.BackgroundScroll(scrollSpeed);
		Background.Draw();

		Collision::EnemyHit(&PlayerShip, &Enemies);
		Collision::PlayerHit(&PlayerShip, &Enemies);
		Collision::ShipsCollide(&PlayerShip, &Enemies);

		PlayerShip.Move(keys);
		PlayerShip.AddShots(keys);
		PlayerShip.Draw(ShaderProgram.GetProgramID());
		
		Enemies.Move();
		Enemies.Shoot(PlayerShip.GetPosition());
		Enemies.Draw(ShaderProgram.GetProgramID());

		extern_Explosion.Draw();

		// FPS Calculation/Limiting
		float fps = CalculateFPS();
		static int printFPS = 0;
		if (printFPS == 100) {
			Enemies.Add(EN_0, PlayerShip.GetPosition());
			Enemies.Add(EN_1, PlayerShip.GetPosition());
			std::cout << fps << std::endl;
			printFPS = 0;
		} else {
			printFPS++;
		}
		
		
		LimitFPS(FPS, startFrame);
		
		if (PlayerShip.GetLives() <= 0)
		{
			window.Close();
		}

		// Swap the screen buffers
		window.SwapBuffers();
	}

	Background.Delete();
	PlayerShip.Delete();
	Enemies.DeleteAll();
	extern_Explosion.DeleteAll();

	// Close GLFW
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	switch (action) {
		case GLFW_PRESS :
			keys[key] = true;
			break;
		case GLFW_RELEASE :
			keys[key] = false;
			break;
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
	} /// ^ else just report whatever specified by FPS at top

	FrameBuffer[frameCount % SAMPLES] = thisFrame;

	frameCount++;
	int count = 0; ///< Used for averaging correct number of frames when frameBuffer is not accurately filled
	if (frameCount < SAMPLES) {
		count = frameCount;
	} else {
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