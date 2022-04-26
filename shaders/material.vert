#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 intexCoords;
layout (location = 3) in vec3 inColors;

out vec3 vertColor;
out vec2 texCoords;
out vec3 vertNormal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec4 vertPos = model * vec4(inPos, 1.0f);
    gl_Position = projection * view * model * vec4(inPos, 1.0f);
    vertColor = inColors;
    texCoords = intexCoords;
    vertNormal = mat3(model) * inNormal;
    FragPos = vertPos.xyz;
}
