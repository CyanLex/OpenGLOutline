#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aBary;
layout(location = 3) in vec3 aNormal;

out vec3 vColor;
out vec3 vBary;
//out vec3 vViewPos;
out vec3 vFragPos;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vColor = aColor;
    vBary = aBary;
    
    vec4 worldPos = model * vec4(aPos,1.0);
    vFragPos = vec3(model * vec4(aPos,1.0));

    //vViewPos = viewPos.xyz;
    vNormal = mat3(transpose(inverse(model))) * aNormal;

    gl_Position = projection * view * vec4(vFragPos,1.0);
}