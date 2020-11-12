#include "model_manager_impl.h"

#include <chrono>
#include <memory>
#include <set>
#include <vector>
#include "base/filesystem.h"
#include "model/loader/md5_loader.h"
#include "model/loader/obj_loader.h"
#include "model/loader/skeleton_loader.h"
#include "model/loader/sweep_loader.h"
#include "model/model.h"

namespace engine {
Model* ModelManagerImpl::GetModel(const std::string& name) {
    if (models_.find(name) != models_.end()) {
        return models_[name].get();
    } else {
        fprintf(stderr, "ModelManager: GetModel model<%s> not found.",
                name.c_str());
        return nullptr;
    }
}

std::unique_ptr<ModelLoader> ModelManagerImpl::CreateLoader(Model* model) {
    if (!model)
        return nullptr;

    std::string model_type(model->GetType());
    if (model->GetType() == "obj") {
        return std::make_unique<ObjLoader>(*model);
    } else if (model_type == "swp") {
        return std::make_unique<SweepLoader>(*model);
    } else if (model_type == "mdl") {
        return std::make_unique<SkeletonLoader>(*model);
    } else if (model_type == "md5") {
        return std::make_unique<MD5ModelLoader>(*model);
    } else {
        fprintf(stderr, "unsupport model type.\n");
    }
    return nullptr;
}

bool ModelManagerImpl::LoadModel(const std::string& model_path,
                                 const std::string& model_type,
                                 const std::string& model_name) {
    if (model_path.empty() || model_type.empty() || model_name.empty()) {
        fprintf(stderr, "can not load model<%s> with type<%s> in path<%s>.\n",
                model_name.c_str(), model_type.c_str(), model_path.c_str());
        return false;
    }

    if (models_.find(model_name) != models_.end()) {
        fprintf(stderr, "model<%s> in path<%s> already loaded.\n",
                model_name.c_str(), model_path.c_str());
        return false;
    }

    bool loaded = false;
    engine::fs::Path path(model_path);
    std::unique_ptr<Model> model(
        std::make_unique<Model>(model_path, model_type));
    std::unique_ptr<ModelLoader> loader(CreateLoader(model.get()));
    fprintf(stdout, "begin load model<%s> in path<%s>.\n", model_name.c_str(),
            model_path.c_str());
    std::chrono::time_point<std::chrono::system_clock> start =
        std::chrono::system_clock::now();
    if (loader && loader->Load(path.data())) {
        models_.insert(std::make_pair(model_name, std::move(model)));
        loaded = true;
    }
    std::chrono::time_point<std::chrono::system_clock> end =
        std::chrono::system_clock::now();
    std::chrono::milliseconds ns =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    fprintf(stdout, "fininshed load model<%s> in path<%s> in %lld ms\n",
            model_name.c_str(), model_path.c_str(), ns.count());
    return loaded;
}

bool ModelManagerImpl::LoadModel(xml::Node* xml_node) {
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not load model with no name.\n");
        return false;
    }
    std::string model_name;
    model_name = attribute->value();

    std::string model_path;
    std::string model_type;
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "file") {
                model_path = value;
            } else if (name == "type") {
                model_type = value;
            } else {
                fprintf(stderr, "load model: unknow attribute<%s=%s>\n",
                        name.c_str(), value.c_str());
            }
        }
        attribute = attribute->next_attribute();
    }
    if (model_path.empty() || model_type.empty() || model_name.empty()) {
        fprintf(
            stderr,
            "xml node not completed: model<%s> with type<%s> in path<%s>.\n",
            model_name.c_str(), model_type.c_str(), model_path.c_str());
        return false;
    }
    return LoadModel(model_path, model_type, model_name);
}

bool ModelManagerImpl::LoadModelAsset(const std::string& path) {
    xml::Document doc;
    auto* root(xml::LoadXML(doc, path, "models"));
    if (!root) {
        fprintf(stderr, "ModelManager: not model list xml file.\n");
        return false;
    }
    auto* xml_node = root->first_node();
    while (xml_node) {
        std::string name(xml_node->name());
        if (name == "model") {
            LoadModel(xml_node);
        } else {
            fprintf(stderr, "unknown xml node type: %s\n", name.c_str());
        }
        xml_node = xml_node->next_sibling();
    }

    return true;
}

bool ModelManagerImpl::Initialize() {
    return true;
}

void ModelManagerImpl::UnInitialize() {}

void ModelManagerImpl::Clear() {
    models_.clear();
}

}  // namespace engine