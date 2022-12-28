#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Square.h"
#include "Piece.h"
#include "Checkerboard.h"
#include "Gameplay.h"
#include "Menu.h"
#include "Settings.h"
#include "Analysis.h"

// komentarz testowy

class App
{
public:
	App() 
	{
		mainWindow = Window(1200, 900);
	}

	void Run()
	{
		mainWindow.Initialize();

		unsigned int n = 0;
		unsigned int color = 0;

		while (!mainWindow.GetShouldClose())
		{
			if (n == 0)
			{
				Menu* menu = new Menu(&mainWindow, &n);
				menu->Run();
				delete menu;
			}
			if (n == 1)
			{
				Gameplay* gameplay = new Gameplay(&mainWindow, &n, color);
				gameplay->Run();
				delete gameplay;
			}
			if (n == 2)
			{
				Analysis* analysis = new Analysis(&mainWindow, &n, color);
				analysis->Run();
				delete analysis;
			}
			if (n == 3)
			{
				Settings* settings = new Settings(&mainWindow, &n, &color);
				settings->Run();
				delete settings;
			}
			if (n > 3)
			{
				break;
			}
		}
	}

	~App() {}

private:
	Window mainWindow;
};

int main()
{
	App* app = new App;
	app->Run();
	delete app;

	return 0;
}