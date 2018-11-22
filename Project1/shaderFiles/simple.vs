#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 tangent;
out vec3 worldPos;
void main()
{
   gl_Position = projection * view * model *vec4(aPos, 1.0);
   worldPos = vec3(model * vec4(aPos, 1.0));
   normal = mat3(transpose(inverse(model))) *aNormal;
   tangent =  mat3(transpose(inverse(model))) *aTangent;
}