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


using namespace std;


//float vertices[] = {
//	// positions          // colors           // texture coords
//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
//};

//unsigned int recDrawOrder[] = {
//	0,1,3,
//	1,2,3
//};

float vertices[] = {
	// position				// texture
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// creating vertex buffer
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// create EBO for rectangle
	//unsigned int EBO;
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(recDrawOrder), recDrawOrder, GL_STATIC_DRAW);

	// set vertex attribute pointer, basically how to get all information of each vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind VAO
	glBindVertexArray(0);

	// let stb flip y axis when loading the image, usually the image's (0,0) is at top of y axis
	stbi_set_flip_vertically_on_load(true);

	// creating wood texture
	Texture woodTexture("texture/container.jpg", GL_RGB);
	// reference to the texture
	unsigned int &wood = woodTexture.texture;

	Texture laughFaceTexture("texture/awesomeface.png", GL_RGBA);
	unsigned int& laughFace = laughFaceTexture.texture;


	// set texture warp/filter options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// creating shaders and shader program
	Shader* shaderObj = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	// activate shader program
	shaderObj->use();

	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, wood);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, laughFace);

	shaderObj->setInt("wood", 0);
	shaderObj->setInt("laughFace", 1);
	shaderObj->setFloat("blendIntensity", 0.5f);

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

		glm::mat4 view;
		view = camera->GetViewMatrix();
		shaderObj->setMat4f("view", 1, false, view);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);
		shaderObj->setMat4f("projection", 1, false, projection);
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, time * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shaderObj->setMat4f("model", 1, false, model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}

		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}

	glfwTerminate();
	return 0;


}


