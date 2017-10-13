// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "camera.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix(){
	return ProjectionMatrix;
}

const float cameraRadius = 20.0;

// Initial position : on +Z
glm::vec3 position = glm::vec3( 0, 0, -cameraRadius);
// Initial horizontal angle : toward -Z
float horizontalAngle = 0.0f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
//float mouseSpeed = 0.005f;

float yOffset = 5;

float mouseWheelFactor = 5;
float fovOffset = 0;


void computeMatricesFromInputs(GLFWwindow* window, double deltaTime, double height){

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	
	//// Right vector
	//glm::vec3 right = glm::vec3(
	//	sin(horizontalAngle - 3.14f/2.0f), 
	//	0,
	//	cos(horizontalAngle - 3.14f/2.0f)
	//);
	//
	//// Up vector
	//glm::vec3 up = glm::cross( right, direction );

	// rotate towards top
	if (glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
		verticalAngle += deltaTime * speed;
	}
	// rotate towards bottom
	if (glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
		verticalAngle -= deltaTime * speed;
	}
	// rotate towards right
	if (glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
		horizontalAngle +=  deltaTime * speed;
	}
	// rotate towards left
	if (glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
		horizontalAngle -= deltaTime * speed;
	}

	// position : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 position(
		cameraRadius * cos(verticalAngle) * sin(horizontalAngle),
		cameraRadius * sin(verticalAngle),
		cameraRadius * cos(verticalAngle) * cos(horizontalAngle)
	);

	float FoV = initialFoV - fovOffset;

	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::vec3 up(0, 1, 0);
	if (glm::cos(verticalAngle) < 0) up[1] = -1.0;


	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								vec3(0.0, yOffset, 0.0),			// look towards to init point
		                        up       // Head is up (set to 0,-1,0 to look upside-down)
						   );

}

void camera_scroll_callback(GLFWwindow* window, double xpos, double ypos)
{
	fovOffset += mouseWheelFactor * ypos;
}