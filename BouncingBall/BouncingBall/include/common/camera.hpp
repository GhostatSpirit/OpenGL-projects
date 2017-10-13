#ifndef CONTROLS_HPP
#define CONTROLS_HPP

void computeMatricesFromInputs(GLFWwindow* window, double deltaTime, double height);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

void camera_scroll_callback(GLFWwindow* window, double xpos, double ypos);

#endif