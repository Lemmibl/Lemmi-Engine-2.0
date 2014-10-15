#ifdef VERTEX_SHADER
#version 400

layout(std140) uniform Matrices
{
	mat4 viewMatrix;
	mat4 projMatrix;
};

uniform mat4 modelMatrix;

layout(location=0) in vec3 VertexPosition;
layout(location=1) in vec3 VertexNormal;
layout(location=2) in vec2 VertexTexCoord;

out vec3 OutNormal;
out vec2 OutTexCoord;


void main()
{
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(VertexPosition.xyz, 1.0);

	OutNormal = normalize(vec3(viewMatrix * modelMatrix * vec4(VertexNormal.xyz, 0.0)));
	OutTexCoord = VertexTexCoord.xy;
}
#endif

#ifdef GEOMETRY_SHADER
//NOPE
#endif

#ifdef FRAGMENT_SHADER
#version 400

layout (std140) uniform Material 
{
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
};

uniform sampler2D diffuseTex;

in vec3 OutNormal;
in vec2 OutTexCoord;

out vec4 outputColor;

void main()
{
	//vec4 zeroVec = vec4(0.0f, 0.0f, 0.0f, 0.0f);

	outputColor = texture2D(diffuseTex, OutTexCoord);// + vec4(OutNormal, 0.0f);

	if(0.45f > outputColor.a)
	{
		discard;
	}
}
#endif