#include"Buffer.h"

void Buffer::createBuffer(GLfloat* vertices, GLuint* indices) {
	VAO.Bind();

	VBO VBO(vertices, maxVertexCount * sizeof(Vertex));
	EBO EBO(indices, sizeof(indices));

	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, position));
	VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texUV));
	VAO.LinkAttrib(VBO, 2, 1, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, texNum));

	// Unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}