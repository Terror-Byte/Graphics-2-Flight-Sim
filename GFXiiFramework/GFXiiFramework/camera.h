//Created for Graphics I and II
//Author: Minsi Chen

#ifndef __CAMERA_H__
#define __CAMERA_H__

//#include "matrix4x4.h"
//#include <glm/glm.hpp>
//#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include "Vector3.h"
#include "quaternion.h"

class Camera
{
	public:
		enum ECameraType
		{
			CAM_ORTHOGRAPHIC,
			CAM_PERSPECTIVE
		};

	private:		
		glm::mat4x4						m_viewMatrix;				//View Matrix
		glm::mat4x4						m_projectionMatrix;			//Projection Matrix
		Vector3							m_position;					//Position
		Vector3							m_upVector;					//up vector
		Vector3							m_rightVector;				//right vector
		Vector3							m_viewVector;				//view vector
		Vector3							m_lookAt;					//look at position
		ECameraType						m_cameratype;				//Projection type
		float							m_fov;						//vertical field of view in degree

		Vector3							m_direction;				//direction of camera
		float							m_aspectRatio;				//aspect ratio of camera

		float							wheelDeltaOld;
		float							m_yaw;
		float							m_pitch;
		float							m_roll;

		float							m_xTrans;
		float							m_yTrans;
		float							m_zTrans;

	public:
	
										Camera();
		virtual							~Camera();

		void							InitCamera();
		void							SetCameraPosition(const Vector3* position);
		inline const Vector3*			GetCameraPosition() const
		{
			return &m_position;
		}

		void							SetLookAtPoint(const Vector3* lookAt);
		inline const Vector3*			GetCameraDirection() const
		{
			return &m_direction;
		}
		void							SetUpVector(const Vector3* up);
		inline const Vector3*			GetCameraUpVector() const
		{
			return &m_upVector;
		}
		inline ECameraType				GetCameraType()
		{
			return m_cameratype;
		}

		inline void						SetCameraFOV(float fov)
		{
			m_fov = fov;
		}

		inline float					GetCameraFOV()
		{
			return m_fov;
		}

		inline void						SetCameraAspectRatio(float ar)
		{
			m_aspectRatio = ar;
		}

		inline float					GetCameraAspectRatio()
		{
			return m_aspectRatio;
		}

		void							UpdateViewMatrix();

		inline const glm::mat4x4*			GetViewMatrix() const
		{
			return &m_viewMatrix;
		}

		inline const glm::mat4x4*			GetProjMatrix() const
		{
			return &m_projectionMatrix;
		}
	
		//TODO: Setup the projection matrix
		void							SetProjectionMatrix(float* projmat);
		void							UpdateProjectionMatrix();

		//TODO: Implement the following camera movement
		void							StrafeCamera(float amount);
		void							DollyCamera(float amount);
		void							PedCamera(float amount);
		void							RotateCamera(float yaw, float pitch, float roll);
		void							ZoomCamera(float amount);

		glm::mat4x4						SkyboxPos();
		glm::mat4x4						Rotate(float pitch, float yaw, float roll);
};

#endif
