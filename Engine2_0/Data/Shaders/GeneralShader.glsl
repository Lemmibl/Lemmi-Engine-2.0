#ifdef VERTEX_SHADER
#version 400

layout (location = 0) in vec3 Position;

void main()
{
	gl_Position = vec4(0.5 * Position.x, 0.5 * Position.y, Position.z, 0.5);
}

#endif

#ifdef GEOMETRY_SHADER
#version 400
void main()
{

}

#endif

#ifdef FRAGMENT_SHADER
#version 400

out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}

#endif