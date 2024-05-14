#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<string>

#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include"VAO.h"
#include"EBO.h"

const unsigned int screenWidth = 1000;
const unsigned int screenHeight = 800;
const char* title = "Daddy Engine - ";

//Variables for basic FPS counter
double prevTime = 0.0;
double crntTime = 0.0;
double timeDiff;
unsigned int counter = 0;

float vertices[] = {
//         Texture       //   Color         //  TexCoord // TextureNum
	200.0f, 500.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,     0.0f,
	200.0f, 300.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,     0.0f,
	400.0f, 300.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,     0.0f,
	400.0f, 500.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,     0.0f,

	400.0f, 500.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,     0.5f,
	400.0f, 300.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,     0.5f,
	600.0f, 300.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,     0.5f,
	600.0f, 500.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,     0.5f,

	600.0f, 500.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,     1.0f,
	600.0f, 300.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,     1.0f,
	800.0f, 300.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,     1.0f,
	800.0f, 500.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,     1.0f,
};

GLuint indices[] = {
	0, 1, 2, 0, 3, 2,
	4, 5, 6, 4, 7, 6,
	8, 9, 10, 8, 11, 10
};

int main() {
	//initialize OpenGL
	glfwInit();

	//Create window object
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Introduce the Window
	glfwMakeContextCurrent(window);

	//Load OpenGL
	//Specify viewport of OpenGL in Window
	//same parameters as the window object
	gladLoadGL();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, screenWidth, screenHeight);
	
	//Shader
	Shader shaderProgram("default.vert", "default.frag");

	//Load Textures
	Texture ground("resources/planks.png", "ground", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture wall("resources/brick.png", "wall", 1, GL_RGBA, GL_UNSIGNED_BYTE);

	//VAO
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 9 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 1, GL_FLOAT, 9 * sizeof(float), (void*)(8 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Create Camera Object
	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.0f, 1.0f));

	shaderProgram.Activate();
	auto loc = glGetUniformLocation(shaderProgram.ID, "u_Textures");
	int samplers[2] = { 0, 1 };
	glUniform1iv(loc, 2, samplers);

	while (!glfwWindowShouldClose(window)) {
		//Set Background Color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clears Color Buffer for next Frame
		glClear(GL_COLOR_BUFFER_BIT);

		shaderProgram.Activate();
		VAO1.Bind();
		glBindTextureUnit(0, ground.ID);
		glBindTextureUnit(1, wall.ID);

		// Take care of the camera Matrix
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");

		// Draw the actual mesh

		//FPS Counter
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		std::string FPS;
		std::string ms;
		//Update FPS in window
		if (timeDiff >= 1.0 / 30.0) {
			FPS = std::to_string((1.0 / timeDiff) * counter);
			ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = title + FPS + " FPS / " + ms + " ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = crntTime;
			counter = 0;
		}

		//Initiate Camera object
		camera.Inputs(window);
		camera.updateMatrix(45.0f, -1.0f, 1.0f);
		camera.Matrix(shaderProgram, "camMatrix");

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		//Swap back buffer w/ front buffer
		glfwSwapBuffers(window);
		//GLFW to take care of events
		glfwPollEvents();
	}
	
	//Delete Buffer Objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//Destroy Window and Close OpenGL
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}