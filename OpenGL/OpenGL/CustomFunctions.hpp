#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;


struct ShaderProgramSource
{
    string VertexSource;
    string FragmentSource;
};

/** <summary>
* Shader File�� �о Vertex, Fragment Shader string ��������
* </summary>
* <param name="filePath">Shader File ���</param>
* <returns>ShaderProgramSource { Vertex, Fragment }</returns>
*/
static ShaderProgramSource ParseShader(const string& filePath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ifstream stream(filePath);
    string line;
    stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    // stream���� �� �پ� �о����
    while (getline(stream, line))
    {
        // #shader ������ ���� vertex, fragement shader ������ �ν�
        if (line.find("#shader") != string::npos)
        {
            if (line.find("vertex") != string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        // #shader�� ���� �κп����� stream�� ���� �߰�
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/** <summary>
* Shader �ҽ� �ڵ带 �Է¹޾� Shader ��ü �����ϰ� ������
* </summary>
* <param name="source">Shader �ҽ� �ڵ�</param>
* <param name="type">GL_VERTEX_SHADER �Ǵ� GL_FRAGMENT_SHADER</param>
* <returns>Compiled Shader ��ü�� ID �Ǵ� 0(����)</returns>
*/
static GLuint CompileShader(GLenum type, const string& source)
{
    // �� Shader ��ü ����
    GLuint shaderID = glCreateShader(type);
    const char* src = source.c_str();

    // Shader �ڵ带 Shader ��ü�� ��ȯ�Ͽ� ���� Shader ��ü id�� binding
    glShaderSource(shaderID, 1, &src, nullptr);

    // Shader Compile
    glCompileShader(shaderID);

    // Shader Compile ��� ����
    GLint result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    // Compile�� ������ ���
    if (result == GL_FALSE)
    {
        int length;

        // Log�� ���̰��� length�� �޾ƿ���
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

        // Stack�� �迭 ���� �Ҵ�
        char* message = (char*)_malloca(length * sizeof(char));

        // Log �޽��� �޾ƿ���
        glGetShaderInfoLog(shaderID, length, &length, message);

        // Error Message ���
        cout << "Failed to Compile "
            << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment")
            << " Shader"
            << endl;

        cout << "\nError Message :" << endl;
        cout << message << endl;

        // Shader Memory ����
        glDeleteShader(shaderID);

        _freea(message);
        return 0;
    }

    return shaderID;
}

/** <summary>
* ���α׷�, Shader ��ü ����
* </summary>
* <param name="vertexShader">Vertex Shader �ҽ� �ڵ�</param>
* <param name="fragmentShader">Fragment Shader �ҽ� �ڵ�</param>
* <returns>������ ���α׷� ��ü�� ID</returns>
*/
static GLuint CreateShader(const string& vertexShader, const string& fragmentShader)
{
    // �� ���α׷� ��ü ����
    GLuint programID = glCreateProgram();

    // Shader ������
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // ���α׷��� Shader ����, ���α׷� ��ŷ, ����
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);
    glLinkProgram(programID);
    glValidateProgram(programID);

    // Shader �޸� ����
    glDeleteShader(vs);
    glDeleteShader(fs);

    return programID;
}