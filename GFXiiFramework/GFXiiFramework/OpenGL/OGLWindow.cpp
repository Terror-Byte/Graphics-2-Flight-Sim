#include "OGLWindow.h"
#include "Resource.h"
#include "GLEW/include/glew.h"
#include <time.h>
#include <random>

#define VK_W 0x57
#define VK_A 0x41
#define VK_S 0x53
#define VK_D 0x44
#define VK_Z 0x5A
#define VK_X 0x58
#define VK_Q 0x51
#define VK_E 0x45
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20

OGLWindow::OGLWindow()
{
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete m_mesh;
	delete m_shader;
	delete m_skybox;
	delete m_input;
	
	DestroyOGLContext();
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height); // THis was commented out for some reason
	m_euler[0] = m_euler[1] = m_euler[2] = 0.0f;
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyRenderWindow()
{
	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{

	glDeleteSamplers( 1, (GLuint*)(&m_texDefaultSampler) );

	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) )
		return FALSE;

	glewInit();

	InitOGLState();

	m_width = width;
	m_height = height;

	//==========HOUSE==========
	m_mesh = new OGLMesh(L"../asset/models/house.obj");
	m_mesh->SetPosition(Vector3(0.0f, 5.0f, -100.0f));
	m_mesh->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	m_mesh->SetScale(Vector3(10.0f, 10.0f, 10.0f));
	m_mesh->SetAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	m_mesh->SetSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_mesh->SetShininess(30.0f);
	//m_mesh->m_boundingBox.center = glm::vec3(m_mesh->GetPosition()[0], m_mesh->GetPosition()[1], m_mesh->GetPosition()[2]);
	//m_mesh->m_boundingBox.sizeX = m_mesh->m_boundingBox.sizeX * m_mesh->GetScale()[0];
	//m_mesh->m_boundingBox.sizeY = m_mesh->m_boundingBox.sizeY * m_mesh->GetScale()[1];
	//m_mesh->m_boundingBox.sizeZ = m_mesh->m_boundingBox.sizeZ * m_mesh->GetScale()[2];
	m_mesh->SetDestructible(true);

	m_texture = new OGLTexture();
	m_texture->CreateTextureFromFile("../asset/texture/house_diffuse.tga");

	m_mesh->SetTexture(m_texture);
	//=========================

	//==========SPACESHIP==========
	m_spaceship = new OGLMesh(L"../asset/models/dark_fighter_6.obj");
	m_spaceship->SetPosition(Vector3(50.0f, 5.0f, -500.0f));
	m_spaceship->SetRotation(Vector3(90.0f, 0.0f, 0.0f));
	m_spaceship->SetScale(Vector3(5.0f, 5.0f, 5.0f));
	m_spaceship->SetAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	m_spaceship->SetSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_spaceship->SetShininess(30.0f);
	//m_spaceship->m_boundingBox.center = glm::vec3(m_spaceship->GetPosition()[0], m_spaceship->GetPosition()[1], m_spaceship->GetPosition()[2]);
	//m_spaceship->m_boundingBox.sizeX = m_spaceship->m_boundingBox.sizeX * m_spaceship->GetScale()[0];
	//m_spaceship->m_boundingBox.sizeY = m_spaceship->m_boundingBox.sizeY * m_spaceship->GetScale()[1];
	//m_spaceship->m_boundingBox.sizeZ = m_spaceship->m_boundingBox.sizeZ * m_spaceship->GetScale()[2];
	m_spaceship->SetDestructible(true);

	m_shipTexture = new OGLTexture();
	m_shipTexture->CreateTextureFromFile("../asset/texture/dark_fighter_6_color.tga");

	m_spaceship->SetTexture(m_shipTexture);
	//=============================

	//==========CAMERA SHIP==========
	m_camShip = new OGLMesh(L"../asset/models/space_frigate_7.obj");
	m_camShip->SetPosition(Vector3(0.0f, -5.0f, -30.0f));
	m_camShip->SetRotation(Vector3(-90.0f, 0.0f, 0.0f));
	m_camShip->SetScale(Vector3(1.0f, 1.0f, 1.0f));
	m_camShip->SetAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	m_camShip->SetSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_camShip->SetShininess(30.0f);

	Vector3 camPos = *(worldCam.GetCameraPosition());
	Vector3 camDir = *(worldCam.GetCameraDirection());
	Vector3 shipPos = m_camShip->GetPosition();
	m_camShip->m_boundingBox.center = glm::vec3((camPos[0] + (camDir[0] * shipPos[0])), (camPos[1] + (camDir[1] * shipPos[1])), (camPos[2] + (camDir[2] * shipPos[2])));
	m_camShip->m_boundingBox.sizeX = m_camShip->m_boundingBox.sizeX * m_camShip->GetScale()[0];
	m_camShip->m_boundingBox.sizeY = m_camShip->m_boundingBox.sizeY * m_camShip->GetScale()[1];
	m_camShip->m_boundingBox.sizeZ = m_camShip->m_boundingBox.sizeZ * m_camShip->GetScale()[2];
	m_camShip->SetDestructible(false);

	m_camTexture = new OGLTexture();
	m_camTexture->CreateTextureFromFile("../asset/texture/space_frigate_6_color.tga");

	m_camShip->SetTexture(m_camTexture);
	//===============================

	//==========CITY==========
	m_city = new OGLMesh(L"../asset/models/scifi_city2.obj");
	m_city->SetPosition(Vector3(0.0f, -100.0f, 0.0f));
	m_city->SetRotation(Vector3(0.0f, 0.0f, 0.0f));
	m_city->SetScale(Vector3(10.0f, 10.0f, 10.0f));
	m_city->SetAmbient(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
	m_city->SetSpecular(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
	m_city->SetShininess(30.0f);
	m_city->m_boundingBox.sizeY /= 2.0f;
	//m_city->m_boundingBox.center = glm::vec3(m_city->GetPosition()[0], m_city->GetPosition()[1], m_city->GetPosition()[2]);
	//m_city->m_boundingBox.sizeX = m_city->m_boundingBox.sizeX * m_city->GetScale()[0];
	//m_city->m_boundingBox.sizeY = m_city->m_boundingBox.sizeY * m_city->GetScale()[1];
	//m_city->m_boundingBox.sizeZ = m_city->m_boundingBox.sizeZ * m_city->GetScale()[2];
	m_city->SetDestructible(false);

	m_cityTexture = new OGLTexture();
	m_cityTexture->CreateTextureFromFile("../asset/texture/SciFi_HumanCity_Diffuse-Map.tga");

	m_city->SetTexture(m_cityTexture);
	//========================

	//==========LIGHT STUFF==========
	m_directionalLight = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//===============================

	m_renderablesVec.push_back(m_mesh);
	m_renderablesVec.push_back(m_spaceship);
	m_renderablesVec.push_back(m_city);

	m_skybox = new SkyBox();
	m_skybox->Init();

	worldCam.InitCamera();
	worldCam.SetCameraAspectRatio(1.7777777777f);
	worldCam.SetCameraFOV(60);
	worldCam.SetCameraPosition(new Vector3(1.0f, 1.0f, 10.0f));
	worldCam.SetLookAtPoint(new Vector3(1.0f, 1.0f, 1.0f));

	m_yaw = 0.0f;
	m_pitch = 0.0f;
	m_roll = 0.0f;

	m_input = new InputHandler();
	m_input->SetCam(&worldCam);
	m_input->SetRoll(&m_roll);
	m_input->SetPitch(&m_pitch);
	m_input->SetYaw(&m_yaw);
	m_input->SetShip(m_camShip);
	m_input->SetMouse(&m_mouseDown);
	return TRUE;
}

void OGLWindow::Render()
{
	//if (m_mouseBound)
	//{
	//	SetCursorPos(m_width / 2, m_height / 2);
	//}
	worldCam.RotateCamera(m_yaw, m_pitch, m_roll);

	Vector3 shipPos = m_spaceship->GetPosition();
	//m_spaceship->SetPosition(Vector3(shipPos[0], shipPos[1], shipPos[2] +=1 ));

	//m_euler[0] = 15;//m_euler[0] > 360.0 ? 0 : m_euler[0] + 1.0;
	//m_euler[1] = m_euler[1] > 360.0 ? 0 : m_euler[1] + .01;
	//m_euler[2] = m_euler[2] > 360.0 ? 0 : m_euler[2] + .01;

	m_input->RunInput();
	CalcCollision();


	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();

	//==========SKYBOX STUFF==========
	m_skyShader->ActivateShaderProgram();
	m_skybox->SetPosition(*worldCam.GetCameraPosition());

	glm::mat4x4 modelViewMatrix = *worldCam.GetViewMatrix();
	float* modelview = glm::value_ptr(modelViewMatrix);
	glm::mat4x4 projViewMatrix = *worldCam.GetProjMatrix();
	float* projection = glm::value_ptr(projViewMatrix);
	glm::mat4x4 modelMatrix = glm::mat4x4();
	modelMatrix[3].x = m_skybox->GetPosition()[0];
	modelMatrix[3].y = m_skybox->GetPosition()[1];
	modelMatrix[3].z = m_skybox->GetPosition()[2];
	modelMatrix[3].w = 1.0f;
	float* model = glm::value_ptr(modelMatrix);

	glUniformMatrix4fv(m_uniform_modelview_sky, 1, GL_FALSE, modelview);
	glUniformMatrix4fv(m_uniform_projection_sky, 1, GL_FALSE, projection);
	glUniformMatrix4fv(m_uniform_model_sky, 1, GL_FALSE, model);

	glBindSampler(0, m_texDefaultSampler);
	glDisable(GL_DEPTH_TEST);
	m_skybox->Render();
	glEnable(GL_DEPTH_TEST);
	m_skyShader->DeactivateShaderProgram();
	//================================

	//==========CAMERA SHIP STUFF==========
	m_shipShader->ActivateShaderProgram();

	//modelViewMatrix = *worldCam.GetViewMatrix();
	//modelview = glm::value_ptr(modelViewMatrix);

	projViewMatrix = *worldCam.GetProjMatrix();
	projection = glm::value_ptr(projViewMatrix);

	modelMatrix = glm::mat4x4();
	modelMatrix[3].x = m_camShip->GetPosition()[0];
	modelMatrix[3].y = m_camShip->GetPosition()[1];
	modelMatrix[3].z = m_camShip->GetPosition()[2];
	modelMatrix[3].w = 1.0f;
	
	// rotation
	glm::mat4x4 rotationMatrixX = RotateObject(m_camShip->GetRotation()[0], 0.0f, 0.0f);
	glm::mat4x4 rotationMatrixY = RotateObject(0.0f, m_camShip->GetRotation()[1], 0.0f);
	glm::mat4x4 rotationMatrixZ = RotateObject(0.0f, 0.0f, m_camShip->GetRotation()[2]);
	glm::mat4x4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
	glm::mat4x4 scaleMat = glm::mat4x4();
	scaleMat[0].x = m_camShip->GetScale()[0];
	scaleMat[1].y = m_camShip->GetScale()[1];
	scaleMat[2].z = m_camShip->GetScale()[2];
	modelMatrix = modelMatrix * rotationMatrix * scaleMat;
	model = glm::value_ptr(modelMatrix);

	m_camShip->CalculateAABB();
	Vector3 camPos = *(worldCam.GetCameraPosition());
	Vector3 camDir = *(worldCam.GetCameraDirection());
	shipPos = m_camShip->GetPosition();
	m_camShip->m_boundingBox.center = glm::vec3((camPos[0] + (camDir[0] * shipPos[0])), (camPos[1] + (camDir[1] * shipPos[1])), (camPos[2] + (camDir[2] * shipPos[2])));
	m_camShip->m_boundingBox.sizeX = m_camShip->m_boundingBox.sizeX * m_camShip->GetScale()[0];
	m_camShip->m_boundingBox.sizeY = m_camShip->m_boundingBox.sizeY * m_camShip->GetScale()[1];
	m_camShip->m_boundingBox.sizeZ = m_camShip->m_boundingBox.sizeZ * m_camShip->GetScale()[2];

	glUniformMatrix4fv(m_uniform_modelview_ship, 1, GL_FALSE, model);
	glUniformMatrix4fv(m_uniform_projection_ship, 1, GL_FALSE, projection);

	glBindSampler(0, m_texDefaultSampler);
	m_camShip->Render();
	m_shipShader->DeactivateShaderProgram();
	//=====================================

	//==========OTHER RENDERABLE OBJECTS==========
	m_shader->ActivateShaderProgram();
	for (int i = 0; i < m_renderablesVec.size(); i++)
	{
		glm::mat4x4 modelMatrix = glm::mat4x4();
		modelMatrix[3].x = m_renderablesVec[i]->GetPosition()[0];
		modelMatrix[3].y = m_renderablesVec[i]->GetPosition()[1];
		modelMatrix[3].z = m_renderablesVec[i]->GetPosition()[2];
		modelMatrix[3].w = 1.0f;

		glm::mat4x4 rotationMatrixX = RotateObject(m_renderablesVec[i]->GetRotation()[0], 0.0f, 0.0f);
		glm::mat4x4 rotationMatrixY = RotateObject(0.0f, m_renderablesVec[i]->GetRotation()[1], 0.0f);
		glm::mat4x4 rotationMatrixZ = RotateObject(0.0f, 0.0f, m_renderablesVec[i]->GetRotation()[2]);
		glm::mat4x4 rotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;
		glm::mat4x4 scaleMat = glm::mat4x4();
		scaleMat[0].x = m_renderablesVec[i]->GetScale()[0];
		scaleMat[1].y = m_renderablesVec[i]->GetScale()[1];
		scaleMat[2].z = m_renderablesVec[i]->GetScale()[2];
		modelMatrix = modelMatrix * rotationMatrix * scaleMat;
		float* model = glm::value_ptr(modelMatrix);

		// Pass camera stuff into shader (My stuff)
		glm::mat4x4 modelViewMatrix = *worldCam.GetViewMatrix();
		float* modelview = glm::value_ptr(modelViewMatrix);

		glm::mat4x4 projViewMatrix = *worldCam.GetProjMatrix();
		float* projection = glm::value_ptr(projViewMatrix);

		float* lightpos = glm::value_ptr(m_directionalLight);
		glm::vec4 tempAmb = m_renderablesVec[i]->GetAmbient();
		float* ambient = glm::value_ptr(tempAmb);
		glm::vec4 tempSpec = m_renderablesVec[i]->GetSpecular();
		float *specular = glm::value_ptr(tempSpec);
		float tempFloat = m_renderablesVec[i]->GetShininess();
		float *shininess = &tempFloat;
		
		Vector3 tempCamPosVec = *worldCam.GetCameraPosition();
		glm::vec4 tempCamPos = glm::vec4(tempCamPosVec[0], tempCamPosVec[1], tempCamPos[2], 1.0f);
		float *campos = glm::value_ptr(tempCamPos);

		m_renderablesVec[i]->CalculateAABB();
		m_renderablesVec[i]->m_boundingBox.center = glm::vec3(m_renderablesVec[i]->GetPosition()[0], m_renderablesVec[i]->GetPosition()[1], m_renderablesVec[i]->GetPosition()[2]);

		// Explainationg for this stuff: https://www.khronos.org/opengles/sdk/docs/man/xhtml/glUniform.xml
		glUniformMatrix4fv(m_uniform_modelview, 1, GL_FALSE, modelview);
		glUniformMatrix4fv(m_uniform_projection, 1, GL_FALSE, projection);
		glUniformMatrix4fv(m_uniform_model, 1, GL_FALSE, model);
		glUniformMatrix4fv(m_uniform_lightvec, 1, GL_FALSE, lightpos);
		glUniformMatrix4fv(m_uniform_ambient, 1, GL_FALSE, ambient);
		glUniformMatrix4fv(m_uniform_specular, 1, GL_FALSE, specular);
		glUniformMatrix4fv(m_uniform_shininess, 1, GL_FALSE, shininess);
		glUniformMatrix4fv(m_uniform_campos, 1, GL_FALSE, campos);

		glBindSampler(0, m_texDefaultSampler);

		m_renderablesVec[i]->Render();
	}
	m_shader->DeactivateShaderProgram();
	//============================================

	glBegin(GL_LINES);
	glVertex2f(0.01f, 0.0f);
	glVertex2f(-0.011f, 0.0f);
	glVertex2f(0.0f, 0.015f);
	glVertex2f(0.0f, -0.016f);
	glEnd();

	SwapBuffers(m_hdc);

	return;
}

void OGLWindow::Resize( int width, int height )
{
	float aspect_ratio = (float)width/(float)height;

	m_width = width;
	m_height = height;

	glViewport( 0, 0, width, height );
	
	glMatrixMode( GL_PROJECTION ); // Unsure if needed
	glLoadIdentity();

	// Minsi's default stuff
	//glFrustum( -5*aspect_ratio, 5*aspect_ratio, -5, 5, 1.0, 100.0 );
	//gluPerspective(60.0, aspect_ratio, 1.0, 1000.0);

	worldCam.SetCameraAspectRatio(aspect_ratio);
	worldCam.UpdateProjectionMatrix();
	worldCam.UpdateViewMatrix();
	
	glMatrixMode( GL_MODELVIEW ); // Unsure if needed
	glLoadIdentity();

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//Initialise OGL shader
	m_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	m_shader->BindAttributeLocation( 0, "position" );
	m_shader->BindAttributeLocation( 1, "inNormal" );
	m_shader->BindAttributeLocation( 2, "inUV" );

	glBindFragDataLocation( m_shader->GetProgramHandle(), 0, "outFrag" );

	m_shader->BuildShaderProgram();
	//m_shader->ActivateShaderProgram();

	m_uniform_modelview = glGetUniformLocation(m_shader->GetProgramHandle(), "modelview");
	m_uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	m_uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	m_uniform_rotation = glGetUniformLocation(m_shader->GetProgramHandle(), "rotation");
	m_uniform_lightvec = glGetUniformLocation(m_shader->GetProgramHandle(), "lightpos");
	m_uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
	m_uniform_ambient = glGetUniformLocation(m_shader->GetProgramHandle(), "ambient");
	m_uniform_specular = glGetUniformLocation(m_shader->GetProgramHandle(), "specular");
	m_uniform_shininess = glGetUniformLocation(m_shader->GetProgramHandle(), "shininess");
	m_uniform_campos = glGetUniformLocation(m_shader->GetProgramHandle(), "campos");

	glUniform1i(m_uniform_texture, 0);

	// Initialise skybox shader
	m_skyShader = new OGLShaderProgram();

	m_skyShader->CreateShaderProgram();
	m_skyShader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.vert", SHADER_VERTEX);
	m_skyShader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.frag", SHADER_FRAGMENT);

	m_skyShader->BindAttributeLocation(0, "position");
	m_skyShader->BindAttributeLocation(1, "inNormal");
	m_skyShader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_skyShader->GetProgramHandle(), 0, "outFrag");

	m_skyShader->BuildShaderProgram();
	//m_skyShader->ActivateShaderProgram();

	m_uniform_modelview_sky = glGetUniformLocation(m_skyShader->GetProgramHandle(), "modelview");
	m_uniform_projection_sky = glGetUniformLocation(m_skyShader->GetProgramHandle(), "projection");
	m_uniform_texture_sky = glGetUniformLocation(m_skyShader->GetProgramHandle(), "texColour");
	m_uniform_model_sky = glGetUniformLocation(m_skyShader->GetProgramHandle(), "model");

	// Initialise camera ship shader
	m_shipShader = new OGLShaderProgram();

	m_shipShader->CreateShaderProgram();
	m_shipShader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/camship.vert", SHADER_VERTEX);
	m_shipShader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/camship.frag", SHADER_FRAGMENT);

	m_shipShader->BindAttributeLocation(0, "position");
	m_shipShader->BindAttributeLocation(1, "inNormal");
	m_shipShader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(m_shipShader->GetProgramHandle(), 0, "outFrag");

	m_shipShader->BuildShaderProgram();

	m_uniform_modelview_ship = glGetUniformLocation(m_shipShader->GetProgramHandle(), "modelview");
	m_uniform_projection_ship = glGetUniformLocation(m_shipShader->GetProgramHandle(), "projection");

	//Create a texture sampler
	glGenSamplers(1, (GLuint*)(&m_texDefaultSampler));

	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glSamplerParameteri(m_texDefaultSampler, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

BOOL OGLWindow::MouseLBDown(int x, int y)
{
	//clickPos[0] = x;
	//clickPos[1] = y;
	
	//worldCam.Rotate(0.0f, (x - m_width / 2), 0.0f);
	//worldCam.Rotate((y - m_height / 2), 0.0f, 0.0f);
	ShootRay();
	m_mouseBound = true;
	m_mouseDown = true;
	return TRUE;
}

BOOL OGLWindow::MouseLBUp(int x, int y)
{
	//clickPos[0] = 0.0f;
	//clickPos[1] = 0.0f;
	m_mouseDown = false;
	return TRUE;
}

BOOL OGLWindow::MouseMove(int x, int y)
{
	// Yaw = left/right, occurs along up vector
	// Pitching = up/down, occurs along right vector
	//worldCam.RotateCamera(xDiff, yDiff, 0);

	if (m_mouseBound && m_mouseDown)
	{
		if (x > ((m_width / 2) + 10))
		{
			//m_yaw = (x - m_width / 2) * 0.001;
			m_yaw = 0.01f;
		}
		else if (x < ((m_width / 2) - 10))
		{
			m_yaw = -0.01f;
		}
		else
		{
			m_yaw = 0.0f;
		}

		if (y > ((m_height / 2) + 10))
		{
			m_pitch = 0.01f;
		}
		else if (y < ((m_height / 2) - 10))
		{
			m_pitch = -0.01f;
		}
		else
		{
			m_pitch = 0.0f;
		}
	}
	else
	{
		//m_yaw = 0.0f;
		//m_pitch = 0.0f;
	}
	
	//clickPos[0] = x;
	//clickPos[1] = y;

	//if (m_mouseBound)
	//{
	//	SetCursorPos(m_width / 2, m_height / 2);
	//}
	return TRUE;
}

BOOL OGLWindow::KeyPress( WPARAM key )
{
	switch (key)
	{
		case VK_W:
			m_input->SetW(true);
			break;

		case VK_A:
			m_input->SetA(true);
			break;

		case VK_S:
			m_input->SetS(true);
			break;

		case VK_D:
			m_input->SetD(true);
			break;

		case VK_Z:
			m_input->SetZ(true);
			break;

		case VK_X:
			m_input->SetX(true);
			break;

		case VK_Q:
			m_input->SetQ(true);
			break;

		case VK_E:
			m_input->SetE(true);
			break;

		case VK_ESCAPE:
			m_mouseBound = false;
			break;

		case VK_UP:
			m_input->SetUp(true);
			break;

		case VK_DOWN:
			m_input->SetDown(true);
			break;

		case VK_LEFT:
			m_input->SetLeft(true);
			break;

		case VK_RIGHT:
			m_input->SetRight(true);
			break;

		case VK_SPACE:
			m_input->SetSpace(true);
			break;
	}
	return TRUE;
}

BOOL OGLWindow::KeyRelease( WPARAM key )
{
	switch (key)
	{
		case VK_W:
			m_input->SetW(false);
			break;

		case VK_A:
			m_input->SetA(false);
			break;

		case VK_S:
			m_input->SetS(false);
			break;

		case VK_D:
			m_input->SetD(false);
			break;

		case VK_Z:
			m_input->SetZ(false);
			break;

		case VK_X:
			m_input->SetX(false);
			break;

		case VK_Q:
			m_input->SetQ(false);
			break;

		case VK_E:
			m_input->SetE(false);
			break;

		case VK_UP:
			m_input->SetUp(false);
			break;

		case VK_DOWN:
			m_input->SetDown(false);
			break;

		case VK_LEFT:
			m_input->SetLeft(false);
			break;

		case VK_RIGHT:
			m_input->SetRight(false);
			break;

		case VK_SPACE:
			m_input->SetSpace(false);
			break;
	}
	return TRUE;
}

BOOL OGLWindow::MouseScroll(WPARAM scroll) // https://msdn.microsoft.com/en-us/library/windows/desktop/ms645617%28v=vs.85%29.aspx
{
	float wheelDelta = GET_WHEEL_DELTA_WPARAM(scroll);
	if (wheelDelta < 0) // Zoom out
	{
		worldCam.ZoomCamera(-1);
	}
	else if (wheelDelta > 0) // Zoom in
	{
		worldCam.ZoomCamera(1);
	}
	return TRUE;
}

glm::mat4x4 OGLWindow::RotateObject(float x, float y, float z)
{
	float pitch = glm::radians(x / 2.0f);
	float yaw = glm::radians(y / 2.0f);
	float roll = glm::radians(z / 2.0f);

	float sinPitch = sin(pitch);
	float sinYaw = sin(yaw);
	float sinRoll = sin(roll);

	float cosPitch = cos(pitch);
	float cosYaw = cos(yaw);
	float cosRoll = cos(roll);

	glm::fquat rotation;
	rotation.x = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
	rotation.y = cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw;
	rotation.z = cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw;
	rotation.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

	/*float magnitude = rotation.w * rotation.w + rotation.x * rotation.x + rotation.y * rotation.y + rotation.z * rotation.z;
	rotation.x /= magnitude;
	rotation.y /= magnitude;
	rotation.z /= magnitude;
	rotation.w /= magnitude;*/

	return glm::mat4x4(rotation);
}

void OGLWindow::ShootRay()
{
	Ray ray;
	Vector3 camPos = *(worldCam.GetCameraPosition());
	Vector3 camDir = *(worldCam.GetCameraDirection());
	glm::vec3 start = glm::vec3(camPos[0], camPos[1], camPos[2]);
	glm::vec3 dir = glm::vec3(camDir[0], camDir[1], camDir[2]);
	ray.SetRay(start, dir);

	for (int i = 0; i < m_renderablesVec.size(); i++)
	{
		if (m_renderablesVec[i]->GetDestructible())
		{
			RayHitResult result = m_renderablesVec[i]->IntersectByRay(ray);
			if (result.hit)
			{
				//srand(time(NULL));
				std::random_device rd;
				std::mt19937 mt(rd());
				std::uniform_real_distribution<double> dist(0, 500);

				float xPos = dist(mt);
				float zPos = dist(mt);

				dist = std::uniform_real_distribution<double>(0, 1.0);
				float sign = dist(mt);
				if (sign < 0.5)
					xPos = -xPos;

				sign = dist(mt);
				if (sign < 0.5)
					zPos = -zPos;

				m_renderablesVec[i]->SetPosition(Vector3(xPos, 5.0f, zPos));
				m_renderablesVec[i]->CalculateAABB();
				//m_renderablesVec[i]->m_boundingBox.center = glm::vec3(m_renderablesVec[i]->GetPosition()[0], m_renderablesVec[i]->GetPosition()[1], m_renderablesVec[i]->GetPosition()[2]);
				printf("Hit!\n");
			}
		}
	}
}

void OGLWindow::CalcCollision()
{
	for (int i = 0; i < m_renderablesVec.size(); i++)
	{
		glm::vec3 shipAABB = m_camShip->m_boundingBox.center;
		glm::vec3 shipMin = glm::vec3(shipAABB.x - (m_camShip->m_boundingBox.sizeX / 2), shipAABB.y - (m_camShip->m_boundingBox.sizeY / 2), shipAABB.z - (m_camShip->m_boundingBox.sizeZ / 2));
		glm::vec3 shipMax = glm::vec3(shipAABB.x + (m_camShip->m_boundingBox.sizeX / 2), shipAABB.y + (m_camShip->m_boundingBox.sizeY / 2), shipAABB.z + (m_camShip->m_boundingBox.sizeZ / 2));

		glm::vec3 otherAABB = m_renderablesVec[i]->m_boundingBox.center;
		glm::vec3 otherMin = glm::vec3(otherAABB.x - (m_renderablesVec[i]->m_boundingBox.sizeX / 2), otherAABB.y - (m_renderablesVec[i]->m_boundingBox.sizeY / 2), otherAABB.z - (m_renderablesVec[i]->m_boundingBox.sizeZ / 2));
		glm::vec3 otherMax = glm::vec3(otherAABB.x + (m_renderablesVec[i]->m_boundingBox.sizeX / 2), otherAABB.y + (m_renderablesVec[i]->m_boundingBox.sizeY / 2), otherAABB.z + (m_renderablesVec[i]->m_boundingBox.sizeZ / 2));

		float dist = 0.0f;

		dist = shipMin.x - otherMin.x;
		bool xUp = dist < otherMax.x;
		bool xDown = -dist < shipMax.x;

		dist = shipMin.y - otherMin.y;
		bool yUp = dist < otherMax.y;
		bool yDown = -dist < shipMax.y;

		dist = shipMin.z - otherMin.z;
		bool zUp = dist < otherMax.z;
		bool zDown = -dist < shipMax.z;

		if ((xUp || xDown) && (yUp || yDown) && (zUp || zDown))
		{
			switch (i)
			{
			case 0: printf("Collision house\n"); // house
				break;
			case 1: printf("Collision ship\n"); // ship
				break;
			case 2: printf("Collision city\n"); // city
				break;
			}
		}

		/*if ((dist = shipMin.x - otherMin.x) < otherMax.x)
		{
			Vector3 camPos = *(worldCam.GetCameraPosition());
			worldCam.SetCameraPosition(&Vector3(camPos[0]--, camPos[1], camPos[2]));
			printf("Collision X\n");
		}
		else if (-dist < shipMax.x)
		{
			Vector3 camPos = *(worldCam.GetCameraPosition());
			worldCam.SetCameraPosition(&Vector3(camPos[0]++, camPos[1], camPos[2]));
			printf("Collision X\n");
		}

		if ((dist = shipMin.y - otherMin.y) < otherMax.y)
		{
			Vector3 camPos = *(worldCam.GetCameraPosition());
			worldCam.SetCameraPosition(&Vector3(camPos[0], camPos[1]--, camPos[2]));
			printf("Collision Y\n");

		}
		else if (-dist < shipMax.y)
		{
			Vector3 camPos = *(worldCam.GetCameraPosition());
			worldCam.SetCameraPosition(&Vector3(camPos[0], camPos[1]++, camPos[2]));
			printf("Collision Y\n");
		}

		if ((dist = shipMin.z - otherMin.z) < otherMax.z)
		{

		}
		else if (-dist < shipMax.z)
		{

		}*/
	}
}