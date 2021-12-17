#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
GLFWwindow* gWindow;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "glm/gtc/type_ptr.hpp"

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")

int gWindowWidth = 1280;
int gWindowHeight = 960;

float fov = 60.0f;
float gYaw = 0.0f;
float gPitch = 0.0f;
float gRadius = 10.0f;

float mYaw = 0.0f;
float mPitch = 0.0f;
float mRadius = 10.0f;

float offsetX = 0.0f;
float offsetY = 0.0f;

glm::vec3 mPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 mTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 mUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 mRight = glm::vec3(0.0f, 0.0f, 0.0f);

struct Volume {
	GLubyte* pData;
	std::vector<float> scalars;
	std::vector<glm::vec3> gradients;
}volumeData;

const float MOUSE_SENSITIVITY = 0.25f;

bool gl_init(void);
void scroll_callback(GLFWwindow*, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double posX, double posY);



int main(void) {
	if (!gl_init()) {
		std::cerr << "faile to init!\n" << std::endl;
		return 0;
	}
	
	std::string file_name = "256x256x39_16bitsigned_BE.raw";
	const char* volume = file_name.c_str();
	FILE* fp;
	if (!(fp = fopen(volume, "rb")))
	{
		std::cout << "ERROR : Failed to open " << volume << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "SUCCESS : Opened " << volume << std::endl;

	size_t size = 256*256*39*2-1;
	GLubyte* data = new GLubyte[size];
	size = (size - 1) / 2;
	
	if (fread(data, sizeof(char), size, fp) != size)
	{
		std::cout << "ERROR : Failed to read " << volume << std::endl;
		exit(EXIT_FAILURE);
	}
	fclose(fp);


	/**************************************************************************************/


	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint ProgramID = LoadShaders("vertexShader.vert", "fragmentShader.frag");
	
	GLuint MatrixID = glGetUniformLocation(ProgramID, "MVP");

	static const GLfloat g_vertex_buffer_data[] = {
		//front face
	   -1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
	   -1.0f,  1.0f,  1.0f,
	   -1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,

		//back face
	   -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
	   -1.0f,  1.0f, -1.0f,
	   -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		//left face
	   -1.0f,  1.0f, -1.0f,
	   -1.0f, -1.0f,  1.0f,
	   -1.0f,  1.0f,  1.0f,
	   -1.0f,  1.0f, -1.0f,
	   -1.0f, -1.0f, -1.0f,
	   -1.0f, -1.0f,  1.0f,

		//right race
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,

		//top face
	   -1.0f, 1.0f, -1.0f,
		1.0f, 1.0f,  1.0f,
		1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f, -1.0f,
	   -1.0f, 1.0f,  1.0f,
		1.0f, 1.0f,  1.0f,

		//bottom face
	   -1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
	   -1.0f, -1.0f,  1.0f,
	   -1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f
	};

	static const GLfloat g_color_buffer_data[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);

	do {

		glm::mat4 mymodel, myview, myprojection, MVP;

		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(ProgramID);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		mymodel = glm::mat4(1.0f);

		//setLookAt -> cubePos
		mTargetPos = cubePos;

		//rotate
		mYaw = glm::radians(gYaw);
		mPitch = glm::radians(gPitch);
		mPitch = glm::clamp(mPitch, -glm::pi<float>() / 2.0f + 0.1f, glm::pi<float>() / 2.0f - 0.1f);

		//update camera vectors;
		mPosition.x = mTargetPos.x + mRadius * cosf(mPitch) * sinf(mYaw);
		mPosition.y = mTargetPos.y + mRadius * sinf(mPitch);
		mPosition.z = mTargetPos.z + mRadius * cosf(mPitch) * cosf(mYaw);

		//set Radius
		mRadius = glm::clamp(gRadius, 2.0f, 80.0f);

		mymodel = glm::translate(mymodel, cubePos);

		//translation
		mTargetPos.x -= offsetX;
		mTargetPos.y += offsetY;
		//mPosition.x += offsetX;
		//mPosition.y += offsetY;
		
		myview = glm::lookAt(mPosition, mTargetPos, mUp);

		myprojection = glm::perspective(glm::radians(fov), (float)gWindowWidth / (float)gWindowHeight, 0.1f, 100.0f);

		MVP = myprojection * myview * mymodel;

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		glfwSwapBuffers(gWindow);

	} while (glfwGetKey(gWindow, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(gWindow) == 0);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteProgram(ProgramID);
	glDeleteVertexArrays(1, &VertexArrayID);

	glfwTerminate();
	return 0;
}

bool gl_init() {
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, "Opengl Rendering", NULL, NULL);
	if (gWindow == NULL) {
		std::cerr << "Failed to open GLFW window.\n" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(gWindow);

	//GLEW ÃÊ±âÈ­
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW.\n" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwSetInputMode(gWindow, GLFW_STICKY_KEYS, GL_TRUE);

	//scroll ÁÜÀÎ, ÁÜ¾Æ¿ô
	glfwSetScrollCallback(gWindow, scroll_callback);

	//¸¶¿ì½º ÄÝ¹é
	glfwSetCursorPosCallback(gWindow, mouse_callback);

	//¹è°æ»ö
	glClearColor(0.8f, 0.4f, 0.4f, 0.5f);

	glViewport(0, 0, gWindowWidth, gWindowHeight);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	return true;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov -= (float)yoffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 200.0f)
		fov = 200.0f;
}

void mouse_callback(GLFWwindow* window, double posX, double posY) {
	static glm::vec2 lastMousePos = glm::vec2(0, 0);
	//rotation
	if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_RIGHT) == 1) {
		gYaw -= ((float)posX - lastMousePos.x) * MOUSE_SENSITIVITY;
		gPitch += ((float)posY - lastMousePos.y) * MOUSE_SENSITIVITY;
	}
	
	//translate 
	if (glfwGetMouseButton(gWindow, GLFW_MOUSE_BUTTON_LEFT) == 1) {
		offsetX += ((float)posX - lastMousePos.x) * 0.01f;
		offsetY += ((float)posY - lastMousePos.y) * 0.01f;
	}

	lastMousePos.x = (float)posX;
	lastMousePos.y = (float)posY;
}
