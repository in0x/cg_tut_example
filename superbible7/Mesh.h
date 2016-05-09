#pragma once

#include "stdafx.h"
#include "assimp.h"

#include <vector>


struct Mesh
{
	GLuint handleToVertexBuffer;
	GLuint handleToNormalBuffer;
	GLuint handleToIndexBuffer;
	GLuint handleToVertexArray;

	unsigned indexCount;
	unsigned vertexCount;
	unsigned faceCount;
};

class AssimpScene
{
public:
	~AssimpScene();
	
	void load(const char* filePath);
	void draw();

private:
	std::vector<Mesh> m_meshes;
};
