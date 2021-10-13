#include <iostream>
#include "include/GL/glew.h"
#include "include/GLFW/glfw3.h"
GLFWwindow* window;

#include "include/glm/glm.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
using namespace glm;

#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "lib/glew32.lib")
#pragma comment(lib, "lib/glfw3.lib")

int main(void) {

	//GLFW 초기화
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW\n" << std::endl;
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); //4x 안티에일리어싱
	//OpenGL 3.3 사용
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//window 생성
	window = glfwCreateWindow(1024, 768, "OpenGLRendering", NULL, NULL);
	if (window == NULL) {
		std::cerr << "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//GLEW 초기화
	if (glewInit() != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW\n" << std::endl;
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	//배경색
	glClearColor(0.8f, 0.4f, 0.4f, 0.5f);

	do {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw nothing, see you in tutorial 2 !


		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	//escape key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);


	
	glfwTerminate();
	return 0;


}


//#include "include/GL/glew.h"		
//#include "include/GLFW/glfw3.h" 
//#include <iostream>
//
//#pragma comment(lib, "OpenGL32.lib")
//#pragma comment(lib, "lib/glew32.lib")
//#pragma comment(lib, "lib/glfw3.lib")
//
//
//
//void window_resized(GLFWwindow* window, int width, int height);
//
//void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
//void show_glfw_error(int error, const char* description);
//
//
//int main()
//{
//	glfwSetErrorCallback(show_glfw_error);
//
//
//	if (!glfwInit()) {
//		std::cerr << "GLFW 초기화 실패" << '\n';
//		exit(-1);
//	}
//
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//
//
//	GLFWwindow* window = glfwCreateWindow(
//		800, // width
//		600, // height
//		"OpenGL Example",
//
//		NULL, NULL);
//	if (!window)
//	{
//		std::cerr << "윈도우 생성 실패" << '\n';
//		glfwTerminate();
//		exit(-1);
//	}
//
//
//	glfwMakeContextCurrent(window);
//
//
//	glfwSetWindowSizeCallback(window, window_resized);
//	glfwSetKeyCallback(window, key_pressed);
//
//
//	glfwSwapInterval(1);
//
//
//	glewExperimental = GL_TRUE;
//	GLenum err = glewInit();
//	if (err != GLEW_OK) {
//		std::cerr << "GLEW 초기화 실패 " << glewGetErrorString(err) << '\n';
//		glfwTerminate();
//		exit(-1);
//	}
//
//
//	std::cout << glGetString(GL_VERSION) << '\n';
//
//
//	int nr_extensions = 0;
//	glGetIntegerv(GL_NUM_EXTENSIONS, &nr_extensions);
//
//	for (int i = 0; i < nr_extensions; ++i) {
//		std::cout << glGetStringi(GL_EXTENSIONS, i) << '\n';
//	}
//
//
//	glClearColor(0, 0, 1, 1);
//
//
//	while (!glfwWindowShouldClose(window)) {
//
//		glClear(GL_COLOR_BUFFER_BIT);
//
//		glfwSwapBuffers(window);
//
//		glfwPollEvents();
//	}
//
//
//	glfwDestroyWindow(window);
//
//
//	glfwTerminate();
//	return 0;
//}
//
//void show_glfw_error(int error, const char* description) {
//	std::cerr << "Error: " << description << '\n';
//}
//
//void window_resized(GLFWwindow* window, int width, int height) {
//	std::cout << "Window resized, new window size: " << width << " x " << height << '\n';
//
//	glClearColor(0, 0, 1, 1);
//	glClear(GL_COLOR_BUFFER_BIT);
//	glfwSwapBuffers(window);
//}
//
//void key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
//	if (key == 'Q' && action == GLFW_PRESS) {
//		glfwTerminate();
//		exit(0);
//	}
//}