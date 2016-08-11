#ifndef WORLD_TYPE_H
#define WORLD_TYPE_H

#define GLEW_STATIC
#define STB_PERLIN_IMPLEMENTATION

#include <stdlib.h>

#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#define GLM_FORCE_RADIANS // force everything in radian
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/ext.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

class WorldType {
  public:
    WorldType();
    WorldType(float maxHeight);

    float waterHeight;
    float treeLine;
    float snowLine;

    float xOffset;
    float yOffset;
    float zOffset;

    glm::vec3 waterColor;
    glm::vec3 treeColor;
    glm::vec3 landColor;
    glm::vec3 snowColor;

    float random();
};

WorldType::WorldType() {
  waterHeight = random() * 10 - 5;
}

WorldType::WorldType(float maxHeight) {
  waterHeight = random() * maxHeight - maxHeight/2;
  std::cout << "Water Height: " << waterHeight << std::endl;
  waterColor = glm::vec3(0.3f, 0.3f, 1.0f);

  float remaining = maxHeight - waterHeight;
  treeLine = random() * remaining + waterHeight;

  treeColor = glm::vec3(0.3f, 1.0f, 0.3f);
  landColor = glm::vec3(0.4f, 0.4f, 0.4f);

  remaining = maxHeight - treeLine;
  snowLine = random() * remaining + treeLine;

  snowColor = glm::vec3(1.0f, 1.0f, 1.0f);

  xOffset = random() * 100;
  yOffset = random() * 100;
  zOffset = random() * 100;
  std::cout << xOffset << ", " << yOffset << ", " << zOffset << std::endl;
}

float WorldType::random() {
  return (float)rand()/RAND_MAX;
}

#endif
