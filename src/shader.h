#pragma once
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

void printShaderLog(GLuint shader){
    int len = 0;
    int written = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0){
        log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, &written, log);
        std::cout << "shader info log: " << log << std::endl;
        free(log);
    }
}

void printProgramLog(GLuint program){
    int len = 0;
    int written = 0;
    char *log;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if(len > 0){
        log = (char*)malloc(len);
        glGetProgramInfoLog(program, len, &written, log);
        std::cout << "program info log: " << log << std::endl;
        free(log);
    }
}

bool checkOpenGLError(){
    bool foundError = false;
    int error = glGetError();
    while(error != GL_NO_ERROR){
        std::cout << "glError: " << error << std::endl;
        foundError = true;
        error = glGetError();
    }
    return foundError;
}

std::string readShaderSource(const char* filePath){
    std::ifstream file(filePath);
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    return content;
}

GLuint createProgram(const char* vertexFileLoc, const char* fragmentFileLoc){
    std::string vShaderSource = readShaderSource(vertexFileLoc);
    std::string fShaderSource = readShaderSource(fragmentFileLoc);

    const char* vShaderSourceChar = vShaderSource.c_str();
    const char* fShaderSourceChar = fShaderSource.c_str();

    GLint vertexIsCompiled;
    GLint fragmentIsCompiled;
    GLint linked;

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vshader, 1, &vShaderSourceChar, NULL);
    glShaderSource(fshader, 1, &fShaderSourceChar, NULL);

    glCompileShader(vshader);
    checkOpenGLError();
    glGetShaderiv(vshader, GL_COMPILE_STATUS, &vertexIsCompiled);
    if(vertexIsCompiled != 1){
        std::cout << "vertex compilation failed\n";
        printShaderLog(vshader);
    }

    glCompileShader(fshader);
    checkOpenGLError();
    glGetShaderiv(fshader, GL_COMPILE_STATUS, &fragmentIsCompiled);
    if(fragmentIsCompiled != 1){
        std::cout << "fragment compilation failed\n";
        printShaderLog(fshader);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);

    glLinkProgram(program);
    checkOpenGLError();
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if(linked != 1){
        std::cout << "linking failed\n";
        printProgramLog(program);
    }

    return program;
}

