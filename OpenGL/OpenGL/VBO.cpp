#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

int main(void) {
	GLFWwindow* window;

	// Initialize the library
	if (!glfwInit())
		return -1;

	// Create a windowed mode windoe and its OpenGL context
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// glewInit은 rendering context를 만들고 난 이후에 해야 함.
	if (glewInit() != GLEW_OK) {
		cout << "GLEW INIT ERROR" << endl;
	}

	// 정점 위치 데이터
	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f,
	};

	// VBO (Vertex Buffer Object) ID 생성
	GLuint buffer;

	// VBO 객체 생성
	glGenBuffers(1, &buffer);

	// VBO type binding
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// VBO에 위치 데이터 연결
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//정점 속성 정의
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//정점 속성 활성화
	glEnableVertexAttribArray(0);

	// Loop until the user closes the window
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}