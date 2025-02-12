#include <iostream>
#include <algorithm>

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
#include "plane.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "quad.h"
#include "Cubemap.h"

using namespace std;

Camera camera{};

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
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
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// ----

// wheel movement ----
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
// ----

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
	glfwSetFramebufferSizeCallback(window, Utils::framebuffer_size_callback);

	// limit frame rate
	//glfwSwapInterval(1);

	// capture mouse input and disable cursor so that the cursor always stays at the center of the screen
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// create an attachement to the framebuffer, here I use texture but it could be a renderbuffer
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	// attach to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	// create renderbuffer for storing depth and stencil values
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// attach to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	// check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "ERROR::Framebuffer not complete" << endl;
	}
	else
	{
		cout << "Framebuffer is complete" << endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// creating vertex array object
	unsigned int laughFaceCubeVAO, laughFaceCubeVBO;
	glGenVertexArrays(1, &laughFaceCubeVAO);
	// creating vertex buffer
	glGenBuffers(1, &laughFaceCubeVBO);
	glBindVertexArray(laughFaceCubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, laughFaceCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesWithTexCoord), cubeVerticesWithTexCoord, GL_STATIC_DRAW);
	// set vertex attribute pointer, basically how to get all information of each vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
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
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// plane VAO, VBO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	// cube with vertices specified counter clockwise
	unsigned int cubeVAOccw, cubeVBOccw;
	glGenVertexArrays(1, &cubeVAOccw);
	glGenBuffers(1, &cubeVBOccw);
	glBindVertexArray(cubeVAOccw);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBOccw);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesCCW), cubeVerticesCCW, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// quad which is just the size of the window
	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// skybox
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glBindVertexArray(skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxCube), skyboxCube, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// creating wood texture
	Texture woodTexture("texture/container.jpg", GL_RGB, GL_RGB);
	// reference to the texture
	unsigned int &wood = woodTexture.texture;

	// laugh face texture
	Texture laughFaceTexture("texture/awesomeface.png", GL_RGB, GL_RGBA);
	unsigned int& laughFace = laughFaceTexture.texture;

	// wood steel texture (also called diffuse map when used in lighting)
	Texture woodSteelTexture("texture/container2.png", GL_RGB, GL_RGBA);
	unsigned int& woodSteel = woodSteelTexture.texture;

	// wood steel specular map (used for lighting)
	Texture woodSteelSpecularMap("texture/container2_specular.png", GL_RGB, GL_RGBA);
	unsigned int& woodSteelSpecMap = woodSteelSpecularMap.texture;

	// matrix emission map (glowing effect on objects)
	Texture matrix("texture/matrix.jpg", GL_RGB, GL_RGB);
	unsigned int& matrixEmissionMap = matrix.texture;

	// brickwall texture
	Texture brickWallTexture("texture/brickwall.jpg", GL_RGB, GL_RGB);
	unsigned int& brickWall = brickWallTexture.texture;

	// grass texture
	Texture grassTexture("texture/grass.png", GL_RGBA, GL_RGBA);
	unsigned int& grass = grassTexture.texture;

	// window texture
	Texture windowTexture("texture/blending_transparent_window.png", GL_RGBA, GL_RGBA);
	unsigned int& redWindow = windowTexture.texture;

	// cubmap
	Cubemap skyboxTexture(faces);
	unsigned int& skybox = skyboxTexture.texture;

	// set texture warp/filter options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// activiate textures, assign indices to them
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
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, brickWall);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, grass);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, redWindow);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);


	// creating shaders and shader program
	Shader defaultShader("VertexShaderDefault.glsl", "FragmentShaderDefault.glsl");

	Shader lightShaderObj("VertexShaderLight.glsl", "FragmentShaderLight.glsl");

	Shader lightSourceShaderObj("VertexShaderLightSource.glsl", "FragmentShaderLightSource.glsl");

	Shader blinnLightShaderObj("VertexShaderBlinnLight.glsl", "FragmentShaderBlinnLight.glsl");

	Shader zBufferShader("VertexShaderZBuffer.glsl", "FragmentShaderZBuffer.glsl");

	Shader stencilShader("VertexShaderStencil.glsl", "FragmentShaderStencil.glsl");

	Shader ppInversionShader("VertexShaderPp.glsl", "FragmentShaderPpInversion.glsl");

	Shader ppGragscaleShader("VertexShaderPp.glsl", "FragmentShaderPpGrayscale.glsl");

	Shader skyboxShader("VertexShaderSkybox.glsl", "FragmentShaderSkybox.glsl");

	Shader simpleDepthShader("VertexShaderDepth.glsl", "FragmentShaderDepth.glsl");

	Shader shadowMappingShader("VertexShaderShadowMapping.glsl", "FragmentShaderShadowMapping.glsl");

	// initialize scene 1
	vector<reference_wrapper<Shader>>* shaders = new vector<reference_wrapper<Shader>>({lightShaderObj, blinnLightShaderObj, 
		lightSourceShaderObj, zBufferShader, stencilShader, defaultShader, simpleDepthShader, shadowMappingShader });
	vector<GLuint*>* VAOs = new vector<GLuint*>({ &cubeVAO, &planeVAO, &cubeVAOccw });
	Scene1 *scene1 = new Scene1(&camera, shaders, VAOs);
	Scene2 *scene2 = new Scene2(&camera, shaders, VAOs);
	Scene3* scene3 = new Scene3(&camera, shaders, VAOs);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	// opengl by default cull back faces (vertices clockwise)
	//glEnable(GL_CULL_FACE);


	// main render loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();

		float currentFrame = time;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// disable writting to depth buffer
		glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		
		//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		//glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//// rendering
		////scene1->drawPlane();
		//scene1->render();
		////scene1->renderDepthBuffer();
		////scene1->renderOutlining();
		///*scene2->drawPlane();
		//scene2->render();*/

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		//ppInversionShader.use();
		////ppGragscaleShader.use();
		//glBindVertexArray(quadVAO);
		//ppInversionShader.setInt("screenTexture", 8);
		//glDrawArrays(GL_TRIANGLES, 0, 6);


		////scene1->drawPlane();
		//scene1->render();
		////scene1->renderDepthBuffer();
		////scene1->renderOutlining();
		///*scene2->drawPlane();
		//scene2->render();*/

		//Utils::drawSkybox(9, skyboxVAO, skyboxShader, &camera);

		scene3->render();

		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	glfwTerminate();



	return 0;


}

