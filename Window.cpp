#include "Window.h"
#include "OBJObject.h"

const char* window_title = "GLFW Starter Project";

// Load models in at startup
OBJObject bunny;
OBJObject bear;
OBJObject dragon; 

// Variables indicating which model to load
bool showBunny = false;
bool showBear = false;
bool showDragon = false;

// Settings, keep values positive
const int POINT_SIZE_MODIFIER = 1;		// How much to increase point size
const float X_POS_MODIFIER = 1.0f;			// How much to move right on x-axis
const float Y_POS_MODIFIER = 1.0f;			// How much to move up on y-axis
const float Z_POS_MODIFIER = 1.0f;			// How much to move away from screen on z-axis
const float SCALE_UP_MODIFIER = 1.1f;		// Factor for how much to scale object up by
const float SCALE_DOWN_MODIFIER = 0.9f;		// Factor for how much to scale object down by
const float ORBIT_MODIFIER = 20.0f;			// How much to orbit the object by

// Window size variables declaration
int Window::width;
int Window::height;

// Display mode
bool DISPLAY_MODE = true;	// True: OpenGL, False: Rasterizer



/* RASTERIZER */

static int window_width = 640, window_height = 480;
static float* pixels = new float[window_width * window_height * 3];

//float z_buffer[640][480] = { FLT_MAX };

static float* z_buffer = new float[window_width * window_height];

using namespace std;


struct Color    // generic color class
{
	float r, g, b;  // red, green, blue
};

// Clear frame buffer
void clearBuffer()
{
	Color clearColor = { 0.0, 0.0, 0.0 };   // clear color: black
	/*
	for (int i = 0; i < 640; i++) {
		for (int j = 0; j < 480; j++) {
			z_buffer[i][j] = FLT_MAX;
		}
	}
	*/
	for (int i = 0; i<window_width*window_height; ++i)
	{
		z_buffer[i] = FLT_MAX;
		pixels[i * 3] = clearColor.r;
		pixels[i * 3 + 1] = clearColor.g;
		pixels[i * 3 + 2] = clearColor.b;
	}
}

// Draw a point into the frame buffer
void drawPoint(int x, int y, float r, float g, float b, int z)
{
	if (z < z_buffer[y*window_width + x]) {
	//if (z < z_buffer[x][y]) {
		z_buffer[y*window_width + x] = z;
		//z_buffer[x][y] = z;
		int offset = y*window_width * 3 + x * 3;
		pixels[offset] = r;
		pixels[offset + 1] = g;
		pixels[offset + 2] = b;
	}
}


void rasterize()
{
	// Put your main rasterization loop here
	// It should go over the point model and call drawPoint for every point in it
	OBJObject object;
	if (showBunny) {
		object = bunny;
	}
	else if (showDragon) {
		object = dragon;
	}
	else if (showBear) {
		object = bear;
	}
	else if (!showBunny && !showDragon && !showBear) {
		return;
	}

	glm::mat4 M_matrix = object.toWorld;

	glm::mat4 C_inverse = glm::lookAt(glm::vec3(0, 0, 20), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 projection_space;
	if (window_height != 0) {
		projection_space = glm::perspective(glm::radians(60.0f), (float)window_width / (float)window_height, 1.0f, 1000.0f);
	}
	else {
		return;
	}
	glm::mat4 D_matrix = { (float)window_width / 2, 0, 0, 0,
		0, (float)window_height / 2, 0, 0,
		0, 0, 0.5f, 0,
		(float)window_width / 2, (float)window_height / 2, 0.5f, 1.0f };

	for (int i = 0; i < object.vertices.size(); i++) {
		glm::vec4 point = { object.vertices[i].x, object.vertices[i].y, object.vertices[i].z, 1 };
		glm::vec4 p_prime = D_matrix * projection_space * C_inverse * M_matrix * point;
		glm::vec3 pixel = { p_prime[0] / p_prime[3], p_prime[1] / p_prime[3], p_prime[2] / p_prime[3] };

		float pixel_r = (glm::normalize(object.normals[i].x) + 1.0f) / 2.0f;
		float pixel_g = (glm::normalize(object.normals[i].y) + 1.0f) / 2.0f;
		float pixel_b = (glm::normalize(object.normals[i].z) + 1.0f) / 2.0f;
		
		std::vector<glm::vec2> includer;
		int z_pixel = (M_matrix * point)[2];

		int distance_modifier = z_pixel / 5;

		int square_size = object.getPointSize() + distance_modifier;
		
		if (square_size < 1) {
			square_size = 1;
		}

		for (int j = 0; j < square_size; j++) {
			for (int k = 0; k < square_size; k++) {
				glm::vec2 square_pixels = { pixel[0] + j, pixel[1] + k };
				includer.push_back(square_pixels);
			}
		}

		for (int m = 0; m < includer.size(); m++) {
			glm::vec2 pixel_pop = includer[m];
			int x_pixel = pixel_pop[0];
			int y_pixel = pixel_pop[1];
			
			if (x_pixel < window_width - 1 && x_pixel > 0 && y_pixel < window_height && y_pixel > 0 && z_pixel < 20) {
				drawPoint(x_pixel, y_pixel, pixel_r, pixel_g, pixel_b, p_prime[2]);
			}
		}
	}
}

// Called whenever the window size changes
void Window::resize_rasterizer(GLFWwindow* window, int width, int height)
{
	window_width = width;
	window_height = height;
	delete[] pixels;
	delete[] z_buffer;
	pixels = new float[window_width * window_height * 3];
	z_buffer = new float[window_width * window_height];
}

void Window::display_rasterizer(GLFWwindow* window)
{
	clearBuffer();
	rasterize();

	// glDrawPixels writes a block of pixels to the framebuffer
	glDrawPixels(window_width, window_height, GL_RGB, GL_FLOAT, pixels);

	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}


/* END RASTERIZER */


void Window::initialize_objects()
{
	bunny = OBJObject("bunny.obj");
	bear = OBJObject("bear.obj");
	dragon = OBJObject("dragon.obj");
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
	if (DISPLAY_MODE == true) {
		Window::resize_callback(window, width, height);
	}
	else {
		Window::resize_rasterizer(window, width, height);
	}

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
	if (DISPLAY_MODE == false) {
		resize_rasterizer(window, width, height);
	}
	else {
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
}

void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the object slightly.
	if (showBunny)
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
		if (showBunny)
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
			if (DISPLAY_MODE) {
				DISPLAY_MODE = false;
				resize_rasterizer(window, Window::width, Window::height);
			}
			else {
				DISPLAY_MODE = true;
				resize_callback(window, window_width, window_height);
			}
		}

	}
}
