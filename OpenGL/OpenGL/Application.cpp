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

    // glewInit�� rendering context�� ����� �� ���Ŀ� �ؾ� ��
    if (glewInit() != GLEW_OK)
    {
        cout << "GLEW INIT ERROR" << endl;
    }

    // ������ GLEW ���� Ȯ��
    cout << glGetString(GL_VERSION) << endl;

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

    // Shader ���� �о����
    ShaderProgramSource source = ParseShader("Resources/Shaders/Basic.shader");
    string vertexShader = source.VertexSource;
    string fragmentShader = source.FragmentSource;

    cout << "\nVERTEX\n" + vertexShader +
        "\nFRAGMENT\n" + fragmentShader << endl;

    // Shader ��ü ���� �� ������, ���α׷� ��ü ����
    GLuint shaderProgram = CreateShader(vertexShader, fragmentShader);

    // Shader ���α׷� ��ü ���
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