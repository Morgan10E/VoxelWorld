#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 lightSpaceMatrix;
uniform mat4 model;
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 worldRotation;
uniform mat4 worldLocation;

void main()
{
    gl_Position = lightSpaceMatrix * worldLocation * worldRotation * translation * rotation * model * vec4(position, 1.0f);
}
