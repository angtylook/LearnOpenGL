#include "gl_debug.h"

#include <cstdio>
#include <cstring>
#include <string>

GLenum glCheckError_(const char* file, int line) {
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR) {
        std::string error;
        switch (errorCode) {
            case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;
            case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;
            case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;
            case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;
            case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;
            case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
        }
        fprintf(stderr, "%s|%s (%d)\n", error.c_str(), file, line);
    }
    return errorCode;
}

void GLAPIENTRY gl_debug_output(GLenum source,
                                GLenum type,
                                GLenum id,
                                GLenum severity,
                                GLsizei length,
                                const GLchar* message,
                                const void* userParam) {
    FILE* f = NULL;
    f = fopen("gl_debug_output.txt", "a");
    if (f) {
        char debSource[16], debType[20], debSev[25];
        if (source == GL_DEBUG_SOURCE_API_ARB)
            strcpy(debSource, "OpenGL");
        else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
            strcpy(debSource, "Windows");
        else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
            strcpy(debSource, "Shader Compiler");
        else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
            strcpy(debSource, "Third Party");
        else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)
            strcpy(debSource, "Application");
        else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
            strcpy(debSource, "Other");
        if (type == GL_DEBUG_TYPE_ERROR_ARB)
            strcpy(debType, "Error");
        else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
            strcpy(debType, "Deprecated behavior");
        else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
            strcpy(debType, "Undefined behavior");
        else if (type == GL_DEBUG_TYPE_PORTABILITY_ARB)
            strcpy(debType, "Portability");
        else if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
            strcpy(debType, "Performance");
        else if (type == GL_DEBUG_TYPE_OTHER_ARB)
            strcpy(debType, "Other");
        if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)
            strcpy(debSev, "High");
        else if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
            strcpy(debSev, "Medium");
        else if (severity == GL_DEBUG_SEVERITY_LOW_ARB)
            strcpy(debSev, "Low");

        fprintf(f, "Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n",
                debSource, debType, id, debSev, message);
        fclose(f);
    }
}
