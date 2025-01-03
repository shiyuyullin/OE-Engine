#pragma once

#include <vector>
#include <string>


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Scene.h"
#include "Scene1.h"


class Utils {
public:
	// draw a plane at position, expecting a VAO
	static void drawPlane(const glm::vec3& position, const GLuint& planeVAO, Shader& shader,  Camera* camera);
	// draw skybox
	static void drawSkybox(unsigned int textureIndex, const GLuint& skyboxVAO, Shader& shader, Camera* camera);
};


void Utils::drawPlane(const glm::vec3& position, const GLuint& planeVAO, Shader& shader, Camera* camera)
{
	glBindVertexArray(planeVAO);
	shader.use();
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.setMat4f("view", 1, false, view);
	shader.setMat4f("projection", 1, false, projection);

	shader.setInt("texture1", 5);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, -1.5, 0.0));
	shader.setMat4f("model", 1, false, model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Utils::drawSkybox(unsigned int textureIndex, const GLuint& skyboxVAO, Shader& shader, Camera* camera)
{
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	shader.use();
	glBindVertexArray(skyboxVAO);
	shader.setInt("skybox", textureIndex);
	glm::mat4 view = glm::mat3(camera->GetViewMatrix());
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	shader.setMat4f("projection", 1, false, projection);
	shader.setMat4f("view", 1, false, view);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LESS);
}

extern vector<string> faces
{
	"texture/skybox/right.jpg",
	"texture/skybox/left.jpg",
	"texture/skybox/top.jpg",
	"texture/skybox/bottom.jpg",
	"texture/skybox/front.jpg",
	"texture/skybox/back.jpg"
};