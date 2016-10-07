#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class OBJObject
{
public:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
glm::mat4 toWorld;
int pointSize = 1;
float yAngle;

public:
	OBJObject();
	OBJObject(const char* filepath);

	void parse(const char* filepath);
	void draw();
	void update();
	void spin(float deg);
	void translate(float x, float y, float z);
	void scale(float mult);
	void restore();
	void orbit(float deg);
	void resizePoint(float size);
	int getPointSize();
};

#endif