#ifndef MODEL_LOADER_SWEEP_LOADER_H_
#define MODEL_LOADER_SWEEP_LOADER_H_

#include <vector>
#include "model/curve.h"
#include "model_loader.h"

class SweepLoader : public ModelLoader {
public:
    using ModelLoader::ModelLoader;

public:
    virtual bool Load(const std::string& path) override;

private:
    void evalBezier(const std::vector<glm::vec3>& P, unsigned steps);
    void evalBspline(const std::vector<glm::vec3>& P, unsigned steps);
    void evalCircle(float radius, unsigned steps);
    void makeSurfRev(const Curve& profile, unsigned steps);
    void makeGenCyl(const Curve& profile, const Curve& sweep);
};

#endif  // MODEL_LOADER_SWEEP_LOADER_H_
