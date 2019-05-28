#include "InputHandler.h"

InputHandler::InputHandler()
{
	m_wDown = false;
	m_aDown = false;
	m_sDown = false;
	m_dDown = false;
	m_qDown = false;
	m_eDown = false;
	m_zDown = false;
	m_xDown = false;
}

InputHandler::~InputHandler()
{
}

void InputHandler::RunInput()
{
	float movAmount = 1.0f;

	// Dollying (forewards/backwards motion)
	if (m_wDown)
		m_cam->DollyCamera(movAmount * 2);

	if (m_sDown)
		m_cam->DollyCamera(-movAmount * 2);

	// Strafing (left/right motion)
	if (m_aDown)
		m_cam->StrafeCamera(-movAmount);

	if (m_dDown)
		m_cam->StrafeCamera(movAmount);

	// Pedestalling (up/down motion)
	if (m_zDown)
		m_cam->PedCamera(movAmount);

	if (m_xDown)
		m_cam->PedCamera(-movAmount);

	// Roll rotation
	if (m_qDown)
	{
		*m_roll = 0.01f;
		//m_shipRoll += 0.575f;
	}

	if (m_eDown)
	{
		*m_roll = -0.01f; 
		//m_shipRoll -= 0.575f;
	}

	if (m_upDown)
	{
		*m_pitch = -0.01f;
		//m_shipPitch -= 0.575f;
	}

	if (m_downDown)
	{
		*m_pitch = 0.01f;
		//m_shipPitch += 0.575f;
	}

	if (m_leftDown)
	{
		*m_yaw = -0.01f;
		//m_shipYaw += 0.575;
	}

	if (m_rightDown)
	{
		*m_yaw = 0.01f;
		//m_shipYaw -= 0.575;
	}
	
	if (!m_qDown && !m_eDown)
		*m_roll = 0.0f;

	if (!m_upDown && !m_downDown && !m_spaceDown)
		*m_pitch = 0.0f;

	if (!m_leftDown && !m_rightDown && !m_spaceDown)
		*m_yaw = 0.0f;

	if (m_spaceDown)
		*m_mouseDown = true;
	else
		*m_mouseDown = false;
	//UpdateShipPos();
}

/*void InputHandler::UpdateShipPos()
{
	m_camShip->SetPosition(*m_cam->GetCameraPosition());
	m_camShip->SetRotation(Vector3(m_shipYaw, m_shipPitch, m_shipRoll));
	Vector3 newPos;
	newPos = m_camShip->GetPosition();
	newPos[1] -= 5;
	newPos[2] -= 30;
	m_camShip->SetPosition(newPos);
}*/

/*m_camShip->SetPosition(*worldCam.GetCameraPosition());
	Vector3 newPos;
	newPos = m_camShip->GetPosition();
	newPos[1] -= 5;
	newPos[2] -= 30;
	m_camShip->SetPosition(newPos);
	return;*/