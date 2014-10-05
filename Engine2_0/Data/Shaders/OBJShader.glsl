//layout (std140) uniform Matrices 

#ifdef VERTEX_SHADER
#version 330

layout(std140) uniform Matrices
{
	mat4 modelMatrix;
	mat4 viewMatrix;
	mat4 projMatrix;
};

in vec3 position;
in vec3 normal;
in vec2 texCoord;

out vec4 vertexPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
	Normal = normalize(vec3(viewMatrix * modelMatrix * vec4(normal, 0.0)));
	TexCoord = vec2(texCoord);
	vertexPos = projMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
	gl_Position = vertexPos;
}
#endif

#ifdef GEOMETRY_SHADER
//NOPE
#endif

#ifdef FRAGMENT_SHADER
#version 330

layout (std140) uniform Material 
{
	vec3 diffuse;
	float padding;
	
	vec3 ambient;
	float padding2;
	
	vec3 specular;
	float shininess;
};

uniform	sampler2D texUnit;

in vec2 TexCoord;
in vec3 Normal;
out vec4 outputColor;

void main()
{
	outputColor = vec4(1.0f, 0.0f, 1.0f, 1.0f) * vec4(Normal, 1.0f);
}
#endif

/*
#version 330
 


 
in vec3 Normal;
in vec2 TexCoord;
out vec4 output;
 
void main()
{
vec4 color;
vec4 amb;
float intensity;
vec3 lightDir;
vec3 n;
lightDir = normalize(vec3(1.0,1.0,1.0));
n = normalize(Normal);	
intensity = max(dot(lightDir,n),0.0);
if (texCount == 0) {
color = diffuse;
amb = ambient;
}
else {
color = texture2D(texUnit, TexCoord);
amb = color * 0.33;
}
output = (color * intensity) + amb;
//output = vec4(texCount,0.0,0.0,1.0);
 
}
*/