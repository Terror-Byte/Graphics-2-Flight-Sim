#include "OpenGL\OGLCube.h"
#include "BaseTexture.h"

class SkyBox : public OGLCube
{
	private:
		BaseTexture			*m_textures[6];
		//OGLTexture				*m_textures[6];
	public:
		
		void				Init();
		virtual void		Render();
};