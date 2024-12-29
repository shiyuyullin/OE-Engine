#pragma once

#include "Scene.h"


class Scene2 : public Scene
{
public:
	Scene2();
	Scene2(Camera* camera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs);
	~Scene2();
	void drawPlane();
	void render();


private:
	vector<GLuint*>* VAOs;
	glm::vec3 transparentObjPos[2];
};


Scene2::Scene2()
{

}

Scene2::Scene2(Camera* camera, vector<reference_wrapper<Shader>>* shaders, vector<GLuint*>* VAOs) : Scene(camera, shaders)
{
	this->VAOs = VAOs;
	// remove artifacts around the border of the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// setting F source and F destination, in this case, F source = alpha of source obj, F destination = 1 - alpha of source obj
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// setting position for transparent objects
	transparentObjPos[0] = glm::vec3(0.0, -1.0, 0.0);
	transparentObjPos[1] = glm::vec3(0.0, -1.0, 1.0);
	cout << "Scene #2 initialized" << endl;
}

Scene2::~Scene2()
{

}

void Scene2::drawPlane()
{
	Utils::drawPlane(glm::vec3(0.0, -1.5, 0.0), *VAOs->at(1), shaders->at(5), camera);
}

void Scene2::render()
{
	

	Shader& defaultShader = shaders->at(5);
	defaultShader.use();

	glm::mat4 view;
	view = camera->GetViewMatrix();
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
	defaultShader.setMat4f("view", 1, false, view);
	defaultShader.setMat4f("projection", 1, false, projection);

	// rendering laugh cube
	glBindVertexArray(*VAOs->at(0));
	defaultShader.setInt("texture1", 1);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -1.5f, -1.0f));
	defaultShader.setMat4f("model", 1, false, model);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);

	// calculate distance from camera to transparent objects
	std::map<float, glm::vec3> sortedByDistance;
	for (unsigned int i = 0; i < 2; i++)
	{
		float distance = glm::length(camera->Position - transparentObjPos[i]);
		sortedByDistance[distance] = transparentObjPos[i];
	}

	// render windows
	for (std::map<float, glm::vec3>::reverse_iterator it = sortedByDistance.rbegin(); it != sortedByDistance.rend(); ++it)
	{
		glBindVertexArray(*VAOs->at(1));
		defaultShader.setInt("texture1", 7);
		model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		defaultShader.setMat4f("model", 1, false, model);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}

	// rendering grass
	glBindVertexArray(*VAOs->at(1));
	defaultShader.setInt("texture1", 6);
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(3.0, -1.0, 0.0));
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	defaultShader.setMat4f("model", 1, false, model);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindVertexArray(0);


}