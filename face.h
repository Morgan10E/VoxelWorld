#define GLEW_STATIC
#define STB_PERLIN_IMPLEMENTATION

#include "Shader.h"
#include "voxel.h"
#include "stb_perlin.h"

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

class Face {
  public:
    Face(Shader* shader, float x, float y, float z, int width, std::string direction, float heightMultiplier, float variability);
    void init(Shader* shader, glm::vec3 color);
    std::vector<std::vector<Voxel> > perlinFieldY(float x, float y, float z);
    // ~Face();

    void render();

    void translate(float x, float y, float z);

    Shader* shader;

  private:
    std::vector<std::vector<Voxel> > voxels;

    int width;
    std::string direction;
    float heightMultiplier, variability;
    float startX, startY, startZ;
};

Face::Face(Shader* shader, float x, float y, float z, int width, std::string direction, float heightMultiplier, float variability) {
  this->shader = shader;
  this->startX = x;
  this->startY = y;
  this->startZ = z;
  this->width = width;
  this->heightMultiplier = heightMultiplier;
  this->variability = variability;

  this->voxels = perlinFieldY(x, y, z);
}

std::vector<std::vector<Voxel> > Face::perlinFieldY(float x, float y, float z) {
  std::vector<std::vector<Voxel> > grid;
  for (int r = 0; r < this->width; r++) {
    std::vector<Voxel> row;
    grid.push_back(row);
    for (int c = 0; c < this->width; c++) {
      float height = stb_perlin_noise3((float)(x+r)/this->width * this->variability, y, (float)(z+c)/this->width * this->variability) * this->heightMultiplier;
      Voxel newVoxel(shader);
      newVoxel.translate(x+r, floorf(y+height), z+c);
      // std::cout << x+r << ", " << floorf(y+height) << ", " << z+c << std::endl;
      grid[r].push_back(newVoxel);
    }
  }

  return grid;
}


void Face::translate(float x, float y, float z) {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->translate(x, y, z);
    }
  }
}

void Face::render() {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->render();
    }
  }
}