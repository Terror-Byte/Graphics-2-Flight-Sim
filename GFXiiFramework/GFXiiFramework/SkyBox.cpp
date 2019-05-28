#include "SkyBox.h"
#include "GLEW/include/glew.h"
#include <Windows.h>

void SkyBox::Init()
{
	OGLTexture * texture;
	// Front
	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_rt.tga");
	//m_textures[0] = &static_cast<BaseTexture>(*texture);
	m_textures[0] = texture;
	
	// Back
	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_ft.tga");
	//m_textures[1] = &static_cast<BaseTexture>(*texture);
	m_textures[1] = texture;
	
	// Left
	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_lt.tga");
	//m_textures[2] = &static_cast<BaseTexture>(*texture);
	m_textures[2] = texture;
	
	// Right
	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_bk.tga");
	//m_textures[3] = &static_cast<BaseTexture>(*texture);
	m_textures[3] = texture;
	
	// Top
	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_tp.tga");
	//m_textures[4] = &static_cast<BaseTexture>(*texture);
	m_textures[4] = texture;
	
	// Bottom
	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_bt.tga");
	//m_textures[5] = &static_cast<BaseTexture>(*texture);
	m_textures[5] = texture;

	texture = new OGLTexture();
	texture->CreateTextureFromFile("../asset/texture/sky_bk.tga");
	this->SetTexture(texture);

	float positionMod = 5.0f;

	SimpleVertex corners[] =
	{
		{ -0.5f - positionMod, -0.5f - positionMod, 0.5f + positionMod, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },// 0
		{ 0.5f + positionMod, -0.5f - positionMod, 0.5f + positionMod, 1.0f, 0.0f, 0.0f, /*1.0f*/1.0f, /*0.0*/0.0f }, // 1
		{ 0.5f + positionMod, 0.5f + positionMod, 0.5f + positionMod, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f }, // 2
		{ -0.5f - positionMod, 0.5f + positionMod, 0.5f + positionMod, 0.0f, 1.0f, 0.0f, /*0.0f*/0.0f, /*1.0f*/1.0f }, // 3

		{ 0.5f + positionMod, -0.5f - positionMod, 0.5f + positionMod, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f }, // 4
		{ 0.5f + positionMod, -0.5f - positionMod, -0.5f - positionMod, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // 5
		{ 0.5f + positionMod, 0.5f + positionMod, -0.5f - positionMod, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 6
		{ 0.5f + positionMod, 0.5f + positionMod, 0.5f + positionMod, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // 7

		{ 0.5f + positionMod, -0.5f - positionMod, -0.5f - positionMod, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f }, // 8
		{ -0.5f - positionMod, -0.5 - positionMod, -0.5f - positionMod, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f }, //9
		{ -0.5f - positionMod, 0.5 + positionMod, -0.5f - positionMod, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 10
		{ 0.5f + positionMod, 0.5f + positionMod, -0.5f - positionMod, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 11

		{ -0.5f - positionMod, -0.5 - positionMod, -0.5f - positionMod, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }, //12
		{ -0.5f - positionMod, -0.5f - positionMod, 0.5f + positionMod, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f },// 13
		{ -0.5f - positionMod, 0.5f + positionMod, 0.5f + positionMod, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f }, // 14
		{ -0.5f - positionMod, 0.5 + positionMod, -0.5f - positionMod, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 15

		{ -0.5f - positionMod, 0.5f + positionMod, 0.5f + positionMod, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f }, // 16
		{ 0.5f + positionMod, 0.5f + positionMod, 0.5f + positionMod, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f }, // 17
		{ 0.5f + positionMod, 0.5f + positionMod, -0.5f - positionMod, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f }, // 18
		{ -0.5f - positionMod, 0.5 + positionMod, -0.5f - positionMod, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f }, // 19

		{ -0.5f - positionMod, -0.5 - positionMod, -0.5f - positionMod, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f }, //20
		{ 0.5f + positionMod, -0.5f - positionMod, -0.5f - positionMod, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f }, // 21
		{ 0.5f + positionMod, -0.5f - positionMod, 0.5f + positionMod, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f }, // 22
		{ -0.5f - positionMod, -0.5f - positionMod, 0.5f + positionMod, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }// 23
	};

	//We will use triangles instead of triangle strips
	unsigned int indices[] =
	{
		/*0, 1, 3,
		1, 2, 3,

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 14, 15,
		12, 13, 14,

		16, 18, 19,
		16, 17, 18,

		20, 22, 23,
		20, 21, 22*/
		3, 1, 0,
		3, 2, 1,

		7, 5, 4,
		7, 6, 5,

		11, 9, 8,
		11, 10, 9,

		15, 14, 12,
		14, 13, 12,

		19, 18, 16,
		18, 17, 16,

		23, 22, 20,
		22, 21, 20
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(SimpleVertex), corners, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 12));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 24));
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void SkyBox::Render()
{
	//==========OGL CUBE'S RENDER, SUBJECT TO CHANGE==========
	/*unsigned int texHandle = dynamic_cast<OGLTexture*>(m_tex)->m_syshandle;
	//unsigned int texHandle = dynamic_cast<OGLTexture*>(m_textures[0])->m_syshandle;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);*/
	//========================================================

	for (int i = 0; i < 6; i++)
	{
		unsigned int texHandle = dynamic_cast<OGLTexture*>(m_textures[i])->m_syshandle;
		//unsigned int texHandle = m_textures[i]->m_syshandle;
		//unsigned int texHandle = dynamic_cast<OGLTexture*>(m_tex)->m_syshandle;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texHandle);

		glBindVertexArray(m_vao);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)((6 * i) * sizeof(GLuint)));

		glBindVertexArray(0);
	}
}

/* Notes n shiet
	Cubemap:
		http://antongerdelan.net/opengl/cubemaps.html
		http://ogldev.atspace.co.uk/www/tutorial25/tutorial25.html

	Drawelements:
		https://www.opengl.org/sdk/docs/man2/xhtml/glDrawElements.xml

	Skybox shiet:
		https://courseresources.derby.ac.uk/bbcswebdav/pid-1406522-dt-content-rid-11506588_1/courses/2014-SPR-OC-KED-5CC510/5CC510_LecWeek05.pdf
		https://courseresources.derby.ac.uk/bbcswebdav/pid-1411872-dt-content-rid-11517787_1/courses/2014-SPR-OC-KED-5CC510/prac04.pdf
*/