#pragma once

#include <Windows.h>
#include "Renderable.h"
#include "Triangle.h"

class OGLTerrain : public Renderable
{
private:
	unsigned int	m_vao;
	unsigned int	m_vbo_verts;
	int				m_numtriangles;

public:
	OGLTerrain();
	~OGLTerrain();

	void BuildMesh();

	void Render();

	void CalculateAABB() { ; };
	RayHitResult IntersectByRay(Ray& ray) { return Ray::s_defaultHitResult; };
};