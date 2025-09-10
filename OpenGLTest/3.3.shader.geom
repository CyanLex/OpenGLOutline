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

void main()
{
// changing to barycentric coordinates so this is not used anymore
   // // Convert to screen space
    //vec2 p0 = uWinScale * (gl_in[0].gl_Position.xy / gl_in[0].gl_Position.w);
   // vec2 p1 = uWinScale * (gl_in[1].gl_Position.xy / gl_in[1].gl_Position.w);
    //vec2 p2 = uWinScale * (gl_in[2].gl_Position.xy / gl_in[2].gl_Position.w);

   // //shader.setVec2("uWinScale", 1.0f / winWidth, 1.0f / winHeight);

   // vec2 v0 = p2 - p1;
   // vec2 v1 = p2 - p0;
   // vec2 v2 = p1 - p0;
   // float area = abs(v1.x * v2.y - v1.y * v2.x);

   // // Vertex 0
   // dist = vec3(area / length(v0), 0.0, 0.0);
   // gColor = vColor[0];
   // gl_Position = gl_in[0].gl_Position;
   // EmitVertex();

   // // Vertex 1
  //  dist = vec3(0.0, area / length(v1), 0.0);
  //  gColor = vColor[1];
   // gl_Position = gl_in[1].gl_Position;
  //  EmitVertex();

   // // Vertex 2
   // dist = vec3(0.0, 0.0, area / length(v2));
   // gColor = vColor[2];
  //  gl_Position = gl_in[2].gl_Position;
  //  EmitVertex();

  //  EndPrimitive();

   // vec3 p0 = vec3(gl_in[0].gl_Position);
   // vec3 p1 = vec3(gl_in[2].gl_Position);
    //vec3 p2 = vec3(gl_in[4].gl_Position);

    // vec3 n = normalize(cross(p1 - p0, p2 - p0));

   // // facing direction
   // vec3 eyeDir = normalize(-p0); 

   // // only draw if facing camera
    //if (dot(n, eyeDir) > 0.0) {
     //   for (int i = 0; i < 3; ++i) {
    //        int idx = i * 2; // main triangle verts: 0,2,4
   //         gColor = vColor[idx];
   //         gBary = (i == 0) ? vec3(1,0,0) :(i == 1) ? vec3(0,1,0) :vec3(0,0,1);
   //         gl_Position = gl_in[idx].gl_Position;
    //        EmitVertex();
   //     }
    //    EndPrimitive();
   // }

   // no real edge detection
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
