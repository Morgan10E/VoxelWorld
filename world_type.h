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

    glm::vec3 waterColorR();
    glm::vec3 treeColorR();
    glm::vec3 landColorR();

    glm::vec3 startWaterColor();
    glm::vec3 startTreeColor();
    glm::vec3 startLandColor();

    float random();
};

WorldType::WorldType() {
  waterHeight = random() * 10 - 5;
}

WorldType::WorldType(float maxHeight) {
  waterHeight = random() * maxHeight/2 - maxHeight/4;
  std::cout << "Water Height: " << waterHeight << std::endl;
  waterColor = startWaterColor();

  float remaining = maxHeight - waterHeight;
  treeLine = random() * remaining * 3 / 4 + waterHeight;

  treeColor = startTreeColor();

  landColor = startLandColor();

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

glm::vec3 WorldType::waterColorR() {
  return glm::vec3(waterColor.r + random() * 0.1f - 0.05f, waterColor.g + random() * 0.1f - 0.05f, waterColor.b + random() * 0.1f - 0.05f);
}

glm::vec3 WorldType::landColorR() {
  return glm::vec3(landColor.r + random() * 0.1f - 0.05f, landColor.g + random() * 0.1f - 0.05f, landColor.b + random() * 0.1f - 0.05f);
}

glm::vec3 WorldType::treeColorR() {
  return glm::vec3(treeColor.r + random() * 0.1f - 0.05f,  treeColor.g + random() * 0.1f - 0.05f, treeColor.b + random() * 0.1f - 0.05f);
}

glm::vec3 WorldType::startWaterColor() {
  std::vector<glm::vec3> colors = {glm::vec3(0.3f, 0.8f, 1.0f), glm::vec3(0.3f, 0.3f, 1.0f), glm::vec3(0.1f, 0.1f, 0.5f)};
  return colors[rand() % colors.size()];
}

glm::vec3 WorldType::startTreeColor() {
  std::vector<glm::vec3> colors = {glm::vec3(0.3f, 1.0f, 0.3f), glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(0.8f, 1.0f, 0.1f)};
  return colors[rand() % colors.size()];
}

glm::vec3 WorldType::startLandColor() {
  std::vector<glm::vec3> colors = {glm::vec3(0.4f, 0.4f, 0.4f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.3f, 0.3f)};
  return colors[rand() % colors.size()];
}

#endif
