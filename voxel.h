#define GLEW_STATIC

#include "Shader.h"

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

class Voxel {
  public:
    Voxel(Shader* shader);
    Voxel(Shader* shader, glm::vec3 color);
    void init(Shader* shader, glm::vec3 color);
    // ~Voxel();

    void render();

    void translate(float x, float y, float z);
    void rotate(glm::vec3 axis, float theta);

    glm::vec3 color;

    Shader* shader;

    glm::mat4 model, rotation, translation;

    // strint type

  private:
    GLuint voxelCornersAO;

    GLint modelLoc, transLoc, rotLoc, viewLoc, projectionLoc, voxelColorLoc;
};

Voxel::Voxel(Shader* shader, glm::vec3 color) {
  this->init(shader, color);
}

Voxel::Voxel(Shader* shader) {
  this->init(shader, glm::vec3(1.0f, 0.2f, 0.2f));
}

void Voxel::init(Shader* shader, glm::vec3 color) {
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  this->model = glm::mat4(1.0f);
  this->rotation = glm::mat4(1.0f);
  this->translation = glm::mat4(1.0f);
  this->color = color;
  this->shader = shader;
  // this->type = "NONE";

  modelLoc = glGetUniformLocation(shader->Program, "model");
  rotLoc = glGetUniformLocation(shader->Program, "rotation");
  transLoc = glGetUniformLocation(shader->Program, "translation");
	projectionLoc = glGetUniformLocation(shader->Program, "projection");
  voxelColorLoc = glGetUniformLocation(shader->Program, "voxelColor");

  glGenVertexArrays(1, &voxelCornersAO);
  glBindVertexArray(voxelCornersAO);

  GLuint voxelCornersB;
  glGenBuffers(1, &voxelCornersB);
  glBindBuffer(GL_ARRAY_BUFFER, voxelCornersB);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(0);
}

void Voxel::render() {
  shader->Use();

  glBindVertexArray(voxelCornersAO);

  glUniform3f(voxelColorLoc, this->color[0], this->color[1], this->color[2]);
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(rotLoc, 1, GL_FALSE, glm::value_ptr(rotation));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, glm::value_ptr(translation));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  glBindVertexArray(0);
}

void Voxel::translate(float x, float y, float z) {
  glm::vec3 translation = glm::vec3(x, y, z);
  this->translation = glm::translate(this->translation, translation);
}

void Voxel::rotate(glm::vec3 axis, float theta) {
  this->rotation = glm::rotate(this->rotation, theta, axis);
}
