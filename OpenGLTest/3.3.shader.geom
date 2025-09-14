#version 330 core
//layout(triangles) in;
//layout(triangle_strip, max_vertices = 3) out;

//in vec3 vColor[];
//out vec3 gColor;
//noperspective out vec3 dist;

//uniform vec2 uWinScale;

// adjadency method for edge detection
layout(triangles_adjacency) in;
layout(triangle_strip, max_vertices = 6) out;

in vec3 vColor[];
out vec3 gColor;
out vec3 gBary;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// geometry shader is not used anymore
void main()
{
   for (int i = 0; i < 3; i++) {
        int idx = i * 2; // 0, 2, 4
        gColor = vColor[idx];
        gBary = vec3(0.0);
        gBary[i] = 1.0;
        gl_Position = gl_in[idx].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
