#include "Window.h"
#include "OBJObject.h"

const char* window_title = "GLFW Starter Project";
Cube cube(5.0f);

// Load models in at startup
OBJObject bunny = OBJObject("bunny.obj");
OBJObject bear = OBJObject("bear.obj");
OBJObject dragon = OBJObject("dragon.obj"); 

// Variables indicating which model to load
bool showBunny = false;
bool showBear = false;
bool showDragon = false;

// Settings, keep values positive
const float POINT_SIZE_MODIFIER = 1.0f;		// How much to increase point size
const float X_POS_MODIFIER = 1.0f;			// How much to move right on x-axis
const float Y_POS_MODIFIER = 1.0f;			// How much to move up on y-axis
const float Z_POS_MODIFIER = 1.0f;			// How much to move away from screen on z-axis
const float SCALE_UP_MODIFIER = 1.1f;		// Factor for how much to scale object up by
const float SCALE_DOWN_MODIFIER = 0.9f;		// Factor for how much to scale object down by
const float ORBIT_MODIFIER = 5.0f;			// How much to orbit the object by

// Window size variables declaration
int Window::width;
int Window::height;

// Display mode
bool DISPLAY_MODE = true;	// True: OpenGL, False: Rasterizer

void Window::initialize_objects()
{
}

void Window::clean_up()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Window::getDisplayMode()
{
	return DISPLAY_MODE;
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
				glPointSize(bunny.getPointSize());
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
				glPointSize(dragon.getPointSize());
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
				glPointSize(bear.getPointSize());
			}
		}

		// P POINT SIZE
		else if (key == GLFW_KEY_P)
		{
			// P
			if (mods == GLFW_MOD_SHIFT)
			{
				if (showBunny)
				{
					bunny.resizePoint(POINT_SIZE_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.resizePoint(POINT_SIZE_MODIFIER);
				}
				else if (showBear)
				{
					bear.resizePoint(POINT_SIZE_MODIFIER);
				}
			}

			// p
			else 
			{
				if (showBunny)
				{
					bunny.resizePoint(-POINT_SIZE_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.resizePoint(-POINT_SIZE_MODIFIER);
				}
				else if (showBear)
				{
					bear.resizePoint(-POINT_SIZE_MODIFIER);
				}
			}
		}

		// X MOVE X
		else if (key == GLFW_KEY_X)
		{
			// X
			if (mods == GLFW_MOD_SHIFT)
			{
				if (showBunny)
				{
					bunny.translate(X_POS_MODIFIER, 0, 0);
				}
				else if (showDragon)
				{
					dragon.translate(X_POS_MODIFIER, 0, 0);
				}
				else if (showBear)
				{
					bear.translate(X_POS_MODIFIER, 0, 0);
				}

			}
			// x
			else
			{
				if (showBunny)
				{
					bunny.translate(-X_POS_MODIFIER, 0, 0);
				}
				else if (showDragon)
				{
					dragon.translate(-X_POS_MODIFIER, 0, 0);
				}
				else if (showBear)
				{
					bear.translate(-X_POS_MODIFIER, 0, 0);
				}
			}

		}

		// Y MOVE Y
		else if (key == GLFW_KEY_Y)
		{
			if (!showBunny && !showDragon && !showBear)
			{
				return;
			}

			// Y
			if (mods == GLFW_MOD_SHIFT)
			{
				
				if (showBunny) 
				{
						bunny.translate(0, Y_POS_MODIFIER, 0);
				}
				else if (showDragon)
				{
						dragon.translate(0, Y_POS_MODIFIER, 0);
				}
				else if (showBear)
				{
						bear.translate(0, Y_POS_MODIFIER, 0);
				}
				
			}

			// y
			else
			{
				
				if (showBunny)
				{
						bunny.translate(0, -Y_POS_MODIFIER, 0);
				}
				else if (showDragon)
				{
						dragon.translate(0, -Y_POS_MODIFIER, 0);
				}
				else if (showBear)
				{
						bear.translate(0, -Y_POS_MODIFIER, 0);
				}
			}

		}

		// Z MOVE Z
		else if (key == GLFW_KEY_Z)
		{
			// Z
			if (mods == GLFW_MOD_SHIFT)
			{
				if (showBunny)
				{
					bunny.translate(0, 0, Z_POS_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.translate(0, 0, Z_POS_MODIFIER);
				}
				else if (showBear)
				{
					bear.translate(0, 0, Z_POS_MODIFIER);
				}
			}

			// z
			else
			{
				if (showBunny)
				{
					bunny.translate(0, 0, -Z_POS_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.translate(0, 0, -Z_POS_MODIFIER);
				}
				else if (showBear)
				{
					bear.translate(0, 0, -Z_POS_MODIFIER);
				}
			}
		}

		// S SCALE MODEL
		else if (key == GLFW_KEY_S)
		{
			// S
			if (mods == GLFW_MOD_SHIFT)
			{
				if (showBunny)
				{
					bunny.scale(SCALE_UP_MODIFIER);
				}
				else if (showBear)
				{
					bear.scale(SCALE_UP_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.scale(SCALE_UP_MODIFIER);
				}
			}

			// s
			else
			{
				if (showBunny)
				{
					bunny.scale(SCALE_DOWN_MODIFIER);
				}
				else if (showBear)
				{
					bear.scale(SCALE_DOWN_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.scale(SCALE_DOWN_MODIFIER);
				}
			}

		}
		
		// O ORBIT MODEL
		else if (key == GLFW_KEY_O)
		{
			// O
			if (mods == GLFW_MOD_SHIFT)
			{
				if (showBunny)
				{
					bunny.orbit(-ORBIT_MODIFIER);
				}
				else if (showBear)
				{
					bear.orbit(-ORBIT_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.orbit(-ORBIT_MODIFIER);
				}
			}

			// o
			else
			{
				if (showBunny)
				{
					bunny.orbit(ORBIT_MODIFIER);
				}
				else if (showBear)
				{
					bear.orbit(ORBIT_MODIFIER);
				}
				else if (showDragon)
				{
					dragon.orbit(ORBIT_MODIFIER);
				}
			}

		}

		// R RESET
		else if (key == GLFW_KEY_R)
		{
			if (showBunny)
			{
				bunny.restore();
			}
			else if (showDragon)
			{
				dragon.restore();
			}
			else if (showBear)
			{
				bear.restore();
			}
		}

		// M RASTER
		else if (key == GLFW_KEY_M)
		{
			DISPLAY_MODE = false;
		}

	}
}
