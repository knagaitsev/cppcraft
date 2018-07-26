#version 150 core
in vec3 position;
in vec2 texcoord;
in vec3 normal;
out vec2 Texcoord;
out vec3 Normal;
uniform mat4 view;
uniform mat4 proj;
void main()
{
    Texcoord = texcoord;
	Normal = normal;
    gl_Position = proj * view * vec4(position, 1.0);
}
