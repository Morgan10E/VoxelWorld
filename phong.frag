// CS148 Summer 2016 Homework 3 - Shaders

#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec3 objectColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    // TODO: Replace with your code...
    // If gl_Position was set correctly, this gives a totally red cube
    // vec4 red = vec4(vec3(1.f,0.f,0.f), 1.0f);


    float shininess = 50.0f;
    float ambientK = 0.1f;
    float diffuseK = 1.0f;
    float specularK = 0.5f;

    vec3 lightVec = normalize(lightPos - FragPos);
    vec3 reflectedLightVec = reflect(-lightVec, Normal);
    vec3 viewVec = normalize(viewPos - FragPos);

    vec4 ambient = ambientK * vec4(lightColor, 1.0f);
    float diffuseVal = max(0.0, dot(lightVec, Normal));
    vec4 diffuse = diffuseK * diffuseVal * vec4(lightColor, 1.0f);
    vec4 specular = vec4(lightColor, 1.0f) * pow(max(0.0, dot(reflectedLightVec, viewVec)), shininess);
    color = (ambient + diffuse + specular) * vec4(objectColor, 1.0f);
    // color = vec4(objectColor, 1.0f);
}
