#include "CustomFunctions.hpp"

using namespace std;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // glewInit은 rendering context를 만들고 난 이후에 해야 함
    if (glewInit() != GLEW_OK)
    {
        cout << "GLEW INIT ERROR" << endl;
    }

    // 간단히 GLEW 버전 확인
    cout << glGetString(GL_VERSION) << endl;

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

    // Shader 파일 읽어오기
    ShaderProgramSource source = ParseShader("Resources/Shaders/Basic.shader");
    string vertexShader = source.VertexSource;
    string fragmentShader = source.FragmentSource;

    cout << "\nVERTEX\n" + vertexShader +
        "\nFRAGMENT\n" + fragmentShader << endl;

    // Shader 객체 생성 및 컴파일, 프로그램 객체 생성
    GLuint shaderProgram = CreateShader(vertexShader, fragmentShader);

    // Shader 프로그램 객체 사용
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f); // Bottom Left
        glVertex2f(0.0f, 0.5f); // Top
        glVertex2f(0.5f, -0.5f); // Bottom Right
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}