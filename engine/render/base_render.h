#ifndef RENDER_BASE_RENDER_H_
#define RENDER_BASE_RENDER_H_

#include <string>
#include <vector>

#include "base/micros.h"

class BaseRender {
public:
    BaseRender();
    virtual ~BaseRender();
    virtual void Render(const std::string& opt_material) = 0;

private:
    DISALLOW_COPY_AND_ASSIGN(BaseRender)
};

#endif  // RENDER_BASE_RENDER_H_
