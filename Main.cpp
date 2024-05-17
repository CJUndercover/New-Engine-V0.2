#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<string>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <array>

#include"Camera.h"
#include"Texture.h"
#include"Shader.h"
#include"Buffer.h"

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 800;
const size_t maxQuadCount = 50;
const size_t maxVertexCount = maxQuadCount * 4;
const size_t maxIndicesCount = maxQuadCount * 6;
const char* title = "Daddy Engine - ";
float scale = 100.0f;

//Variables for basic FPS counter
double prevTime = 0.0;
double crntTime = 0.0;
double timeDiff;
unsigned int counter = 0;

static Vertex* CreateQuad(Vertex* target, float x, float y, int textureID) {
	//Do not initialize as one like this, float size = 1.0f;

	target->position = { x, y, 0.0f };
	target->texUV = { 0.0f, 0.0f };
	target->texNum = textureID;
	target++;

	target->position = { x + scale, y, 0.0f };
	target->texUV = { 1.0f, 0.0f };
	target->texNum = textureID;
	target++;
	
	target->position = { x + scale, y + scale, 0.0f };
	target->texUV = { 1.0f, 1.0f };
	target->texNum = textureID;
	target++;

	target->position = { x, y + scale, 0.0f };
	target->texUV = { 0.0f, 1.0f };
	target->texNum = textureID;
	target++;

	return target;
}
static GLFWwindow* onInit() {
	//initialize OpenGL
	glfwInit();

	//Create window object
	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
	}

	//Introduce the Window
	glfwMakeContextCurrent(window);

	//Load OpenGL
	//Specify viewport of OpenGL in Window
	//same parameters as the window object
	gladLoadGL();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glViewport(0, 0, screenWidth, screenHeight);

	//Load Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	return window;
}
static void fpsCounter(GLFWwindow* window) {
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
}

//Original Vertices Array
/*float vertices[] = {
//         Texture       //      Color      // TexCoord  // TextureNum
	200.0f, 500.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,     0.0f,
	200.0f, 300.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,     0.0f,
	400.0f, 300.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,     0.0f,
	400.0f, 500.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,     0.0f,

	400.0f, 500.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,     2.0f,
	400.0f, 300.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,     2.0f,
	600.0f, 300.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,     2.0f,
	600.0f, 500.0f, 0.0f,  1.0f, 1.0f, 1.0f,  1.0f, 0.0f,     2.0f
};*/
//Original Indices Array
/*GLuint indices[] = {
	0, 1, 2, 0, 3, 2,
	4, 5, 6, 4, 7, 6,
	//8, 9, 10, 8, 11, 10
};*/

int main() {
	GLFWwindow* window = onInit();

	bool drawObj = true;
	bool showDemo = false;
	float quadPosition[2] = { 200.0f, 300.0f };
	int quadCount = 0;

	//Shader
	Shader shaderProgram("default.vert", "default.frag");

	//Indices handler for larger batches
	GLuint indices[maxIndicesCount]{};
	GLuint offset = 0;
	for (GLuint i = 0; i < maxIndicesCount; i += 6) {
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;
		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;
		offset += 4;
	}

	VAO VAO1;
	VAO1.Bind();

	VBO VBO1(nullptr, maxVertexCount * sizeof(Vertex));
	EBO EBO1(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	VAO1.LinkAttrib(VBO1, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
	VAO1.LinkAttrib(VBO1, 2, 1, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texNum));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Load Textures
	Texture ground("resources/planks.png", "ground", 0, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture wall("resources/brick.png", "wall", 1, GL_RGBA, GL_UNSIGNED_BYTE);
	Texture penguin("resources/penguin.png", "penguin", 2, GL_RGBA, GL_UNSIGNED_BYTE);
	wall.collision = true;

	//Create Camera Object
	Camera camera(screenWidth, screenHeight, glm::vec3(0.0f, 0.0f, 1.0f));

	shaderProgram.Activate();
	auto loc = glGetUniformLocation(shaderProgram.ID, "u_Textures");
	int samplers[3] = { 0, 1, 2 };
	glUniform1iv(loc, 3, samplers);

	//---------Driver--------//
	while (!glfwWindowShouldClose(window)) {
		fpsCounter(window);

		//Set Background Color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		//Clears Color Buffer for next Frame

		int tileMap[7][7] = {
			{0, 1, 1, 1, 1, 1, 1},
			{1, 0, 1, 1, 1, 1, 1},
			{1, 1, 0, 1, 1, 1, 1},
			{1, 1, 1, 0, 1, 1, 1},
			{1, 1, 1, 1, 0, 1, 1},
			{1, 1, 1, 1, 1, 0, 1},
			{1, 1, 1, 1, 1, 1, 0},
		};

		uint32_t indexCount = 0;
		std::array<Vertex, 200> vertices{};
		Vertex* buffer = vertices.data();
		int len = 7;
		for (int y = 0; y < len; y++) {
			for (int x = 0; x < len; x++) {
				buffer = CreateQuad(buffer, x * scale, y * scale, tileMap[y][x]);
				indexCount += 6;
				quadCount++;
			}
		}

		//Controlled Quad
		buffer = CreateQuad(buffer, screenWidth / 2 + camera.Position.x - scale, screenHeight / 2 + camera.Position.y - scale, 2);
		indexCount += 6; 

		//Set Dynamic Vertex Buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBO1.ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());

		glClear(GL_COLOR_BUFFER_BIT);

		//Create new ImGui Frame
		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		//Bind Textures for batch rendering
		shaderProgram.Activate();
		VAO1.Bind();
		glBindTextureUnit(0, ground.ID);
		glBindTextureUnit(1, wall.ID);
		glBindTextureUnit(2, penguin.ID);

		//ImGui Logic
		if (showDemo) {
			showDemo = true;
			ImGui::ShowDemoWindow(&showDemo);
		}
		else {
		    showDemo = false;
	    }

		//Different Sub-Objects
		ImGui::Begin("2D Engine Debug Menu");
		ImGui::DragFloat2("Quad Position", quadPosition, 0.1f);
		ImGui::DragFloat("Quad Scale", &scale, 10.0f, 0.0f, 200.0f);
		ImGui::Checkbox("Demo Window", &showDemo);
		ImGui::Text("Quad Count: %d", quadCount);
		ImGui::Text("Vertex Count: %d", quadCount * 4);
		ImGui::Text("Index Count: %d", quadCount * 6);
		ImGui::End();

		//Initiate Camera object
		camera.Inputs(window);
		camera.updateMatrix(-1.0f, 1.0f);
		camera.Matrix(shaderProgram, "camMatrix");

		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		camera.Matrix(shaderProgram, "camMatrix");

		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

		//Render ImGui Window
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		quadCount = 0;
		//Swap back buffer w/ front buffer
		glfwSwapBuffers(window);
		//GLFW to take care of events
		glfwPollEvents();
	}
	//Delete ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//Delete Buffer Objects
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//Destroy Window and Close OpenGL
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}