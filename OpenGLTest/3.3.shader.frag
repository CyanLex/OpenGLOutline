#version 330 core
in vec3 vColor;
in vec3 vBary;
in vec3 vFragPos;
in vec3 vNormal;

out vec4 FragColor;

uniform vec3 lightPos = vec3(2.0, 2.0, 2.0);
uniform vec3 lightColor = vec3(1.0, 1.0, 1.0);
uniform vec3 viewPos;
uniform vec3 objectColor;

// closer to the edge, closer is to 0
float edgeFactor()
{
    // compute how close we are to an edge
    vec3 balanceVec = fwidth(vBary);
    vec3 pixelVec = smoothstep(vec3(0.0), balanceVec * 3, vBary);
    return min(min(pixelVec.x, pixelVec.y), pixelVec.z);
}

void main()
{
    // phnog lighting
    vec3 normals = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);

    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    float diff = max(dot(normals, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectionDir = reflect(-lightDir, normals);
    float spec = pow(max(dot(viewDir, reflectionDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  

    vec3 phongResult = (ambient + diffuse + specular) * objectColor;


    float EF = edgeFactor();
    vec3 edgeColor = vec3(0.0, 1.0, 1.0); // 0,1,1 for cyan
    // chose color depending on EF
    // phongColor instead of vColor
    FragColor = vec4(mix(edgeColor, phongResult, EF), 1.0);
}
