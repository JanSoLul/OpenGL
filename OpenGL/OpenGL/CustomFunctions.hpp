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
* Shader File을 읽어서 Vertex, Fragment Shader string 가져오기
* </summary>
* <param name="filePath">Shader File 경로</param>
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

    // stream에서 한 줄씩 읽어오기
    while (getline(stream, line))
    {
        // #shader 라인을 통해 vertex, fragement shader 시작점 인식
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
        // #shader가 없는 부분에서는 stream에 라인 추가
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

/** <summary>
* Shader 소스 코드를 입력받아 Shader 객체 생성하고 컴파일
* </summary>
* <param name="source">Shader 소스 코드</param>
* <param name="type">GL_VERTEX_SHADER 또는 GL_FRAGMENT_SHADER</param>
* <returns>Compiled Shader 객체의 ID 또는 0(에러)</returns>
*/
static GLuint CompileShader(GLenum type, const string& source)
{
    // 빈 Shader 객체 생성
    GLuint shaderID = glCreateShader(type);
    const char* src = source.c_str();

    // Shader 코드를 Shader 객체로 변환하여 위의 Shader 객체 id에 binding
    glShaderSource(shaderID, 1, &src, nullptr);

    // Shader Compile
    glCompileShader(shaderID);

    // Shader Compile 결과 검증
    GLint result;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    // Compile이 실패한 경우
    if (result == GL_FALSE)
    {
        int length;

        // Log의 길이값을 length에 받아오기
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);

        // Stack에 배열 동적 할당
        char* message = (char*)_malloca(length * sizeof(char));

        // Log 메시지 받아오기
        glGetShaderInfoLog(shaderID, length, &length, message);

        // Error Message 출력
        cout << "Failed to Compile "
            << ((type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment")
            << " Shader"
            << endl;

        cout << "\nError Message :" << endl;
        cout << message << endl;

        // Shader Memory 해제
        glDeleteShader(shaderID);

        _freea(message);
        return 0;
    }

    return shaderID;
}

/** <summary>
* 프로그램, Shader 객체 생성
* </summary>
* <param name="vertexShader">Vertex Shader 소스 코드</param>
* <param name="fragmentShader">Fragment Shader 소스 코드</param>
* <returns>생성된 프로그램 객체의 ID</returns>
*/
static GLuint CreateShader(const string& vertexShader, const string& fragmentShader)
{
    // 빈 프로그램 객체 생성
    GLuint programID = glCreateProgram();

    // Shader 컴파일
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // 프로그램에 Shader 장착, 프로그램 링킹, 검증
    glAttachShader(programID, vs);
    glAttachShader(programID, fs);
    glLinkProgram(programID);
    glValidateProgram(programID);

    // Shader 메모리 해제
    glDeleteShader(vs);
    glDeleteShader(fs);

    return programID;
}