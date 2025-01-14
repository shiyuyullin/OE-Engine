#pragma once

#include "Scene.h"

class Scene1 : public Scene
{
public:
	Scene1(Camera* canera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs);
	void render();
	void renderDepthBuffer();
	void renderOutlining();
	void drawPlane();
private:
	vector<GLuint*>* VAOs;
	glm::vec3 pointLightPositions[4];
	glm::vec3 cubePositions[10];
	glm::vec3 lightSourcePosition;
	float lightSourceMoveRange;
	float lightSourceMoveSpeed;
};

Scene1::Scene1(Camera* camera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs) : Scene(camera, shaders)
{
	this->VAOs = VAOs;

	pointLightPositions[0] = glm::vec3(0.7f, 0.2f, 2.0f);
	pointLightPositions[1] = glm::vec3(2.3f, -3.3f, -4.0f);
	pointLightPositions[2] = glm::vec3(-4.0f, 2.0f, -12.0f);
	pointLightPositions[3] = glm::vec3(0.0f, 0.0f, -3.0f);

	cubePositions[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	cubePositions[1] = glm::vec3(2.0f, 5.0f, -15.0f);
	cubePositions[2] = glm::vec3(-1.5f, -2.2f, -2.5f);
	cubePositions[3] = glm::vec3(-3.8f, -2.0f, -12.3f);
	cubePositions[4] = glm::vec3(2.4f, -0.4f, -3.5f);
	cubePositions[5] = glm::vec3(-1.7f, 3.0f, -7.5f);
	cubePositions[6] = glm::vec3(1.3f, -2.0f, -2.5f);
	cubePositions[7] = glm::vec3(1.5f, 2.0f, -2.5f);
	cubePositions[8] = glm::vec3(1.5f, 0.2f, -1.5f);
	cubePositions[9] = glm::vec3(-1.3f, 1.0f, -1.5f);
	
	lightSourcePosition = glm::vec3(7.2f, 0.0f, -5.0f);
	lightSourceMoveRange = 3.0f;
	lightSourceMoveSpeed = 1.0f;
	cout << "Scene #1 initialized" << endl;
}

void Scene1::drawPlane()
{
	Utils::drawPlane(glm::vec3(0.0, -5.0, 0.0), *VAOs->at(1), shaders->at(5), camera);
}

void Scene1::renderDepthBuffer()
{

	Shader& zBufferShader = shaders->at(3);

	glBindVertexArray(*VAOs->at(0));

	zBufferShader.use();

	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

	zBufferShader.setMat4f("view", 1, false, view);
	zBufferShader.setMat4f("projection", 1, false, projection);
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		zBufferShader.setMat4f("model", 1, false, model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Scene1::renderOutlining()
{
	float time = glfwGetTime();

	Shader& zBufferShader = shaders->at(3);
	Shader& stencilShader = shaders->at(4);

	glBindVertexArray(*VAOs->at(0));

	// only if the tests pass, we update buffer
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// all fragments will pass stencil test
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	// enable writing to stencil buffer
	glStencilMask(0xFF);
	zBufferShader.use();
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	zBufferShader.setMat4f("view", 1, false, view);
	zBufferShader.setMat4f("projection", 1, false, projection);
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		zBufferShader.setMat4f("model", 1, false, model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	// a fragment passes stencil test if its value in stencil buffer if not eqaul to 1
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	// disable writing to stencil buffer
	glStencilMask(0x00);
	glDisable(GL_DEPTH_TEST);
	stencilShader.use();
	stencilShader.setMat4f("view", 1, false, view);
	stencilShader.setMat4f("projection", 1, false, projection);
	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		model = glm::scale(model, glm::vec3(1.02f, 1.02f, 1.02f));
		stencilShader.setMat4f("model", 1, false, model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glEnable(GL_DEPTH_TEST);
}



void Scene1::render()
{
	float time = glfwGetTime();

	glBindVertexArray(*VAOs->at(0));
	
	Shader& lightShaderObj = shaders->at(0);

	lightShaderObj.use();
	// view and projection matrices are always the same no matter which shader to use
	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

	lightShaderObj.setMat4f("view", 1, false, view);
	lightShaderObj.setMat4f("projection", 1, false, projection);
	lightShaderObj.setVec3fv("cameraPos", 1, camera->Position);
	lightShaderObj.setInt("material.diffuseMap", 2);
	lightShaderObj.setInt("material.specularMap", 3);
	lightShaderObj.setFloat("material.shininess", 32.0f);
	lightShaderObj.setInt("useEmissonMap", 0);
	lightShaderObj.setInt("material.emissionMap", 4);
	// directional light
	lightShaderObj.setVec3fv("directionalLight.direction", 1, glm::vec3(-0.2f, -1.0f, -0.3f));
	lightShaderObj.setVec3fv("directionalLight.ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("directionalLight.diffuse", 1, glm::vec3(0.4f, 0.4f, 0.4f));
	lightShaderObj.setVec3fv("directionalLight.specular", 1, glm::vec3(0.5f, 0.5f, 0.5f));
	// point light 0
	lightShaderObj.setVec3fv("pointLights[0].position", 1, pointLightPositions[0]);
	lightShaderObj.setVec3fv("pointLights[0].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("pointLights[0].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	lightShaderObj.setVec3fv("pointLights[0].specular", 1, glm::vec3(1.0f));
	lightShaderObj.setFloat("pointLights[0].constant", 1.0f);
	lightShaderObj.setFloat("pointLights[0].linear", 0.09f);
	lightShaderObj.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 1
	lightShaderObj.setVec3fv("pointLights[1].position", 1, pointLightPositions[1]);
	lightShaderObj.setVec3fv("pointLights[1].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("pointLights[1].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	lightShaderObj.setVec3fv("pointLights[1].specular", 1, glm::vec3(1.0f));
	lightShaderObj.setFloat("pointLights[1].constant", 1.0f);
	lightShaderObj.setFloat("pointLights[1].linear", 0.09f);
	lightShaderObj.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 2
	lightShaderObj.setVec3fv("pointLights[2].position", 1, pointLightPositions[2]);
	lightShaderObj.setVec3fv("pointLights[2].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("pointLights[2].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	lightShaderObj.setVec3fv("pointLights[2].specular", 1, glm::vec3(1.0f));
	lightShaderObj.setFloat("pointLights[2].constant", 1.0f);
	lightShaderObj.setFloat("pointLights[2].linear", 0.09f);
	lightShaderObj.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 3 
	lightShaderObj.setVec3fv("pointLights[3].position", 1, pointLightPositions[3]);
	lightShaderObj.setVec3fv("pointLights[3].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("pointLights[3].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	lightShaderObj.setVec3fv("pointLights[3].specular", 1, glm::vec3(1.0f));
	lightShaderObj.setFloat("pointLights[3].constant", 1.0f);
	lightShaderObj.setFloat("pointLights[3].linear", 0.09f);
	lightShaderObj.setFloat("pointLights[3].quadratic", 0.032f);

	for (int i = 0; i < 10; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightShaderObj.setMat4f("model", 1, false, model);
		glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));
		lightShaderObj.setMat3f("normalMat", 1, false, normalMat);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}


	// lighting with blinn-phong
	Shader& blinnLightShaderObj = shaders->at(1);
	blinnLightShaderObj.use();
	blinnLightShaderObj.setMat4f("view", 1, false, view);
	blinnLightShaderObj.setMat4f("projection", 1, false, projection);
	blinnLightShaderObj.setVec3fv("cameraPos", 1, camera->Position);
	blinnLightShaderObj.setInt("material.diffuseMap", 5);
	blinnLightShaderObj.setInt("material.specularMap", 5);
	blinnLightShaderObj.setFloat("material.shininess", 4.0f);
	blinnLightShaderObj.setInt("useEmissonMap", 0);
	blinnLightShaderObj.setInt("material.emissionMap", 4);
	// model matrix
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(17.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 0.5f, 6.0f));
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	blinnLightShaderObj.setMat4f("model", 1, false, modelMatrix);
	blinnLightShaderObj.setMat3f("normalMat", 1, false, normalMatrix);
	// directional light
	blinnLightShaderObj.setVec3fv("directionalLight[0].direction", 1, glm::vec3(-0.2f, -1.0f, -0.3f));
	blinnLightShaderObj.setVec3fv("directionalLight[0].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	blinnLightShaderObj.setVec3fv("directionalLight[0].diffuse", 1, glm::vec3(0.4f, 0.4f, 0.4f));
	blinnLightShaderObj.setVec3fv("directionalLight[0].specular", 1, glm::vec3(0.5f, 0.5f, 0.5f));
	// point light
	blinnLightShaderObj.setVec3fv("pointLights[0].position", 1, glm::vec3(17.0f, 4.0f, 0.0f));
	blinnLightShaderObj.setVec3fv("pointLights[0].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	blinnLightShaderObj.setVec3fv("pointLights[0].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	blinnLightShaderObj.setVec3fv("pointLights[0].specular", 1, glm::vec3(1.0f));
	blinnLightShaderObj.setFloat("pointLights[0].constant", 1.0f);
	blinnLightShaderObj.setFloat("pointLights[0].linear", 0.09f);
	blinnLightShaderObj.setFloat("pointLights[0].quadratic", 0.032f);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(17.0f, 1.5f, 0.0f));
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	blinnLightShaderObj.setMat4f("model", 1, false, modelMatrix);
	blinnLightShaderObj.setMat3f("normalMat", 1, false, normalMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//emission map
	blinnLightShaderObj.setInt("material.diffuseMap", 2);
	blinnLightShaderObj.setInt("material.specularMap", 3);
	blinnLightShaderObj.setInt("useEmissonMap", 1);
	blinnLightShaderObj.setFloat("material.emissionBrightness", sin(0.85 * time));
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-5.0f, 1.5f, 0.0f));
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	blinnLightShaderObj.setMat4f("model", 1, false, modelMatrix);
	blinnLightShaderObj.setMat3f("normalMat", 1, false, normalMatrix);
	glDrawArrays(GL_TRIANGLES, 0, 36);



	lightShaderObj.use();
	lightShaderObj.setMat4f("view", 1, false, view);
	lightShaderObj.setMat4f("projection", 1, false, projection);
	lightShaderObj.setVec3fv("cameraPos", 1, camera->Position);
	lightShaderObj.setInt("material.diffuseMap", 5);
	lightShaderObj.setInt("material.specularMap", 5);
	lightShaderObj.setFloat("material.shininess", 4.0f);
	lightShaderObj.setInt("useEmissonMap", 0);
	lightShaderObj.setInt("material.emissionMap", 4);
	// model matrix
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(8.0f, 0.0f, 0.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 0.5f, 6.0f));
	normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelMatrix)));
	lightShaderObj.setMat4f("model", 1, false, modelMatrix);
	lightShaderObj.setMat3f("normalMat", 1, false, normalMatrix);
	// directional light
	lightShaderObj.setVec3fv("directionalLight.direction", 1, glm::vec3(-0.2f, -1.0f, -0.3f));
	lightShaderObj.setVec3fv("directionalLight.ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("directionalLight.diffuse", 1, glm::vec3(0.4f, 0.4f, 0.4f));
	lightShaderObj.setVec3fv("directionalLight.specular", 1, glm::vec3(0.5f, 0.5f, 0.5f));
	// point light
	lightShaderObj.setVec3fv("pointLights[0].position", 1, glm::vec3(8.0f, 1.0f, 0.0f));
	lightShaderObj.setVec3fv("pointLights[0].ambient", 1, glm::vec3(0.05f, 0.05f, 0.05f));
	lightShaderObj.setVec3fv("pointLights[0].diffuse", 1, glm::vec3(0.8f, 0.8f, 0.8f));
	lightShaderObj.setVec3fv("pointLights[0].specular", 1, glm::vec3(1.0f));
	lightShaderObj.setFloat("pointLights[0].constant", 1.0f);
	lightShaderObj.setFloat("pointLights[0].linear", 0.09f);
	lightShaderObj.setFloat("pointLights[0].quadratic", 0.032f);
	glDrawArrays(GL_TRIANGLES, 0, 36);



	// light source bouces in the range 
	lightSourcePosition.x = 6.2f + (lightSourceMoveRange * sin(lightSourceMoveSpeed * time));

	// light source 
	Shader& lightSourceShaderObj = shaders->at(2);
	lightSourceShaderObj.use();
	glm::mat4 modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, lightSourcePosition);
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.5f, 0.5f, 0.5f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, pointLightPositions[0]);
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.3f, 0.3f, 0.3f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, pointLightPositions[1]);
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.3f, 0.3f, 0.3f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, pointLightPositions[2]);
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.3f, 0.3f, 0.3f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, pointLightPositions[3]);
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.3f, 0.3f, 0.3f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, glm::vec3(8.0f, 1.0f, 0.0f));
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.3f, 0.3f, 0.3f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	modelForLightSourceShader = glm::mat4(1.0f);
	modelForLightSourceShader = glm::translate(modelForLightSourceShader, glm::vec3(17.0f, 4.0f, 0.0f));
	modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.3f, 0.3f, 0.3f));
	lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
	lightSourceShaderObj.setMat4f("view", 1, false, view);
	lightSourceShaderObj.setMat4f("projection", 1, false, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


