#pragma once

#include "BaseTexture.h"
#include "Vector3.h"
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_access.hpp"
#include "Ray.h"

//This is an abstract class
//It cannot be instantiated without being inherited by a concrete class
class Renderable
{
	protected:
		BaseTexture				*m_tex;
		Vector3					m_position; // Do some get/sets for this
		Vector3					m_rotation;
		Vector3					m_scale;

		glm::vec4				m_ambient;
		glm::vec4				m_specular;
		float					m_shininess;
		bool					m_destructible;

	public:

		//this is a pure virtual function
								Renderable() { m_tex = 0; }
		virtual void			Render() = 0;
		
		inline void				SetTexture( BaseTexture* tex )
		{
			m_tex = tex;
		}


		// Bounding box
		struct AABB
		{
			glm::vec3			center;
			float				sizeX;
			float				sizeY;
			float				sizeZ;
		};

		AABB m_boundingBox;

		// Pos/Rot stuff
		inline void				SetPosition(Vector3 pos)
		{
			m_position = pos;
		}

		inline Vector3			GetPosition()
		{
			return	m_position;
		}

		inline void				SetRotation(Vector3 rot)
		{
			m_rotation = rot;
		}

		inline Vector3			GetRotation()
		{
			return m_rotation;
		}

		inline void				SetScale(Vector3 scale)
		{
			m_scale = scale;
		}

		inline Vector3			GetScale()
		{
			return m_scale;
		}

		// Diffuse/ambient/specular/diffuse
		inline glm::vec4		GetAmbient()
		{
			return m_ambient;
		}

		inline void				SetAmbient(glm::vec4 &ambient)
		{
			m_ambient = ambient;
		}

		inline glm::vec4		GetSpecular()
		{
			return m_specular;
		}

		inline void				SetSpecular(glm::vec4 &specular)
		{
			m_specular = specular;
		}

		inline float			GetShininess()
		{
			return m_shininess;
		}

		inline void				SetShininess(float shininess)
		{
			m_shininess = shininess;
		}

		inline bool				GetDestructible()
		{
			return				m_destructible;
		}

		inline void				SetDestructible(bool destructible)
		{
			m_destructible = destructible;
		}

		virtual void			CalculateAABB() = 0;
		virtual RayHitResult	IntersectByRay(Ray& ray) = 0;
};