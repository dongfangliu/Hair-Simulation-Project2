#version 330 core
in vec3 normal;
in vec3 tangent;
in vec3 worldPos;

out vec4 FragColor;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
float StrandSpecular(vec3 T, vec3 V, vec3 L, float exponent)
{
    vec3 H = normalize(L + V);
    float dotTH = dot(T, H);
    float sinTH = sqrt(1.0 - dotTH*dotTH);
    float dirAtten = smoothstep(-1.0, 0.0, dot(T, H));
    return dirAtten * pow(sinTH, exponent);
}

void main()
{
  vec3 diffuseColor = vec3(0.0f,1.0f,0.0f);
  vec3 specularColor = vec3(1.0f,0.0f,0.0f);

  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - worldPos);
  vec3 diffuse =  max(dot(norm, lightDir), 0.0) *diffuseColor;

  
  vec3 viewDir = normalize(viewPos - worldPos);
  vec3 specular =StrandSpecular(normalize(tangent),viewDir,lightDir,128)*specularColor;
  
  FragColor =vec4((diffuse+specular)* lightColor, 1.0f);
}