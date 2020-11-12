#ifndef RENDER_SKYBOX_H_
#define RENDER_SKYBOX_H_

#include <string>
#include <vector>
#include "GL/glew.h"
#include "base/micros.h"

class Skybox {
public:
    Skybox();
    ~Skybox();
    void Init(const std::string& texture);
    void SetCubeTexture(const std::string& texture);
    void Render();

private:
    GLuint vao_;
    GLuint vbo_;
    GLuint ebo_;
    GLuint texture_;

private:
    DISALLOW_COPY_AND_ASSIGN(Skybox)
};

#endif  // RENDER_SKYBOX_H_
