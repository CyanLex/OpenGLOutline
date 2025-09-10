#version 330 core
in vec3 gColor;;
in vec3 gBary;

out vec4 FragColor;

// closer to the edge closer is to 0
float edgeFactor()
{
    // compute how close we are to an edge
    vec3 balanceVec = fwidth(gBary);
    vec3 pixelVec = smoothstep(vec3(0.0), balanceVec * 1.5, gBary);
    return min(min(pixelVec.x, pixelVec.y), pixelVec.z);
}

void main()
{
    float EF = edgeFactor();
    vec3 edgeColor = vec3(0.0, 1.0, 1.0); // cyan
    // chose color depending on EF
    FragColor = vec4(mix(edgeColor, gColor, EF), 1.0);
}
