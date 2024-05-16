#include"Camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
	Camera::width = width;
	Camera::height = height;
	Position = position;
}

void Camera::updateMatrix(float nearPlane, float farPlane) {
	//Orthographic view model
	glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, nearPlane, farPlane);
	glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
	glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, objectPos);

	//Multiply matrices together to form output matrix
	glm::mat4 mvp = proj * view * model;
	
	// Sets new camera matrix
	cameraMatrix = mvp;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
	// Exports camera matrix
	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window) {
	// Handles key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(Up);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += speed * -glm::normalize(Up);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		Position += speed * glm::normalize(glm::cross(Orientation, Up));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		speed = 3.0f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		speed = 10.0f;
	}
}