#version 430

layout (binding = 0) uniform sampler2D texColour;
layout (binding = 1) uniform sampler2D texNormal;
layout (binding = 2) uniform sampler2D texSpec;
layout (binding = 3) uniform sampler2D texEnv;
layout (binding = 4) uniform sampler2D shadowmap;
uniform vec4 ambient;
uniform vec4 specular;
uniform float shininess;

in vec4 outNormal;	//input: normal
in vec4 lightvec;	//input: light vector
in vec4 viewvec;	//input: view vector
in vec2 outUV;		//input: texcoords
in vec4 outPosInLight;	//input: position in light space

layout (location = 0) out vec4 outFrag;

void main()
{
	//vec4 white = vec4(1.0,1.0, 1.0, 1.0);

	//vec4 spec = vec4(0.0); // Specular, initialised as black

	//vec3 n = normalize(vec3(outNormal)); // Normalised vector
	//vec3 e = normalize(vec3(viewvec)); // Normalised view vector

	//float intensity = max(dot(n, vec3(lightvec)), 0.0); 

	//if (intensity > 0.0)
	//{
	//	vec3 h = normalize(vec3(lightvec) + e);
	//	float inSpec = max(dot(h, n), 0.0);
	//	spec = specular * pow(inSpec, shininess);
	//}

	//vec4 tColour = texture(texColour, outUV);
	//vec4 colourOut = max(intensity * tColour + spec, ambient);
	//vec4 colourOut = max(tColour + spec, ambient);
	//vec4 colourOut = tColour + spec;
	//vec4 colourOut = ambient;

	//outFrag = colourOut;
	//outFrag = ambient;

	// original
	//outFrag = texture(texColour, outUV);
	//outFrag = vec4(intensity, intensity, intensity, 1.0);
	//outFrag = intensity * tColour + spec + ambient;


	//====================New code====================
	vec4 halfVec = normalize(lightvec + viewvec);
	float intensity = max(dot(outNormal, lightvec), 0.0);
	
	float dotProdHalf = max(dot(halfVec, outNormal), 0.0);
	vec4 specularOut = specular * pow(dotProdHalf, shininess);
	
	vec4 tColour = texture(texColour, outUV);
	vec4 diffuseOut = tColour * intensity;
	vec4 ambientOut = tColour * ambient;
	//specularOut = specularOut * tColour;

	outFrag = ambientOut + diffuseOut + specularOut;
	//outFrag = outNormal;

	//outFrag = max((intensity * tColour + specularOut), ambient);
	//outFrag = outNormal;
	//outFrag = lightvec;
}