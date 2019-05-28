#version 430

layout (location = 0) uniform mat4 modelview;	//modelview matrix
layout (location = 1) uniform mat4 projection;	//projection matrix
layout (location = 2) uniform mat4 model;		//model matrix
layout (location = 3) uniform vec4 lightpos;	//light position
layout (location = 4) uniform vec4 camPos;		//camera position
//layout (location = 4) uniform mat4 lightmatrix;	//light matrix
//layout (location = 5) uniform mat4 lightproj;	//light projection matrix

layout (location = 0) in vec4 position;	//vertex attribute: position
layout (location = 1) in vec4 inNormal;	//vertex attribute: normal
layout (location = 2) in vec2 inUV;		//vertex attribute: texcoords

out vec4 outNormal;		//output: normal
out vec4 lightvec;		//output: light vector
out vec4 viewvec;		//output: view vector (eye?)
out vec2 outUV;			//output: texcoords
out vec4 outPosInLight;	//output: vertex position in light space

void main()
{	
	//gl_Position = projection*modelview*position; // original
	gl_Position = projection*modelview*model*position;

	//per-vertex lighting here
	//==========OLD SHIET==========
	//viewvec = -(modelview * position);
	//viewvec = normalize(modelview - position); // NEW	
	//lightvec = normalize(mat4(lightpos) - position); // NEW
	//lightvec = lightpos;
	//=============================
	
	//vec4 pos = projection * modelview * position;
	viewvec = camPos - (model * position);
	lightvec = normalize(lightpos - (model * position));

	outNormal = normalize(inNormal);
	outUV = inUV;
}
