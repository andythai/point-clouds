#define _CRT_SECURE_NO_DEPRECATE
#include "OBJObject.h"
#include <iostream>

OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
	parse(filepath);
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	
	FILE* fp;     // file pointer
	float x, y, z;  // vertex coordinates
	float r, g, b;  // vertex color
	int c1 = 0, c2 = 0;    // characters read from file
	glm::vec3 parser_vec3; // holds float values from parsing
	
	fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files
	if (fp == NULL) { 
		std::cerr << "error loading file" << std::endl; 
		exit(-1); 
	}  // just in case the file can't be found or is corrupt

	
	
	// Repeat until end of file
	while (c1 != EOF || c2 != EOF) {
		c1 = fgetc(fp);
		c2 = fgetc(fp);

		// DEBUG 
		//std::cout << "c1: " << c1 << ", c2: " << c2 << std::endl;
		
		// Check if line is a vertex
		if ((c1 == 'v') && (c2 == ' '))
		{
			fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
			parser_vec3 = glm::vec3(x, y, z);
			vertices.push_back(parser_vec3);
			// DEBUG std::cout << "v " << x << " " << y << " " << z << std::endl;
		}

		// read normal data accordingly
		else if ((c1 == 'v') && (c2 == 'n'))
		{
			fscanf(fp, "%f %f %f", &x, &y, &z);
			parser_vec3 = glm::vec3(x, y, z);
			normals.push_back(parser_vec3);
			// DEBUG std::cout << "vn " << x << " " << y << " " << z << std::endl;
		}
		// DEBUG std::cout << x << " " << y << " " << z << std::endl;
	}
	// DEBUG std::cout << "Done!" << std::endl;
	

	fclose(fp);   // make sure you don't forget to close the file when done
	
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data
}

void OBJObject::draw() 
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

	glBegin(GL_POINTS);
	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}