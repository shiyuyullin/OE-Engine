#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"
#include "Camera.h"
#include "Texture.h"
#include "cube.h"

using namespace std;


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 lightSourcePosition(6.2f, 0.0f, -2.0f);
float lightSourceMoveRange = 3.0f;
float lightSourceMoveSpeed = 1.0f;

Camera* camera = new Camera();

float deltaTime = 0.0f;
float lastFrame = 0.0f;



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// When processing input, using delta time to make sure that frame rate won't impact the 
// distance of movement
// Higher delta time value means that rendering time between current frame and last frame is longer 
// (overall frame rate is lower), thus speed will be higher (multiplying by higher value) to balance it out  

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}
}

// mouse movement ----

// initialize the cursor position, they are initialized to be at the center of the screen
// since the screen resolution is 800 x 600
float lastX = 400, lastY = 300;

bool firstTimeMouse = true;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	// issue -- the callback is called with the position where the cursor entered the screen from,
	// that location could be significantly far from the center of the screen resulting in a big jump of view 
	// solve -- Update initial cursor position to the new position 
	if (firstTimeMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstTimeMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // ypos - lastY -> reversed, mouse up -> camera down, mouse down -> camera up
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);
}

// ----

// wheel movement ----
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll(yoffset);
}
// ----

void renderBackgroundWithColor()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(800, 600, "Playground", NULL, NULL);

	if (window == NULL)
	{
		cout << "failed to create glfw window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "failed to initialize GLAD" << endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// resizing window & resize viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// limit frame rate
	//glfwSwapInterval(1);

	// capture mouse input and disable cursor so that the cursor always stays at the center of the screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// creating vertex array object
	unsigned int laughFaceCubeVAO;
	glGenVertexArrays(1, &laughFaceCubeVAO);

	// creating vertex buffer
	unsigned int laughFaceCubeVBO;
	glGenBuffers(1, &laughFaceCubeVBO);

	glBindVertexArray(laughFaceCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, laughFaceCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesWithTexCoord), cubeVerticesWithTexCoord, GL_STATIC_DRAW);

	// set vertex attribute pointer, basically how to get all information of each vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind VAO, VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesWithTexAndNormal), cubeVerticesWithTexAndNormal, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// unbind VAO, VBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	// let stb flip y axis when loading the image, usually the image's (0,0) is at top of y axis
	stbi_set_flip_vertically_on_load(true);

	// creating wood texture
	Texture woodTexture("texture/container.jpg", GL_RGB);
	// reference to the texture
	unsigned int &wood = woodTexture.texture;

	// laugh face texture
	Texture laughFaceTexture("texture/awesomeface.png", GL_RGBA);
	unsigned int& laughFace = laughFaceTexture.texture;

	// wood steel texture (also called diffuse map when used in lighting)
	Texture woodSteelTexture("texture/container2.png", GL_RGBA);
	unsigned int& woodSteel = woodSteelTexture.texture;

	// wood steel specular map (used for lighting)
	Texture woodSteelSpecularMap("texture/container2_specular.png", GL_RGBA);
	unsigned int& woodSteelSpecMap = woodSteelSpecularMap.texture;

	// matrix emission map (glowing effect on objects)
	Texture matrix("texture/matrix.jpg", GL_RGB);
	unsigned int& matrixEmissionMap = matrix.texture;


	// set texture warp/filter options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wood);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, laughFace);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, woodSteel);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, woodSteelSpecMap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, matrixEmissionMap);

	// creating shaders and shader program
	Shader plainShaderObj("VertexShader.glsl", "FragmentShader.glsl");

	Shader lightShaderObj("VertexShaderLight.glsl", "FragmentShaderLight.glsl");

	Shader lightSourceShaderObj("VertexShaderLightSource.glsl", "FragmentShaderLightSource.glsl");


	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	glEnable(GL_DEPTH_TEST);

	// main render loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();

		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderBackgroundWithColor();

		glBindVertexArray(laughFaceCubeVAO);
		plainShaderObj.use();

		plainShaderObj.setInt("wood", 0);
		plainShaderObj.setInt("laughFace", 1);
		plainShaderObj.setFloat("blendIntensity", 0.5f);
		glm::mat4 view;
		view = camera->GetViewMatrix();
		plainShaderObj.setMat4f("view", 1, false, view);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		plainShaderObj.setMat4f("projection", 1, false, projection);
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			plainShaderObj.setMat4f("model", 1, false, model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(cubeVAO);

		// light source bouces in the range 
		lightSourcePosition.x = 6.2f + (lightSourceMoveRange * sin(lightSourceMoveSpeed * time));

		lightShaderObj.use();
		glm::mat4 modelForLightShader = glm::mat4(1.0f);
		modelForLightShader = glm::translate(modelForLightShader, glm::vec3(5.0f, 0.0f, 0.0f));
		glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(modelForLightShader)));
		lightShaderObj.setMat4f("model", 1, false, modelForLightShader);
		lightShaderObj.setMat4f("view", 1, false, view);
		lightShaderObj.setMat4f("projection", 1, false, projection);
		lightShaderObj.setMat3f("normalMat", 1, false, normalMat);
		lightShaderObj.setVec3fv("cameraPos", 1, camera->Position);
		lightShaderObj.setInt("material.diffuseMap", 2);
		lightShaderObj.setInt("material.specularMap", 3);
		lightShaderObj.setInt("useEmissonMap", 1);
		lightShaderObj.setInt("material.emissionMap", 4);
		lightShaderObj.setFloat("material.shininess", 0.25f);
		lightShaderObj.setVec3fv("light.lightPosition", 1, lightSourcePosition);
		lightShaderObj.setVec3fv("light.ambient", 1, glm::vec3(1.0f));
		lightShaderObj.setVec3fv("light.diffuse", 1, glm::vec3(1.0f));
		lightShaderObj.setVec3fv("light.specular", 1, glm::vec3(1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glm::mat4 modelMatRubyCube(1.0f);
		modelMatRubyCube = glm::translate(modelMatRubyCube, glm::vec3(8.0f, 0.0f, 0.0f));
		glm::mat4 normalMatRubyCube = glm::transpose(glm::inverse(glm::mat3(modelMatRubyCube)));
		lightShaderObj.setMat4f("model", 1, false, modelMatRubyCube);
		lightShaderObj.setMat3f("normalMat", 1, false, normalMatRubyCube);
		lightShaderObj.setInt("useEmissonMap", 0);
		lightShaderObj.setInt("material.diffuseMap", 2);
		lightShaderObj.setInt("material.specularMap", 3);
		lightShaderObj.setFloat("material.shininess", 0.6f);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		lightSourceShaderObj.use();
		glm::mat4 modelForLightSourceShader = glm::mat4(1.0f);
		modelForLightSourceShader = glm::translate(modelForLightSourceShader, lightSourcePosition);
		modelForLightSourceShader = glm::scale(modelForLightSourceShader, glm::vec3(0.5f, 0.5f, 0.5f));
		lightSourceShaderObj.setMat4f("model", 1, false, modelForLightSourceShader);
		lightSourceShaderObj.setMat4f("view", 1, false, view);
		lightSourceShaderObj.setMat4f("projection", 1, false, projection);

		glDrawArrays(GL_TRIANGLES, 0, 36);




		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}

	glfwTerminate();
	return 0;


}


