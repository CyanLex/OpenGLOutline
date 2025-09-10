#version 330 core
in vec3 vColor;;
in vec3 vBary;

out vec4 FragColor;

// closer to the edge closer is to 0
float edgeFactor()
{
    // compute how close we are to an edge
    vec3 balanceVec = fwidth(vBary);
    vec3 pixelVec = smoothstep(vec3(0.0), balanceVec * 1.5, vBary);
    return min(min(pixelVec.x, pixelVec.y), pixelVec.z);
}

void main()
{
    float EF = edgeFactor();
    vec3 edgeColor = vec3(0.0, 1.0, 1.0); // cyan
    // chose color depending on EF
    FragColor = vec4(mix(edgeColor, vColor, EF), 1.0);
}
