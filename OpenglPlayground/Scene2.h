#pragma once

#include "Scene.h"

class Scene2 : public Scene
{
public:
	Scene2();
	Scene2(Camera* canera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs);
	~Scene2();
	void render();
private:
	vector<GLuint*>* VAOs;

};


Scene2::Scene2()
{

}

Scene2::Scene2(Camera* canera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs) : Scene(canera, shaders)
{
	this->VAOs = VAOs;
}

Scene2::~Scene2()
{

}

void Scene2::render()
{
	/*unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	float near_plane = 1.0f, far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(0.0f, 3.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;




	glBindVertexArray(*VAOs->at(0));

	/*Shader& depthMapShaders = shaders->at(1);
	depthMapShaders.setMat4f("lightSpaceMatrix", 1, false, lightSpaceMatrix);
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 0.5f, 6.0f));
	depthMapShaders.setMat4f("model", 1, false, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
	depthMapShaders.setMat4f("model", 1, false, modelMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/



	Shader& shadowShaders = shaders->at(0);

	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

	shadowShaders.use();
	shadowShaders.setMat4f("view", 1, false, view);
	shadowShaders.setMat4f("projection", 1, false, projection);
	shadowShaders.setMat4f("lightSpaceMat", 1, false, lightSpaceMatrix);
	shadowShaders.setVec3fv("cameraPos", 1, camera->Position);
	shadowShaders.setInt("material.diffuseMap", 5);
	shadowShaders.setInt("material.specularMap", 5);
	shadowShaders.setFloat("material.shininess", 4.0f);
	shadowShaders.setInt("useEmissonMap", 0);
	shadowShaders.setInt("material.emissionMap", 4);
	// model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 0.5f, 6.0f));
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	shadowShaders.setMat4f("model", 1, false, modelMatrix);
	shadowShaders.setMat3f("normalMat", 1, false, normalMatrix);
	// directional light
	shadowShaders.setVec3fv("directionalLight[0].direction", 1, glm::vec3(-0.2f, -1.0f, -0.3f));
	shadowShaders.setVec3fv("directionalLight[0].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	shadowShaders.setVec3fv("directionalLight[0].diffuse", 1, glm::vec3(0.4f, 0.4f, 0.4f));
	shadowShaders.setVec3fv("directionalLight[0].specular", 1, glm::vec3(0.5f, 0.5f, 0.5f));
	// point light
	shadowShaders.setVec3fv("pointLights[0].position", 1, glm::vec3(0.0f, 4.0f, 0.0f));
	shadowShaders.setVec3fv("pointLights[0].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	shadowShaders.setVec3fv("pointLights[0].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	shadowShaders.setVec3fv("pointLights[0].specular", 1, glm::vec3(1.0f));
	shadowShaders.setFloat("pointLights[0].constant", 1.0f);
	shadowShaders.setFloat("pointLights[0].linear", 0.09f);
	shadowShaders.setFloat("pointLights[0].quadratic", 0.032f);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	shadowShaders.setMat4f("model", 1, false, modelMatrix);
	shadowShaders.setMat3f("normalMat", 1, false, normalMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}