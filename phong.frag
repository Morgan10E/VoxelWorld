// CS148 Summer 2016 Homework 3 - Shaders

#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec3 objectColor;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    if (closestDepth + 0.05 < projCoords.z) return 1.0;

    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // Check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF

    // float shadow = 0.0;
    // vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    // for(int x = -1; x <= 1; ++x)
    // {
    //     for(int y = -1; y <= 1; ++y)
    //     {
    //         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
    //         shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
    //     }
    // }
    // shadow /= 9.0;

    float pcfDepth = texture(shadowMap, projCoords.xy).r;
    float shadow = currentDepth - bias > pcfDepth ? 1.0 : 0.0;

    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main()
{
    // TODO: Replace with your code...
    // If gl_Position was set correctly, this gives a totally red cube
    // vec4 red = vec4(vec3(1.f,0.f,0.f), 1.0f);


    float shininess = 10.0f;
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

    float shadow = ShadowCalculation(FragPosLightSpace);
    shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions

    color = (ambient + (1.0 - shadow) * (diffuse + specular)) * vec4(objectColor, 1.0f);


    // vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    // projCoords = projCoords * 0.5 + 0.5;
    // float closestDepth = texture(shadowMap, projCoords.xy).r;
    // // if (closestDepth == 0) closestDepth == 1;
    // color = vec4(vec3(closestDepth), 1.0f);
    // color = vec4(objectColor, 1.0f);
}
