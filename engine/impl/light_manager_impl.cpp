#include "light_manager_impl.h"

#include <memory>
#include "base/string_util.h"
#include "glm/glm.hpp"
#include "model/light.h"

namespace engine {
Light* LightManagerImpl::GetLight(const std::string& light) {
    if (lights_.find(light) != lights_.end()) {
        return lights_[light].get();
    } else {
        fprintf(stderr, "can not find light<%s>.\n", light.c_str());
        return nullptr;
    }
}

bool LightManagerImpl::LoadLightAsset(const std::string& path) {
    xml::Document doc;
    auto* root(engine::xml::LoadXML(doc, path, "lights"));
    if (!root) {
        fprintf(stderr, "LightManager: not light list xml file.\n");
        return false;
    }
    auto* xml_node = root->first_node();
    while (xml_node) {
        std::string name(xml_node->name());
        if (name == "light") {
            LoadLight(xml_node);
        } else {
            fprintf(stderr, "unknown xml node type: %s\n", name.c_str());
        }
        xml_node = xml_node->next_sibling();
    }
    return true;
}

bool LightManagerImpl::LoadLight(xml::Node* xml_node) {
    // check name
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not load light with no name.\n");
        return false;
    }
    std::string light_name;
    light_name = attribute->value();
    // check exist
    if (lights_.find(light_name) != lights_.end()) {
        fprintf(stderr, "light<%s> already exist.\n", light_name.c_str());
        return false;
    }
    // create light
    std::unique_ptr<Light> light(std::make_unique<Light>(light_name));
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "type") {
                light->SetType(GetLightType(value));
            } else if (name == "pos") {
                light->SetPosition(ReadVec3(value, 0.0f));
            } else if (name == "direction") {
                light->SetDirection(ReadVec3(value, 0.0f));
            } else if (name == "innercut") {
                light->SetInnerCut(glm::cos(glm::radians(std::stof(value))));
            } else if (name == "outercut") {
                light->SetOuterCut(glm::cos(glm::radians(std::stof(value))));
            } else if (name == "constant") {
                light->SetConstant(std::stof(value));
            } else if (name == "linear") {
                light->SetLinear(std::stof(value));
            } else if (name == "quadratic") {
                light->SetQuadratic(std::stof(value));
            } else if (name == "ambient") {
                light->SetAmbient(ReadVec4(value, 1.0f));
            } else if (name == "diffuse") {
                light->SetDiffuse(ReadVec4(value, 1.0f));
            } else if (name == "specular") {
                light->SetSpecular(ReadVec4(value, 1.0f));
            } else {
                fprintf(stderr, "load light: unknow attribute<%s=%s>\n",
                        name.c_str(), value.c_str());
            }
        }
        attribute = attribute->next_attribute();
    }
    lights_.insert(std::make_pair(light->GetName(), std::move(light)));
    return true;
}

}  // namespace engine
