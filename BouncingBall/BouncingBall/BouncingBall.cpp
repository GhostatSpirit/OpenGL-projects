// BouncingBall.cpp : Defines the entry point for the console application.

#include "sb7.h"

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>

#include "common/objloader.hpp"
#include "common/shader.hpp"
#include "common/camera.hpp"

#include "bounds.h"

using namespace std;

using std::runtime_error;
using std::string;
using std::vector;

struct StringHelper {
	const char *p;
	StringHelper(const std::string& s) : p(s.c_str()) {}
	operator const char**() { return &p; }
};



class BouncingBallApp : public sb7::application
{
private:
	const double INIT_Y = 10.0;
	const double GROUND_Y = 0.0;
	const double GRAVITY = -20;

	const float PLANE_SIDE_LENGTH = 10;

	double elasticity = 0.9;
	double veloDecreaseFactor = 0.9;

	double lastTime = 0;
	double deltaTime = 0;

	double height = INIT_Y;
	double velocity = 0;

	double mass = 1.0;

	bool deforming = false;
	double deformFactor = 0.3;
	double velocityBeforeDeform = 0.0;

	double restoreFactor = 0.995;

	Bounds objBounds;

    glm::vec4 getCurrentPos(double deltaTime) {

		if (!deforming) {
			velocity += GRAVITY * deltaTime;
			height += velocity * deltaTime + GRAVITY * deltaTime * deltaTime * 0.5;
		}
		else {
			double x = GROUND_Y - (height - objBounds.getEntents()[1]);
			double k = 500.0 / elasticity;
			double acceleration = k / mass * x;
			velocity += acceleration * deltaTime;

			//if (velocity > 0) {
			//	velocity *= restoreFactor;
			//}
			//else {
			//	velocity /= restoreFactor;
			//}

			height += velocity * deltaTime;
			if (height - objBounds.getEntents()[1] >= GROUND_Y) {
				deforming = false;

				velocity *= veloDecreaseFactor;
				/*velocity *= elasticity;*/
			}

			//// we cannot let the object go below the ground
			//if (height < GROUND_Y)  height = GROUND_Y;
		}


		// height collision check
		if (height - objBounds.getEntents()[1] <= GROUND_Y && !deforming) {
			// hit ground
			//height = GROUND_Y + objBounds.getEntents()[1];
			//velocity = -velocity * elasticity;
			deforming = true;
			height = GROUND_Y + objBounds.getEntents()[1];
			velocityBeforeDeform = velocity;
			
		}

		//if (velocity < 0) {
		//	cout << "height: " << height << ", velocity: " << velocity << endl;
		//}
		return vec4(0.0f, height, 0.0f, 1.0f);
	}

	double getCurrentYScale() {
		// must be called after getCurrentPos
		if (!deforming) {
			return 1.0;
		}
		else {
			return 1.0 - (objBounds.getEntents()[1] - height + GROUND_Y) / objBounds.getSize()[1];
		}
	}

	Bounds getBounds(vector<glm::vec3>& vert) {
		float xmax = 0, xmin = 0;
		float ymax = 0, ymin = 0;
		float zmax = 0, zmin = 0;

		for (auto iter = vert.begin(); iter != vert.end(); ++iter) {
			if ((*iter)[0] > xmax) xmax = (*iter)[0];
			if ((*iter)[0] < xmin) xmin = (*iter)[0];
			if ((*iter)[1] > ymax) ymax = (*iter)[1];
			if ((*iter)[1] < ymin) ymin = (*iter)[1];
			if ((*iter)[2] > zmax) zmax = (*iter)[2];
			if ((*iter)[2] < zmin) zmin = (*iter)[2];
		}

		return Bounds(vec3(xmin, ymin, zmin), vec3(xmax, ymax, zmax));
	}

	double thrust = 40.0;

	void processInput(double deltaTime) {
		int mouseLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		//if (lastSpaceInput == GLFW_RELEASE && spaceInput == GLFW_PRESS) {
		//	// user press down the space key
		//	velocity += thrust;
		//}
		if (mouseLeft == GLFW_PRESS) {
			velocity += thrust * deltaTime;
		}

		int mouseRight = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
		if (mouseRight == GLFW_PRESS && height - objBounds.getEntents()[1] >= GROUND_Y) {
			velocity -= thrust * deltaTime;
		}

	}

public:

	void startup() {
		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		// Accept fragment if it closer to the camera than the former one
		glDepthFunc(GL_LESS);

		// Cull triangles which normal is not towards the camera
		glEnable(GL_CULL_FACE);

		glGenVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// Create and compile our GLSL program from the shaders
		programID = LoadShaders("SimpleTransform.vert", "SingleColor.frag");

		// Get a handle for our "MVP" uniform
		matrixID = glGetUniformLocation(programID, "MVP");

		// Read our .obj file
		cout << "> Please input .obj file name: $ ";
		string filename;
		cin >> noskipws >> filename;

		// try to open file with the given file name
		std::ifstream file(filename);
		if (loadOBJ(filename, vertices, uvs, normals) == false) {
			cout << "> Falling back to \"sphere.obj\"... " << endl;
			loadOBJ("sphere.obj", vertices, uvs, normals);
		}

		//bool res = loadOBJ("mario.obj", vertices, uvs, normals);

		// add the plane to vector
		float half = PLANE_SIDE_LENGTH / 2.0;

		// load the vertices into a VBO
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &(vertices[0]), GL_STATIC_DRAW);

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		cout << "> Object read..." << endl;
		

		objBounds = getBounds(vertices);
		cout << "> Bounds: max(" << (objBounds.getMax())[0] << ", " << (objBounds.getMax())[1] << ", " << (objBounds.getMax())[2] << "), ";
		cout << "min(" << (objBounds.getMin())[0] << ", " << (objBounds.getMin())[1] << ", " << (objBounds.getMin())[2] << ")." << endl;

		glfwSetScrollCallback(window, camera_scroll_callback);
	}

	virtual void render(double currentTime)
	{
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		deltaTime = currentTime - lastTime;

		processInput(deltaTime);


		computeMatricesFromInputs(window, deltaTime, height);
		glm::mat4 Projection = getProjectionMatrix();
		glm::mat4 View = getViewMatrix();

		//// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		//glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
		//// Camera matrix
		//glm::mat4 View = glm::lookAt(
		//	glm::vec3(4, 4, 20), // Camera is at (4,3,-3), in World Space
		//	glm::vec3(0, 0, 0), // and looks at the origin
		//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		//);
		// Model matrix : an identity matrix (model will be at the origin)
		glm::mat4 Model = glm::mat4(glm::vec4(1, 0, 0, 0), 
			                        glm::vec4(0, 1, 0, 0), 
			                        glm::vec4(0, 0, 1, 0), 
			                        glm::vec4(getCurrentPos(deltaTime))
		);
		Model[1][1] = (float)getCurrentYScale();
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = Projection * View * Model;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // attribute
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		glDisableVertexAttribArray(0);

		lastTime = currentTime;
	}

	void shutdown() {
		glDeleteVertexArrays(1, &vertexArrayID);
		glDeleteProgram(programID);
	}

private:
	GLuint programID;
	GLuint vertexArrayID;

	GLuint matrixID;
	GLuint vertexBuffer;

	// .obj file content
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals; // Won't be used at the moment.
};


DECLARE_MAIN(BouncingBallApp)