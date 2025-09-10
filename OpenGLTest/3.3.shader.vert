#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout (location = 2) in vec3 aBary;

out vec3 vColor;
out vec3 vBary;
out vec3 vViewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vColor = aColor;
    vBary = aBary;
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    vViewPos = viewPos.xyz;
    gl_Position = projection * viewPos;
}