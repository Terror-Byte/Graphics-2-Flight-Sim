#pragma once

#include "RenderWindow.h"
#include "OGLCube.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include "camera.h"
#include "SkyBox.h"
#include "InputHandler.h"

class OGLWindow : public RenderWindow
{
	private:
		float	    m_euler[3];
		HDC			m_hdc;				//handle to a device context
		HGLRC		m_hglrc;			//handle to a gl rendering context

		int			m_width;
		int			m_height;
		
		//This is not an ideal place to hold geometry data
		//int				m_renderableItems = 3;
		//Renderable		*m_renderables[3];
		std::vector<Renderable*> m_renderablesVec;

		Renderable				*m_mesh;
		OGLTexture				*m_texture;

		Renderable				*m_spaceship;
		OGLTexture				*m_shipTexture;

		Renderable				*m_camShip;
		OGLTexture				*m_camTexture;

		Renderable				*m_city;
		OGLTexture				*m_cityTexture;

		SkyBox					*m_skybox;

		InputHandler			*m_input;

		//Declear an OGL shader program
		OGLShaderProgram		*m_shader;
		int						m_uniform_modelview;
		int						m_uniform_projection;
		int						m_uniform_model;
		int						m_uniform_rotation;
		int						m_uniform_lightvec;
		int                     m_uniform_texture;
		int						m_texDefaultSampler;
		int						m_uniform_ambient;
		int						m_uniform_specular;
		int						m_uniform_shininess;
		int						m_uniform_campos;

		// Skybox shader
		OGLShaderProgram		*m_skyShader;
		int						m_uniform_modelview_sky;
		int						m_uniform_projection_sky;
		int                     m_uniform_texture_sky;
		int						m_uniform_model_sky;

		// Camera ship shader
		OGLShaderProgram		*m_shipShader;
		int						m_uniform_modelview_ship;
		int						m_uniform_projection_ship;
		int						m_uniform_texture_ship;

		glm::vec4				m_directionalLight;
		Camera					worldCam;
		float					wheelDeltaOld = NULL;

		bool					m_mouseDown = false;
		bool					m_mouseBound = false;
		Vector3					clickPos;

	protected:

		HGLRC					CreateOGLContext (HDC hdc);
		BOOL					DestroyOGLContext();
		void					InitOGLState();

	public:
								OGLWindow();
								OGLWindow(HINSTANCE hInstance, int width, int height);
								~OGLWindow();
		
		BOOL					InitWindow(HINSTANCE hInstance, int width, int height);

		void					Render();
		void					Resize(int width, int height);
		void					DestroyRenderWindow();

		BOOL					MouseLBDown ( int x, int y );
		BOOL					MouseLBUp ( int x, int y );
		BOOL					MouseMove ( int x, int y );
		BOOL					KeyPress( WPARAM key);
		BOOL					KeyRelease( WPARAM key );
		BOOL					MouseScroll(WPARAM scroll);

		glm::mat4x4				RotateObject(float x, float y, float z);

		float					m_yaw;
		float					m_pitch;
		float					m_roll;

		void					CalculateAABB() { ; };
		RayHitResult IntersectByRay(Ray& ray) { return Ray::s_defaultHitResult; };
		void					ShootRay();
		void					CalcCollision();
};
