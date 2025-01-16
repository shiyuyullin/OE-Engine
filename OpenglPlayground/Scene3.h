#pragma once

#include "Scene.h"

// shadow mapping
class Scene3 : public Scene
{
public:
	Scene3(Camera* camera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs);
	void render();
private:
	GLuint depthMapFBO;
	GLuint depthMap;
	// increase the resolution of shadow map could make shadows softer
	const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
	glm::mat4 lightProjectionMat;
	glm::mat4 lightViewMat;
	glm::vec3 lightPos;
	vector<GLuint*>* VAOs;
};

Scene3::Scene3(Camera* camera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs) : Scene(camera, shaders)
{
	this->VAOs = VAOs;
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "Error::Scene3 Framebuffer not complete." << endl;
	}
	else
	{
		cout << "Scene3 Framebuffer complete." << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	lightPos = glm::vec3(0.0f, 2.0f, -1.0f);
	lightProjectionMat = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
	lightViewMat = glm::lookAt(lightPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	cout << "Scene3 initialized" << endl;
}


void Scene3::render()
{
	float time = glfwGetTime();
	// moving the light source along x-axis
	lightPos.x = -2.0f + (4.0f * sin(0.3f * time));
	lightViewMat = glm::lookAt(lightPos,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	// render to shadow map
	Shader& simpleDepthShader = shaders->at(6);
	simpleDepthShader.use();
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glBindVertexArray(*VAOs->at(0));
	glClear(GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
	simpleDepthShader.setMat4f("lightSpaceMatrix", 1, false, lightProjectionMat * lightViewMat);
	simpleDepthShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(1.0f, 0.0f, 1.5f));
	simpleDepthShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(-2.0f, 1.5f, 1.0f));
	simpleDepthShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindVertexArray(*VAOs->at(1));
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
	simpleDepthShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render scene as normal
	glViewport(0, 0, 800, 600);
	Shader& shadowMappingShader = shaders->at(7);
	//Shader& defaultShader = shaders->at(5);
	glBindVertexArray(*VAOs->at(0));
	shadowMappingShader.use();
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 viewMat = camera->GetViewMatrix();
	glm::mat4 projectionMat = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	shadowMappingShader.setMat4f("model", 1, false, modelMat);
	shadowMappingShader.setMat4f("view", 1, false, viewMat);
	shadowMappingShader.setMat4f("projection", 1, false, projectionMat);
	shadowMappingShader.setMat4f("lightSpaceMatrix", 1, false, lightProjectionMat * lightViewMat);
	shadowMappingShader.setInt("diffuseTexture", 0);
	shadowMappingShader.setInt("shadowMap", 10);
	shadowMappingShader.setVec3fv("lightPos", 1, glm::vec3(-2.0f, 2.0f, -1.0f));
	shadowMappingShader.setVec3fv("viewPos", 1, camera->Position);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(1.0f, 0.0f, 1.5f));
	shadowMappingShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(-2.0f, 1.5f, 1.0f));
	shadowMappingShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glBindVertexArray(*VAOs->at(1));
	modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f));
	shadowMappingShader.setMat4f("model", 1, false, modelMat);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}