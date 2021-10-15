#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

/*
<summary>
shader 소스 코드를 입력받아 shader 객체 생성하고 compile
</summary>
<param name="source">shader 소스 코드</param>
<param name="type">GL_VERTEX_SHADER 또는 GL_FRAGMENT_SHADER</param>
<returns>compile된 shader 객체의 ID 또는 0(error)</returns>
*/
static GLuint CompileShader(GLenum type, const string& source) {
	// 빈 shader 객체 생성
	GLuint shaderID = glCreateShader(type);
	const char* src = source.c_str();

	// shader 코드를 shader 객체로 변환하여 위의 shader 객체 id에 binding
	glShaderSource(shaderID, 1, &src, nullptr);

	// shader compile
	glCompileShader(shaderID);

	// shader compile 결과 검증
	GLint result;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

	// compile이 실패한 경우
	if (result == GL_FALSE) {
		int length;

		// log의 길이 값을 length에 받아오기
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

		// stack에 배열 동적 할당
		char* message = (char*)_malloca(length * sizeof(char));

		// log message 받아오기
		glGetShaderInfoLog(shaderID, length, &length, message);

		// error message 출력
		cout << "Failed to Compule"
			<< ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment")
			<< " Shader"
			<< endl;

		cout << "\nError Message :" << endl;
		cout << message << endl;

		// shader 메모리 해제
		glDeleteShader(shaderID);

		_freea(message);
		return 0;
	}
	return shaderID;
}

/*
<summary>
프로그램, shader 객체 생성
</summary>
<param name="vertexShader">vertex shader source code</param>
<param name="fragmentShader">fragment shader source code</param>
<returns>생성된 프로그램 객체의 ID</returns>
*/
static GLuint CreateShader(const string& vertexShader, const string& fragmentShader) {
	// 빈 프로그램 객체 생성
	GLuint programID = glCreateProgram();

	// shader compile
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// 프로그램에 shader 장착, 프로그램 linking, 검증
	glAttachShader(programID, vs);
	glAttachShader(programID, fs);
	glLinkProgram(programID);
	glValidateProgram(programID);

	// shader 메모리 해제
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

	// shader 작성
	string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   gl_Position = position;\n"
		"}\n";
	// 사용할 glsl 버전 설정(330 -> 3.3 version)
	// 정점 좌표 attribute의 index를 location에 전달
	// 지금 정점 좌표는 vec2이지만, 좌표는 항상 vec4로 사용

	string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"\n"
		"void main()\n"
		"{\n"
		"   color = vec4(0.2, 0.3, 0.8, 1.0);\n"
		"}\n";

	// shader 객체 생성 및 compile, 프로그램 객체 생성
	GLuint shaderProgram = CreateShader(vertexShader, fragmentShader);

	// shader 프로그램 객체 사용
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

	// 프로그램 객체 메모리 해제
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}