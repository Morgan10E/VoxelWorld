// CS148 Summer 2016 Homework 3 - Shaders

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 Normal;
out vec3 FragPos;
out vec3 objectColor;

uniform mat4 model;
uniform mat4 rotation;
uniform mat4 translation;
uniform mat4 worldRotation;
uniform mat4 worldLocation;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 voxelColor;

void main()
{
    // TODO: Your code here
    // Remember to set gl_Position (correctly) or you will get a black screen...

    gl_Position = projection * view * worldLocation * worldRotation * translation * rotation * model * vec4(position, 1.0);
    Normal = normalize(vec3(worldRotation * vec4(normal, 1.0f)));
    FragPos = (model * vec4(position, 1.0f)).rgb;

    objectColor = voxelColor;
}
