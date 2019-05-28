#include <Windows.h>
#include "OGLMesh.h"
#include "../Triangle.h"
#include "../OBJFileReader.h"
#include "OGLTexture.h"
#include "GLEW/include/glew.h"

OGLMesh::OGLMesh()
{

}

OGLMesh::OGLMesh(LPCWSTR filename)
{
	LoadAndBuildMeshFromOBJFile(filename);
}

OGLMesh::~OGLMesh()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo_verts);
}

void OGLMesh::Render()
{
	unsigned int texHandle = dynamic_cast<OGLTexture*>(m_tex)->m_syshandle;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glDrawArrays(GL_TRIANGLES, 0, m_numtriangles*3);

	glBindVertexArray(0);
}

void OGLMesh::LoadAndBuildMeshFromOBJFile(LPCWSTR filename)
{
	//Triangle* mesh;

	m_numtriangles = importOBJMesh(filename, &mesh);

	// Bounding box stuff
	Vector3 max = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 min = Vector3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < m_numtriangles; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector3 currPos = mesh[i].m_vertices[j].m_position;

			if (currPos[0] < min[0])
				min[0] = currPos[0];
			if (currPos[0] > max[0])
				max[0] = currPos[0];

			if (currPos[1] < min[1])
				min[1] = currPos[1];
			if (currPos[1] > max[1])
				max[1] = currPos[1];

			if (currPos[2] < min[2])
				min[2] = currPos[2];
			if (currPos[2] > max[2])
				max[2] = currPos[2];
		}
	}

	m_boundingBox.center = glm::vec3(0.0f, 0.0f, 0.0f);
	m_boundingBox.sizeX = max[0] - min[0];
	m_boundingBox.sizeY = max[1] - min[1];
	m_boundingBox.sizeZ = max[2] - min[2];
	
	int offset = sizeof(Vector3);
	int stride = sizeof(Vertex);

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_numtriangles * sizeof(Vertex), &(mesh[0].m_vertices[0].m_position[0]), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NULL + offset));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(NULL + 2*offset));
	glEnableVertexAttribArray(2);
			
	glBindVertexArray(0);

	//delete[] mesh;
}

void OGLMesh::CalculateAABB()
{
	// Bounding box stuff
	Vector3 max = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 min = Vector3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < m_numtriangles; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Vector3 currPos = mesh[i].m_vertices[j].m_position;

			if (currPos[0] < min[0])
				min[0] = currPos[0];
			if (currPos[0] > max[0])
				max[0] = currPos[0];

			if (currPos[1] < min[1])
				min[1] = currPos[1];
			if (currPos[1] > max[1])
				max[1] = currPos[1];

			if (currPos[2] < min[2])
				min[2] = currPos[2];
			if (currPos[2] > max[2])
				max[2] = currPos[2];
		}
	}

	m_boundingBox.center = glm::vec3(m_position[0], m_position[1], m_position[2]); //glm::vec3(0.0f, 0.0f, 0.0f);
	m_boundingBox.sizeX = (max[0] - min[0]) * m_scale[0];
	m_boundingBox.sizeY = (max[1] - min[1]) * m_scale[1];
	m_boundingBox.sizeZ = (max[2] - min[2]) * m_scale[2];
}

RayHitResult OGLMesh::IntersectByRay(Ray& ray)
{
	RayHitResult result = Ray::s_defaultHitResult;
	result.hit = false;
	double t = FARFAR_AWAY;
	glm::vec3 intersection_point;

	for (float range = 1.0f; range < 500.0f; range++)
	{
		intersection_point = ray.GetRayStart() + ray.GetRay() * range;

		glm::vec3 max = glm::vec3(m_boundingBox.center.x + (m_boundingBox.sizeX / 2), m_boundingBox.center.y + (m_boundingBox.sizeY / 2), m_boundingBox.center.z + (m_boundingBox.sizeZ / 2));
		glm::vec3 min = glm::vec3(m_boundingBox.center.x - (m_boundingBox.sizeX / 2), m_boundingBox.center.y - (m_boundingBox.sizeY / 2), m_boundingBox.center.z - (m_boundingBox.sizeZ / 2));

		bool withinMax = intersection_point.x < max.x && intersection_point.y < max.y && intersection_point.z < max.z;
		bool withinMin = intersection_point.x > min.x && intersection_point.y > min.y && intersection_point.z > min.z;

		if (withinMax && withinMin)
		{
			result.hit = true;
			break;
		}
	}

	return result;
}