#define GLEW_STATIC
#define STB_PERLIN_IMPLEMENTATION

#include "Shader.h"
#include "face.h"
// #include "stb_perlin.h"

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

class World {
  public:
    World(Shader* shader, float x, float y, float z, int width, float heightMultiplier, float variability);
    void init(Shader* shader, glm::vec3 color);
    // ~Face();

    void render();

    void translate(float x, float y, float z);

    Shader* shader;

  private:
    std::vector<Face> faces;

    int width;
    float heightMultiplier, variability;
    float startX, startY, startZ;
};

World::World(Shader* shader, float x, float y, float z, int width, float heightMultiplier, float variability) {
  this->shader = shader;
  this->startX = x;
  this->startY = y;
  this->startZ = z;
  this->width = width;
  this->heightMultiplier = heightMultiplier;
  this->variability = variability;

  Face face1(shader, x,y,z, width, "NZ", heightMultiplier, variability);
  this->faces.push_back(face1);
  Face face2(shader, x,y,z, width, "NY", heightMultiplier, variability);
  this->faces.push_back(face2);
  Face face3(shader, y,y,z+width, width, "PZ", heightMultiplier, variability);
  this->faces.push_back(face3);
  Face face4(shader, x,y+width,z, width, "PY", heightMultiplier, variability);
  this->faces.push_back(face4);
  Face face5(shader, x,y,z, width, "NX", heightMultiplier, variability);
  this->faces.push_back(face5);
  Face face6(shader, x+width,y,z, width, "PX", heightMultiplier, variability);
  this->faces.push_back(face6);
}

void World::translate(float x, float y, float z) {
  for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
      fit->translate(x, y, z);
  }
}

void World::render() {
  for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
      fit->render();
  }
}
