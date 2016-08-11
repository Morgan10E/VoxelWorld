#ifndef WORLD_H
#define WORLD_H

#define GLEW_STATIC
#define STB_PERLIN_IMPLEMENTATION

#include "Shader.h"
#include "face.h"
#include "world_type.h"
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
    World(Shader* shader, Shader* shadowShader, GLuint depthMap, GLuint depthMapFBO, glm::mat4 lightSpaceMatrix, float x, float y, float z, int width, float heightMultiplier, float variability);
    void init(Shader* shader, Shader* shadowShader, GLuint depthMap, GLuint depthMapFBO, glm::mat4 lightSpaceMatrix, glm::vec3 color);
    // ~Face();

    void render();
    void renderReal(int w, int h);
    void renderShadow();

    void translate(float x, float y, float z);
    void rotate(glm::vec3 axis, float theta);

    Shader* shader;

  private:
    std::vector<Face> faces;
    WorldType worldType;

    int width;
    float heightMultiplier, variability;
    float startX, startY, startZ;

    glm::mat4 location, rotation;
    GLint worldLoc, worldRot, worldShadLoc, worldShadRot;
    GLuint depthMapFBO, depthMap;

    glm::mat4 lightSpaceMatrix;
};

World::World(Shader* shader, Shader* shadowShader, GLuint depthMap, GLuint depthMapFBO, glm::mat4 lightSpaceMatrix, float x, float y, float z, int width, float heightMultiplier, float variability) {
  this->shader = shader;
  this->startX = x - width/2;
  this->startY = y - width/2;
  this->startZ = z - width/2;
  this->width = width;
  this->heightMultiplier = heightMultiplier;
  this->variability = variability;
  this->lightSpaceMatrix = lightSpaceMatrix;
  this->depthMap = depthMap;
  this->depthMapFBO = depthMapFBO;
  WorldType type(heightMultiplier);
  this->worldType = type;

  location = glm::mat4(1.0f);
  location = glm::translate(location, glm::vec3(startX, startY, startZ));

  rotation = glm::mat4(1.0f);

  worldLoc = glGetUniformLocation(shader->Program, "worldLocation");
  worldRot = glGetUniformLocation(shader->Program, "worldRotation");
  worldShadLoc = glGetUniformLocation(shadowShader->Program, "worldLocation");
  worldShadRot = glGetUniformLocation(shadowShader->Program, "worldRotation");

  Face face1(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, startX,startY,startZ, width, "NZ", heightMultiplier, variability, worldType);
  this->faces.push_back(face1);
  Face face2(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, startX,startY,startZ, width, "NY", heightMultiplier, variability, worldType);
  this->faces.push_back(face2);
  Face face3(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, startX,startY,startZ+width, width, "PZ", heightMultiplier, variability, worldType);
  this->faces.push_back(face3);
  Face face4(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, startX,startY+width,startZ, width, "PY", heightMultiplier, variability, worldType);
  this->faces.push_back(face4);
  Face face5(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, startX,startY,startZ, width, "NX", heightMultiplier, variability, worldType);
  this->faces.push_back(face5);
  Face face6(shader, shadowShader, depthMap, depthMapFBO, lightSpaceMatrix, startX+width,startY,startZ, width, "PX", heightMultiplier, variability, worldType);
  this->faces.push_back(face6);
}

void World::translate(float x, float y, float z) {
  location = glm::translate(location, glm::vec3(x,y,z));
  // for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
  //     fit->translate(x, y, z);
  // }
}

void World::rotate(glm::vec3 axis, float theta) {
  // for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
  //     fit->rotate(axis, theta);
  // }
  rotation = glm::rotate(rotation, theta, axis);
}

void World::render() {
  glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(location));
  glUniformMatrix4fv(worldRot, 1, GL_FALSE, glm::value_ptr(rotation));
  for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
      fit->render();
  }
}

void World::renderReal(int w, int h) {
  glViewport(0, 0, w, h);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(location));
  glUniformMatrix4fv(worldRot, 1, GL_FALSE, glm::value_ptr(rotation));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depthMap);

  for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
      fit->renderReal(w,h);
  }
}

void World::renderShadow() {
  // glCullFace(GL_FRONT);
  glViewport(0, 0, 1024, 1024);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(worldShadLoc, 1, GL_FALSE, glm::value_ptr(location));
    glUniformMatrix4fv(worldShadRot, 1, GL_FALSE, glm::value_ptr(rotation));
    for (std::vector<Face>::iterator fit = faces.begin(); fit != faces.end(); ++fit) {
        fit->renderShadow();
    }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // glCullFace(GL_BACK);
}

#endif
