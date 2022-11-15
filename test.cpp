
#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "stb_image.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	} 
void error_callback( int error, const char *msg ) {
    std::string s;
    s = " [" + std::to_string(error) + "] " + msg + '\n';
    std::cerr << s << std::endl;
}
void processInput(GLFWwindow *window, int &combine, float &xOffset, float &yOffset)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
		combine = 0;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		combine = 1;
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		combine = 2;
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		combine = 3;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		xOffset += 0.01;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		xOffset -= 0.01;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		yOffset += 0.01;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		yOffset -= 0.01;
}

int main() {

	glfwSetErrorCallback( error_callback );

	if( GL_TRUE != glfwInit() )
		std::cerr << "Failed to init" << std::endl;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	float rectangle[] = {
		// positions        // colors          // texture coords
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
	}; 
	unsigned int rectangleVerts[] = {
		0, 1, 3, 
		1, 2, 3
	};
	const unsigned int numTriangles = 2;
	unsigned int VBO[numTriangles], VAO[numTriangles], EBO[numTriangles];
	glGenBuffers(numTriangles, VBO);
	glGenBuffers(numTriangles, EBO);
	glGenVertexArrays(numTriangles, VAO);

	glBindVertexArray(VAO[0]);

	// Binding the VBO to the 
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangle), rectangle, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectangleVerts), rectangleVerts, GL_STATIC_DRAW);
	
	// Position 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	int width, height, nrChannels;
	unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	Shader shader("vertshader.glsl", "fragshader.glsl");

	
	int combine = 4;
	float xOffset = 0;
	float yOffset = 0;
	while(!glfwWindowShouldClose(window))
	{
		// input
		processInput(window, combine, xOffset, yOffset);

		// rendering commands here
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, texture);

		shader.use();
		shader.setInt("combine", combine);
		shader.setFloat("xOffset", xOffset);
		shader.setFloat("yOffset", yOffset);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();    
	}


	glfwTerminate();
	return 0;
}
