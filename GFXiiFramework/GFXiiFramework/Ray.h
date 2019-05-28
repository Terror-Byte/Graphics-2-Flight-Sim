#pragma once

#include "glm/glm/glm.hpp"
//#include "Renderable.h"

//class Renderable;
#define FARFAR_AWAY 1000000.0

struct RayHitResult
{
	//glm::vec3					normal;
	//glm::vec3					point;
	//double						t;
	//void*						data;
	//Renderable					*target;
	bool						hit;
};

class Ray
{
	private:
		glm::vec3				m_start; //ray origin
		glm::vec3				m_ray; //direction of ray (unit vector)

	public:
		static RayHitResult		s_defaultHitResult; //Constant for storing default ray result i.e. nothing
								Ray();
								~Ray();

		inline void				SetRay(glm::vec3 start, glm::vec3 ray)
		{
			m_start = start;
			m_ray = ray;
		}

		inline glm::vec3		GetRay()
		{
			return	m_ray;
		}

		inline glm::vec3		GetRayStart()
		{
			return m_start;
		}
};