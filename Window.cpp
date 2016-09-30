#include "Window.h"
#include "OBJObject.h"

const char* window_title = "GLFW Starter Project";
Cube cube(5.0f);

// Load models in at startup
OBJObject bunny = OBJObject("bunny.obj");
OBJObject bear = OBJObject("bear.obj");
OBJObject dragon = OBJObject("dragon.obj");
bool showBunny = false;
bool showBear = false;
bool showDragon = false;
float pointSize = 1.0f;

int Window::width;
int Window::height;

void Window::initialize_objects()
{
}

void Window::clean_up()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	Window::width = width;
	Window::height = height;
	// Set the viewport size
	glViewport(0, 0, width, height);
	// Set the matrix mode to GL_PROJECTION to determine the proper camera properties
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// Set the perspective of the projection viewing frustum
	gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
	// Move camera back 20 units so that it looks at the origin (or else it's in the origin)
	glTranslatef(0, 0, -20);
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the object slightly.
	if (!showDragon && !showBear && !showBunny)
	{
		cube.update();
	}
	else if (showBunny)
	{
		bunny.update();
	}
	else if (showBear)
	{
		bear.update();
	}
	else {
		dragon.update();
	}
}

void Window::display_callback(GLFWwindow* window)
{
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Set the matrix mode to GL_MODELVIEW
	glMatrixMode(GL_MODELVIEW);
	// Load the identity matrix
	glLoadIdentity();
	
	// Render objects
	if (!showBunny && !showBear && !showDragon)
	{
		cube.draw();
	}
	else if (showBunny)
	{
		bunny.draw();
	}
	else if (showBear) {
		bear.draw();
	}
	else if (showDragon)
	{
		dragon.draw();
	}

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// F1 BUNNY
		else if (key == GLFW_KEY_F1)
		{
			// Show the bunny
			if (!showBunny)
			{
				clean_up();
				showBunny = true;
				showBear = false;
				showDragon = false;
			}
		}

		// F2 DRAGON
		else if (key == GLFW_KEY_F2)
		{
			// Show the dragon
			if (!showDragon)
			{
				clean_up();
				showBunny = false;
				showBear = false;
				showDragon = true;
			}
		}

		// F3 BEAR
		else if (key == GLFW_KEY_F3) 
		{
			// Show the bear
			if (!showBear)
			{
				clean_up();
				showBunny = false;
				showBear = true;
				showDragon = false;
			}
		}

		// P POINT SIZE
		else if (key == GLFW_KEY_P)
		{
			// P
			if (mods == GLFW_MOD_SHIFT)
			{
				pointSize = pointSize + 1;
			}

			// p
			else 
			{
				if (pointSize >= 1) {
					pointSize = pointSize - 1;
					if (pointSize < 1) 
					{
						pointSize = 1;
					}
				}
			}
			glPointSize(pointSize);
		}

		// X MOVE X
		else if (key == GLFW_KEY_X)
		{
			// X
			if (mods == GLFW_MOD_SHIFT)
			{
				
			}

			// x
			else
			{

			}

		}

		// Y MOVE Y
		else if (key == GLFW_KEY_Y)
		{
			// Y
			if (mods == GLFW_MOD_SHIFT)
			{

			}

			// y
			else
			{

			}

		}

		// Z MOVE Z
		else if (key == GLFW_KEY_Z)
		{
			// Z
			if (mods == GLFW_MOD_SHIFT)
			{

			}

			// z
			else
			{

			}

		}


		// S SCALE MODEL
		else if (key == GLFW_KEY_S)
		{
			// S
			if (mods == GLFW_MOD_SHIFT)
			{

			}

			// s
			else
			{

			}

		}

		// O ORBIT MODEL
		else if (key == GLFW_KEY_O)
		{
			// O
			if (mods == GLFW_MOD_SHIFT)
			{

			}

			// o
			else
			{

			}

		}

		// R RESET
		else if (key == GLFW_KEY_R)
		{
			if (showBunny)
			{
				bunny = OBJObject("bunny.obj");
			}
			else if (showDragon)
			{
				dragon = OBJObject("dragon.obj");
			}
			else if (showBear)
			{
				bear = OBJObject("bear.obj");
			}
			else
			{
				cube = Cube(5.0f);
			}

			pointSize = 1;
			glPointSize(pointSize);
		}

		// M RASTER
		else if (key == GLFW_KEY_M)
		{

		}

	}
}
