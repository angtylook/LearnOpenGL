#ifndef MODEL_LOADER_MODEL_LOADER_H_
#define MODEL_LOADER_MODEL_LOADER_H_

#include <vector>
#include "model/model.h"

class ModelLoader {
public:
    ModelLoader(Model& model);
    virtual ~ModelLoader();
    virtual bool Load(const std::string& path) = 0;

protected:
    Model& GetModel();

private:
    Model& model_ref_;
};

#endif  // MODEL_LOADER_MODEL_LOADER_H_
