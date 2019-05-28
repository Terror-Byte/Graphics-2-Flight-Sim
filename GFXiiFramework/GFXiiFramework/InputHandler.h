#include "camera.h"
#include "Renderable.h"

class  InputHandler
{
	private:
		bool			m_wDown;
		bool			m_aDown;
		bool			m_sDown;
		bool			m_dDown;
		bool			m_qDown;
		bool			m_eDown;
		bool			m_zDown;
		bool			m_xDown;
		bool			m_upDown;
		bool			m_downDown;
		bool			m_leftDown;
		bool			m_rightDown;
		bool			m_spaceDown;
		bool			*m_mouseDown;

		Camera			*m_cam;
		Renderable		*m_camShip;

		float			*m_roll;
		float			*m_pitch;
		float			*m_yaw;

		//float			m_shipRoll;
		//float			m_shipPitch;
		//float			m_shipYaw;

	public:
		
						InputHandler();
						~InputHandler();

		void			RunInput();

		inline bool		GetW() { return m_wDown; };
		inline void		SetW(bool w) { m_wDown = w; };

		inline bool		GetA() { return m_aDown; };
		inline void		SetA(bool a) { m_aDown = a; };

		inline bool		GetS() { return m_sDown; };
		inline void		SetS(bool s) { m_sDown = s; };

		inline bool		GetD() { return m_dDown; };
		inline void		SetD(bool d) { m_dDown = d; };

		inline bool		GetQ() { return m_qDown; };
		inline void		SetQ(bool q) { m_qDown = q; };

		inline bool		GetE() { return m_eDown; };
		inline void		SetE(bool e) { m_eDown = e; };

		inline bool		GetZ() { return m_zDown; };
		inline void		SetZ(bool z) { m_zDown = z; };

		inline bool		GetX() { return m_xDown; };
		inline void		SetX(bool x) { m_xDown = x; };

		inline bool     GetSpace() { m_spaceDown; };
		inline void		SetSpace(bool space) { m_spaceDown = space; };

		inline void		SetUp(bool up) { m_upDown = up; };
		inline void		SetDown(bool down) { m_downDown = down; };
		inline void		SetLeft(bool left) { m_leftDown = left; };
		inline void		SetRight(bool right) { m_rightDown = right; };

		inline void		SetCam(Camera* inCam) { m_cam = inCam; };
		inline void		SetRoll(float* roll) { m_roll = roll; };
		inline void		SetPitch(float* pitch) { m_pitch = pitch; };
		inline void		SetYaw(float* yaw) { m_yaw = yaw; };
		inline void		SetMouse(bool* mouse) { m_mouseDown = mouse; };

		//inline float	GetShipRoll() { return m_shipRoll; };
		//inline float	GetShipPitch() { return m_shipPitch; };
		//inline float	GetShipYaw() { return m_shipYaw; };

		inline void		SetShip(Renderable* ship) { m_camShip = ship; };
		//inline void		SetShipRoll(float roll) { m_shipRoll = roll; };
		//inline void		SetShipPitch(float pitch) { m_shipPitch = pitch; };
		//inline void		SetShipYaw(float yaw) { m_shipYaw = yaw; };

		void			UpdateShipPos();
};