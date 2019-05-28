//Created for Graphics I and II
//Author: Minsi Chen

#include "camera.h"
#include <stdio.h> // Just for testing

Camera::Camera()
{
	InitCamera();
}

Camera::~Camera()
{
}

void Camera::InitCamera()
{
	//Initialise the camera that sits at the origin looking down the negative z-axis.
	m_position.SetVector(0.0f, 0.0f, 10.0f);
	m_upVector.SetVector(0.0f, 1.0f, 0.0f);
	m_direction.SetVector(0.0f, 0.0, -1.0f);
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);
	UpdateViewMatrix();

	m_yaw = 0;
	m_pitch = 0;
	m_roll = 0;
}

void Camera::SetCameraPosition(const Vector3* position)
{
	m_position = (*position);
	UpdateViewMatrix();
}

void Camera::SetLookAtPoint(const Vector3* lookAt)
{
	m_direction = (*lookAt) - m_position;
	m_direction.Normalise();
	
	//Recompute right vector and reinforce orthonormality of this three vectors.
	//Recompute up vector, from new direction and right vector;
	//TODO: Gram-Schmidt orthogonalisation algorithm can be used for this
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);

	m_rightVector.Normalise();
	m_upVector.Normalise();

	//========Gram-Schmidt stuff========
	// Gram-Schmidt code. Dis vidya: https://www.khanacademy.org/math/linear-algebra/alternate_bases/orthonormal_basis/v/linear-algebra-gram-schmidt-example-with-3-basis-vectors
	// Also wikipedia: http://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process
	/*Vector3 u1 = m_direction;
	Vector3 u2 = ( m_rightVector - (u1 * m_rightVector.DotProduct(u1)) ).Normalise();
	Vector3 u3 = ( m_upVector - (u1 * m_upVector.DotProduct(u1)) - (u2 * m_upVector.DotProduct(u2)) ).Normalise();

	m_direction = u1;
	m_rightVector = u2;
	m_upVector = u3;
	UpdateViewMatrix();*/
	//printf("\nDirection Vector: %f %f %f\n", m_direction[0], m_direction[1], m_direction[2]);
	//printf("Right Vector: %f %f %f\n", m_rightVector[0], m_rightVector[1], m_rightVector[2]);
	//printf("Up Vector: %f %f %f\n", m_upVector[0], m_upVector[1], m_upVector[2]);
	//===============END================

	UpdateViewMatrix();
}

void Camera::SetUpVector(const Vector3* up)
{
	m_upVector = (*up);
	
	m_rightVector = m_direction.CrossProduct(m_upVector);
	m_upVector = m_rightVector.CrossProduct(m_direction);

	m_rightVector.Normalise();
	m_upVector.Normalise();

	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	/*
	float mat[16];

	mat[0] = m_rightVector[0];
	mat[4] = m_rightVector[1];
	mat[8] = m_rightVector[2];
	
	mat[1] = m_upVector[0];
	mat[5] = m_upVector[1];
	mat[9] = m_upVector[2];

	mat[2] = -m_direction[0];
	mat[6] = -m_direction[1];
	mat[10] = -m_direction[2];

	mat[3] = mat[7] = mat[11] = 0.0f;

	mat[12] = -m_position[0];
	mat[13] = -m_position[1];
	mat[14] = -m_position[2];
	mat[15] = 1.0f;

	//m_viewMatrix.SetMatrix(&mat[0]);
	m_viewMatrix = glm::make_mat4x4(&mat[0]);
	*/
	
	glm::vec3* eye = new glm::vec3(m_position[0], m_position[1], m_position[2]);
	glm::vec3* center = new glm::vec3(m_position[0] + m_direction[0], m_position[1] + m_direction[1], m_position[2] + m_direction[2]);
	glm::vec3* up = new glm::vec3(m_upVector[0], m_upVector[1], m_upVector[2]);
	m_viewMatrix = glm::lookAt(*eye, *center, *up);

	//m_upVector = Vector3(m_viewMatrix[1].x, m_viewMatrix[1].y, m_viewMatrix[1].z);
	//m_rightVector = Vector3(m_viewMatrix[0].x, m_viewMatrix[0].y, m_viewMatrix[0].z);
	//m_direction = Vector3(m_viewMatrix[2].x, m_viewMatrix[2].y, m_viewMatrix[2].z);
	//m_position = Vector3(m_viewMatrix[3].x, m_viewMatrix[3].y, m_viewMatrix[3].z);
}

void Camera::UpdateProjectionMatrix()
{
	m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, 0.1f, 100000.0f);
}

void Camera::SetProjectionMatrix(float* projmat)
{
	m_projectionMatrix = glm::make_mat4x4(projmat);
}

void Camera::StrafeCamera(float amount)
{
	//m_view(direction)Vector
	
	m_position[0] += amount * m_rightVector[0];
	m_position[1] += amount * m_rightVector[1];
	m_position[2] += amount * m_rightVector[2];

	//m_xTrans += amount;
	UpdateViewMatrix();
}

void Camera::DollyCamera(float amount)
{
	//m_rightVector

	m_position[0] += amount * m_direction[0];
	m_position[1] += amount * m_direction[1];
	m_position[2] += amount * m_direction[2];
	//m_zTrans -= amount;
	UpdateViewMatrix();
}

void Camera::PedCamera(float amount)
{
	//m_upVector
	m_position[0] += amount * m_upVector[0];
	m_position[1] += amount * m_upVector[1];
	m_position[2] += amount * m_upVector[2];

	//m_yTrans += amount;
	UpdateViewMatrix();
}

void Camera::RotateCamera(float yaw, float pitch, float roll)
{
	// https://www.youtube.com/watch?v=Ne3RNhEVSIE
	//yaw - up vector
	//pitch - right vector
	//roll - direction vector
	// Times em all together
	// m_rightVector = 
	// quaternion0 = (scalar 0, vector to be rotated along)
	// vector to be rotated = rotationQuaternion * quaternion0 * inverseRotationQuaternion
	// Yaw stuff
	if (yaw != 0 || pitch != 0 || roll != 0)
	{
		/*glm::vec3 upVec(m_viewMatrix[0].x, m_viewMatrix[0].y, m_viewMatrix[0].z);
		glm::vec3 rightVec(m_viewMatrix[1].x, m_viewMatrix[1].y, m_viewMatrix[1].z);
		glm::vec3 dirVec(-m_viewMatrix[2].x, -m_viewMatrix[2].y, -m_viewMatrix[2].z);*/
		glm::vec3 upVec(m_upVector[0], m_upVector[1], m_upVector[2]);
		glm::vec3 rightVec(m_rightVector[0], m_rightVector[1], m_rightVector[2]);
		glm::vec3 dirVec(m_direction[0], m_direction[1], m_direction[2]);
		//glm::vec3 posVec(m_position[0], m_position[1], m_position[2]);

		glm::quat yawQuat = glm::quat(cos(yaw / 2.0f), upVec * sin(yaw / 2.0f));
		glm::quat pitchQuat = glm::quat(cos(pitch / 2.0f), rightVec * sin(pitch / 2.0f));
		glm::quat rollQuat = glm::quat(cos(roll / 2.0f), dirVec * sin(roll / 2.0f));

		glm::quat rotation = pitchQuat * rollQuat * yawQuat;

		//glm::vec3 euler = glm::radians(glm::eulerAngles(rotation));
		glm::vec3 euler = glm::eulerAngles(rotation) * 3.14159f / 180.0f;

		m_viewMatrix *= glm::mat4(euler[0]);

		m_upVector = Vector3((upVec * rotation).x, (upVec * rotation).y, (upVec * rotation).z);
		m_rightVector = Vector3((rightVec * rotation).x, (rightVec * rotation).y, (rightVec * rotation).z);
		//m_viewVector = Vector3((viewVec * rotation).x, (viewVec * rotation).y, (viewVec * rotation).z);
		m_direction = Vector3((dirVec * rotation).x, (dirVec * rotation).y, (dirVec * rotation).z);

		UpdateViewMatrix();
	}
}

void Camera::ZoomCamera(float amount)
{
	m_fov -= amount;
	printf("Field of view: %f\n", m_fov);
	UpdateProjectionMatrix();
}

glm::mat4x4 Camera::SkyboxPos()
{
	glm::vec3* eye = new glm::vec3(m_position[0], m_position[1], m_position[2]);
	glm::vec3* center = new glm::vec3(m_position[0] + m_direction[0], m_position[1] + m_direction[1], m_position[2] + m_direction[2]);
	glm::vec3* up = new glm::vec3(m_upVector[0], m_upVector[1], m_upVector[2]);
	return glm::lookAt(*eye, *center, *up);
}

glm::mat4x4 Camera::Rotate(float pitch, float yaw, float roll)
{
	float pitchVal = glm::radians(pitch  / 2.0f);
	float yawVal = glm::radians(yaw / 2.0f);
	float rollVal = glm::radians(roll / 2.0f);

	float sinPitch = sin(pitchVal);
	float sinYaw = sin(yawVal);
	float sinRoll = sin(rollVal);

	float cosPitch = cos(pitchVal);
	float cosYaw = cos(yawVal);
	float cosRoll = cos(rollVal);

	glm::fquat rotation;
	rotation.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	rotation.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	rotation.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	rotation.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

	return glm::mat4x4(rotation);
}