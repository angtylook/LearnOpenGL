#include <stdio.h>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

#include <stdlib.h>

#include "base/filesystem.h"
#include "gl_shader.h"

namespace {
std::string ReadShaderContent(const char* file) {
    std::string shader_code;
    std::ifstream instream(file, std::ios::in);
    if (instream.is_open()) {
        std::string line;
        while (getline(instream, line))
            shader_code += "\n" + line;
        instream.close();
    } else {
        printf("Impossible to open %s. \n", file);
    }
    return shader_code;
}
}  // namespace

GLuint LoadShaders(const char* vertex_file_path,
                   const char* fragment_file_path) {
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode(ReadShaderContent(
        engine::fs::GetRawPath(engine::fs::Path(vertex_file_path)).data()));
    if (VertexShaderCode.empty()) {
        fprintf(stderr, "vertex shader file<%s> is empty.\n", vertex_file_path);
        return 0;
    }
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode(ReadShaderContent(
        engine::fs::GetRawPath(engine::fs::Path(fragment_file_path)).data()));
    if (FragmentShaderCode.empty()) {
        fprintf(stderr, "fragment shader file<%s> is empty.\n",
                fragment_file_path);
        return 0;
    }

    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile Vertex Shader
    fprintf(stdout, "Compiling shader : %s\n", vertex_file_path);
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    GLint Result = GL_FALSE;
    int InfoLogLength = 0;
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
                           &VertexShaderErrorMessage[0]);
        fprintf(stderr, "%s\n", &VertexShaderErrorMessage[0]);
    }
    if (Result == GL_FALSE) {
        return 0;
    }

    // Compile Fragment Shader
    fprintf(stdout, "Compiling shader : %s\n", fragment_file_path);
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    Result = GL_FALSE;
    InfoLogLength = 0;
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
                           &FragmentShaderErrorMessage[0]);
        fprintf(stderr, "%s\n", &FragmentShaderErrorMessage[0]);
    }

    if (Result == GL_FALSE) {
        return 0;
    }

    // Link the program
    fprintf(stdout, "Linking program.\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    Result = GL_FALSE;
    InfoLogLength = 0;
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
                            &ProgramErrorMessage[0]);
        fprintf(stderr, "%s\n", &ProgramErrorMessage[0]);
    }
    if (Result == GL_FALSE) {
        return 0;
    }
    // clear
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);
    return ProgramID;
}
