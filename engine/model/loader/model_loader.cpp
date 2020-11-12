#include "model_loader.h"

ModelLoader::ModelLoader(Model& model) : model_ref_(model) {}

ModelLoader::~ModelLoader() {}

Model& ModelLoader::GetModel() {
    return model_ref_;
}
