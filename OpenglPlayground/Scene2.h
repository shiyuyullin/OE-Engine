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
	cout << "Scene 2 initialized successfully" << endl;
}

Scene2::~Scene2()
{

}

void Scene2::render()
{

}