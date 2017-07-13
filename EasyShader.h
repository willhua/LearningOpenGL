#ifndef EASY_SHADER_H

#define EASY_SHADER_H

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>

class EasyShader
{
public:
    EasyShader(const char *vertexShaderPath, const char* fragmentShaderPath);
    ~EasyShader();

    unsigned int getProgramID()
    {
        return ID;
    }

    void use() 
    {
        glUseProgram(ID);
    }

    void setInt(std::string name, int value)
    {
        glUniform1i(glGetAttribLocation(ID, name.c_str()), value);
    }

    void setFloat(std::string name, float value)
    {
        glUniform1f(glGetAttribLocation(ID, name.c_str()), value);
    }

    void setFloat(std::string name, float x, float y)
    {
        glUniform2f(glGetAttribLocation(ID, name.c_str()), x, y);
    }

    void setFloat(std::string name, float x, float y, float z)
    {
        glUniform3f(glGetAttribLocation(ID, name.c_str()), x, y, z);
    }

    void setFloat(std::string name, float x, float y, float z, float w)
    {
        glUniform4f(glGetAttribLocation(ID, name.c_str()), x, y, z, w);
    }

private:
    unsigned int ID;
    void checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }
};

EasyShader::EasyShader(const char * vertexShaderPath, const char * fragmentShaderPath)
{
    std::string vertexcode, fragmentcode;
    std::ifstream vertexFileStream, fragmentFileStream;

    // ensure ifstream objects can throw exceptions:
    vertexFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertexFileStream.open(vertexShaderPath);
        fragmentFileStream.open(fragmentShaderPath);
        std::stringstream vss, fss;

        //read from file to stream
        vss << vertexFileStream.rdbuf();
        fss << fragmentFileStream.rdbuf();
        vertexFileStream.close();
        fragmentFileStream.close();

        //convert stream to code
        vertexcode = vss.str();
        fragmentcode = fss.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "EasyShader read file fail:" << std::endl;
    }

    const char *vsource = vertexcode.c_str();
    const char *fsource = fragmentcode.c_str();
    int success;
    char info[512];

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vsource, NULL);
    glCompileShader(vertexShader);
    checkCompileErrors(vertexShader, "VERTEX_SHADER");

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fsource, NULL);
    glCompileShader(fragmentShader);
    checkCompileErrors(fragmentShader, "FRAGMENT_SHADER");

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

}

EasyShader::~EasyShader()
{
}

#endif // !EASY_SHADER_H
