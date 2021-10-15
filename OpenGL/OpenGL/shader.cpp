#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

/*
<summary>
shader �ҽ� �ڵ带 �Է¹޾� shader ��ü �����ϰ� compile
</summary>
<param name="source">shader �ҽ� �ڵ�</param>
<param name="type">GL_VERTEX_SHADER �Ǵ� GL_FRAGMENT_SHADER</param>
<returns>compile�� shader ��ü�� ID �Ǵ� 0(error)</returns>
*/
static GLuint CompileShader(GLenum type, const string& source) {
	// �� shader ��ü ����
	GLuint shaderID = glCreateShader(type);
	const char* src = source.c_str();

	// shader �ڵ带 shader ��ü�� ��ȯ�Ͽ� ���� shader ��ü id�� binding
	glShaderSource(shaderID, 1, &src, nullptr);

	// shader compile
	glCompileShader(shaderID);

	// shader compile ��� ����
	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

	// compile�� ������ ���
	if (result == GL_FALSE) {
		int length;

		// log�� ���� ���� length�� �޾ƿ���
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		// stack�� �迭 ���� �Ҵ�
		char* message = (char*)_malloca(length * sizeof(char));

		// log message �޾ƿ���
		glGetShaderInfoLog(shaderID, length, &length, message);

		// error message ���
		cout << "Failed to Compule"
			<< ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment")
			<< " Shader"
			<< endl;

		cout << "\nError Message :" << endl;
		cout << message << endl;

		// shader �޸� ����
		glDeleteShader(shaderID);

		_freea(message);
		return 0;
	}
	return shaderID;
}

/*
<summary>
���α׷�, shader ��ü ����
</summary>
<param name="vertexShader">vertex shader source code</param>
<param name="fragmentShader">fragment shader source code</param>
<returns>������ ���α׷� ��ü�� ID</returns>
*/
static GLuint CreateShader(const string& vertexShader, const string& fragmentShader) {
	// �� ���α׷� ��ü ����
	GLuint programID = glCreateProgram();

	// shader compile
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// ���α׷��� shader ����, ���α׷� linking, ����
	glAttachShader(programID, vs);
	glAttachShader(programID, fs);
	glLinkProgram(programID);
	glValidateProgram(programID);

	// shader �޸� ����
	glDeleteShader(vs);
	glDeleteShader(fs);

	return programID;
}

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

	// glewInit�� rendering context�� ����� �� ���Ŀ� �ؾ� ��.
	if (glewInit() != GLEW_OK) {
		cout << "GLEW INIT ERROR" << endl;
	}

	// ���� ��ġ ������
	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f,
	};

	// VBO (Vertex Buffer Object) ID ����
	GLuint buffer;

	// VBO ��ü ����
	glGenBuffers(1, &buffer);

	// VBO type binding
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// VBO�� ��ġ ������ ����
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	//���� �Ӽ� ����
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	//���� �Ӽ� Ȱ��ȭ
	glEnableVertexAttribArray(0);

	// shader �ۼ�
	string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   gl_Position = position;\n"
		"}\n";
	// ����� glsl ���� ����(330 -> 3.3 version)
	// ���� ��ǥ attribute�� index�� location�� ����
	// ���� ���� ��ǥ�� vec2������, ��ǥ�� �׻� vec4�� ���

	string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   color = vec4(0.2, 0.3, 0.8, 1.0);\n"
		"}\n";

	// shader ��ü ���� �� compile, ���α׷� ��ü ����
	GLuint shaderProgram = CreateShader(vertexShader, fragmentShader);

	// shader ���α׷� ��ü ���
	glUseProgram(shaderProgram);

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

	// ���α׷� ��ü �޸� ����
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}