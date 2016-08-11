#ifndef FACE_H
#define FACE_H

#define GLEW_STATIC
#define STB_PERLIN_IMPLEMENTATION

#include "Shader.h"
#include "voxel.h"
#include "stb_perlin.h"
#include "world_type.h"

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
    Face(Shader* shader, Shader* shadowShader, GLuint depthMap, GLuint depthMapFBO, glm::mat4 lightSpaceMatrix, float x, float y, float z, int width, std::string direction, float heightMultiplier, float variability, WorldType worldType);
    void init(Shader* shader, Shader* shadowShader, GLuint depthMap, GLuint depthMapFBO, glm::vec3 color);
    std::vector<std::vector<Voxel> > perlinFieldX(float x, float y, float z, int direction);
    std::vector<std::vector<Voxel> > perlinFieldY(float x, float y, float z, int direction);
    std::vector<std::vector<Voxel> > perlinFieldZ(float x, float y, float z, int direction);
    // ~Face();

    void render();
    void renderReal(int w, int h);
    void renderShadow();

    void translate(float x, float y, float z);
    void rotate(glm::vec3 axis, float theta);

    Shader* shader;
    Shader* shadowShader;

    GLuint depthMap, depthMapFBO;

  private:
    std::vector<std::vector<Voxel> > voxels;

    WorldType worldType;

    int width;
    std::string direction;
    float heightMultiplier, variability;
    float startX, startY, startZ;

    glm::mat4 lightSpaceMatrix;
};

Face::Face(Shader* shader, Shader* shadowShader, GLuint depthMap, GLuint depthMapFBO, glm::mat4 lightSpaceMatrix, float x, float y, float z, int width, std::string direction, float heightMultiplier, float variability, WorldType worldType) {
  this->shader = shader;
  this->shadowShader = shadowShader;
  this->depthMap = depthMap;
  this->depthMapFBO = depthMapFBO;
  this->lightSpaceMatrix = lightSpaceMatrix;
  this->startX = x;
  this->startY = y;
  this->startZ = z;
  this->width = width;
  this->heightMultiplier = heightMultiplier;
  this->variability = variability;
  this->worldType = worldType;

  if (direction == "NY") {
    this->voxels = perlinFieldY(x, y, z, -1);
  } else if (direction == "PY") {
    this->voxels = perlinFieldY(x, y, z, 1);
  } else if (direction == "NZ") {
    this->voxels = perlinFieldZ(x, y, z, -1);
  } else if (direction == "PZ") {
    this->voxels = perlinFieldZ(x, y, z, 1);
  } else if (direction == "NX") {
    this->voxels = perlinFieldX(x, y, z, -1);
  } else if (direction == "PX") {
    this->voxels = perlinFieldX(x, y, z, 1);
  }
}

std::vector<std::vector<Voxel> > Face::perlinFieldY(float x, float y, float z, int direction) {
  std::vector<std::vector<Voxel> > grid;
  for (int r = 0; r < this->width; r++) {
    std::vector<Voxel> row;
    grid.push_back(row);
    for (int c = 0; c < this->width; c++) {
      float height = 1 + stb_perlin_noise3((float)(worldType.xOffset+x+r)/this->width * this->variability, (worldType.yOffset+y)/this->width * this->variability, (float)(worldType.zOffset+z+c)/this->width * this->variability) * this->heightMultiplier * (this->width/2 - abs(this->width/2 - c)) / (this->width/2) * (this->width/2 - abs(this->width/2 - r)) / (this->width/2);
      glm::vec3 color(1.0f, 0.3f, 0.3f);
      if (height < worldType.waterHeight) {
        color = worldType.waterColor;
        if (worldType.waterHeight > 0)
          height = 0;
        else
          height = worldType.waterHeight * (this->width/2 - abs(this->width/2 - c)) / (this->width/2) * (this->width/2 - abs(this->width/2 - r)) / (this->width/2);
      } else if (height < worldType.treeLine) {
        color = worldType.treeColor;
      } else if (height < worldType.snowLine) {
        color = worldType.landColor;
      } else {
        color = worldType.snowColor;
      }
      Voxel newVoxel(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, color);

      if (floorf(height) > 0) {
        newVoxel.translate(x+r, floorf(y+direction*height/2), z+c);
        newVoxel.scale(1, floorf(height), 1);
      } else {
        newVoxel.translate(x+r, floorf(y+direction*height), z+c);
      }
      // std::cout << x+r << ", " << floorf(y+height) << ", " << z+c << std::endl;
      grid[r].push_back(newVoxel);
    }
  }

  return grid;
}

std::vector<std::vector<Voxel> > Face::perlinFieldX(float x, float y, float z, int direction) {
  std::vector<std::vector<Voxel> > grid;
  for (int r = 0; r < this->width; r++) {
    std::vector<Voxel> row;
    grid.push_back(row);
    for (int c = 0; c < this->width; c++) {
      float height = stb_perlin_noise3((worldType.xOffset+x)/this->width * this->variability, (float)(worldType.yOffset+y+r)/this->width * this->variability, (float)(worldType.zOffset+z+c)/this->width * this->variability) * this->heightMultiplier * (this->width/2 - abs(this->width/2 - c)) / (this->width/2) * (this->width/2 - abs(this->width/2 - r)) / (this->width/2);
      glm::vec3 color(1.0f, 0.3f, 0.3f);
      if (height < worldType.waterHeight) {
        color = worldType.waterColor;
        if (worldType.waterHeight > 0)
          height = 0;
        else
          height = worldType.waterHeight * (this->width/2 - abs(this->width/2 - c)) / (this->width/2) * (this->width/2 - abs(this->width/2 - r)) / (this->width/2);
      } else if (height < worldType.treeLine) {
        color = worldType.treeColor;
      } else if (height < worldType.snowLine) {
        color = worldType.landColor;
      } else {
        color = worldType.snowColor;
      }
      Voxel newVoxel(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, color);

      if (floorf(height) > 0) {
        newVoxel.translate(floorf(x+direction*height/2), y+r, z+c);
        newVoxel.scale(floorf(height), 1, 1);
      } else {
        newVoxel.translate(floorf(x+direction*height), y+r, z+c);
      }
      // std::cout << x+r << ", " << floorf(y+height) << ", " << z+c << std::endl;
      grid[r].push_back(newVoxel);
    }
  }

  return grid;
}

std::vector<std::vector<Voxel> > Face::perlinFieldZ(float x, float y, float z, int direction) {
  std::vector<std::vector<Voxel> > grid;
  for (int r = 0; r < this->width; r++) {
    std::vector<Voxel> row;
    grid.push_back(row);
    for (int c = 0; c < this->width; c++) {
      float height = stb_perlin_noise3((float)(worldType.xOffset+x+r)/this->width * this->variability, (float)(worldType.yOffset+y+c)/this->width * this->variability, (worldType.zOffset+z)/this->width * this->variability) * this->heightMultiplier * (this->width/2 - abs(this->width/2 - c)) / (this->width/2) * (this->width/2 - abs(this->width/2 - r)) / (this->width/2);
      glm::vec3 color(1.0f, 0.3f, 0.3f);
      if (height < worldType.waterHeight) {
        color = worldType.waterColor;
        if (worldType.waterHeight > 0)
          height = 0;
        else
          height = worldType.waterHeight * (this->width/2 - abs(this->width/2 - c)) / (this->width/2) * (this->width/2 - abs(this->width/2 - r)) / (this->width/2);
      } else if (height < worldType.treeLine) {
        color = worldType.treeColor;
      } else if (height < worldType.snowLine) {
        color = worldType.landColor;
      } else {
        color = worldType.snowColor;
      }
      Voxel newVoxel(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, color);

      if (floorf(height) > 0) {
        newVoxel.translate(x+r, y+c, floorf(z+direction*height/2));
        newVoxel.scale(1, 1, floorf(height));
      } else {
        newVoxel.translate(x+r, y+c, floorf(z+direction*height));
      }
      // std::cout << x+r << ", " << floorf(y+height) << ", " << z+c << std::endl;
      grid[r].push_back(newVoxel);
      // if ((c == 0 || c == this->width - 1) && floorf(height) > 1) {
      //
      // }
    }
  }

  // void fillBackZ(float x, float y, float z, int nSteps, int direction, std::vector<Voxel>& row) {
  //   for (int i = 1; i < nSteps; i++) {
  //     Voxel newVoxel(shader);
  //     newVoxel.translate(x, y, floorf(z+direction*i));
  //     row.push_back(newVoxel);
  //   }
  // }

  return grid;
}


void Face::translate(float x, float y, float z) {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->translate(x, y, z);
    }
  }
}

void Face::rotate(glm::vec3 axis, float theta) {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->rotate(axis, theta);
    }
  }
}

void Face::renderReal(int w, int h) {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->renderReal(w,h);
    }
  }
}

void Face::renderShadow() {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->renderShadow();
    }
  }
}

void Face::render() {
  for (std::vector<std::vector<Voxel> >::iterator rit = voxels.begin(); rit != voxels.end(); ++rit) {
    for (std::vector<Voxel>::iterator cit = (*rit).begin(); cit != (*rit).end(); ++cit) {
      cit->render(shader);
    }
  }
}

#endif
