#ifndef BUFFER_CLASS_H
#define BUFFER_CLASS_H

#include"VAO.h"
#include"VBO.h"
#include"EBO.h"

class Buffer {
	public:
		GLuint ID;
		const size_t maxQuadCount = 50;
		const size_t maxVertexCount = maxQuadCount * 4;
		const size_t maxIndicesCount = maxQuadCount * 6;

		VAO VAO;

		void createBuffer(GLfloat* vertices, GLuint* indices);
	private:

};
#endif
