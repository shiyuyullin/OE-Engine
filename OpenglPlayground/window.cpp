#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "stb_image.h"

using namespace std;

//// vertices for a triangle
//float vertices[] = {
//	// positions		// colors
//	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//	 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
//	 0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f
//
//};

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




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

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
	//glfwSwapInterval(2);

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
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind VAO
	glBindVertexArray(0);

	// let stb flip y axis when loading the image, usually the image's (0,0) is at top of y axis
	stbi_set_flip_vertically_on_load(true);

	// create wood texture
	unsigned int wood;
	glGenTextures(1, &wood);
	glBindTexture(GL_TEXTURE_2D, wood);
	// loading wood image
	int width, height, nrChannels;
	unsigned char* data = stbi_load("texture/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "failed to load wood image" << endl;
	}
	// free image memory
	stbi_image_free(data);

	// create laugh face texture
	unsigned int laughFace;
	glGenTextures(1, &laughFace);
	glBindTexture(GL_TEXTURE_2D, laughFace);
	// load laugh face image
	unsigned char *laughFaceData = stbi_load("texture/awesomeface.png", &width, &height, &nrChannels, 0);
	if (laughFaceData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, laughFaceData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "failed to load wood image" << endl;
	}
	// free image memory
	stbi_image_free(laughFaceData);

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

	/*glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));

	glm::mat4 transform2 = glm::mat4(1.0f);
	transform2 = glm::translate(transform2, glm::vec3(-0.5f, 0.5f, 0.0f));*/


	glm::mat4 view = glm::mat4(1.0f);
	// this has the same visual effect as we move the camera backward, but we are actually moving the scene forward, thus negative z-axis
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


	glEnable(GL_DEPTH_TEST);

	// main render loop
	while (!glfwWindowShouldClose(window))
	{
		float time = glfwGetTime();

		processInput(window);
		glClear(GL_DEPTH_BUFFER_BIT);
		renderBackgroundWithColor();

		//model = glm::rotate(model, time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		/*shaderObj->setMat4f("model", 1, false, model);
		shaderObj->setMat4f("view", 1, false, view);
		shaderObj->setMat4f("projection", 1, false, projection);*/

		/*transform = glm::rotate(transform, time, glm::vec3(0.0, 0.0, 1.0));
		shaderObj->setMat4f("transform", 1, false, transform);*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		/*transform2 = glm::rotate(transform2, time, glm::vec3(0.0, 1.0, 0.0));
		shaderObj->setMat4f("transform", 1, false, transform2);*/
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		shaderObj->setMat4f("view", 1, false, view);
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

		//glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwPollEvents();
		glfwSwapBuffers(window);
		
	}

	glfwTerminate();
	return 0;


}


