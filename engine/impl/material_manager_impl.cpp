#include "material_manager_impl.h"

#include "base/string_util.h"
#include "model/material.h"

namespace engine {
Material* MaterialManagerImpl::GetMaterial(const std::string& material) {
    if (materials_.find(material) != materials_.end()) {
        return materials_[material].get();
    } else {
        fprintf(stderr, "can not find material<%s>.\n", material.c_str());
        return nullptr;
    }
}

bool MaterialManagerImpl::AddMaterial(const std::string& material_name,
                                      std::unique_ptr<Material> material) {
    if (!material)
        return false;

    if (materials_.find(material_name) == materials_.end()) {
        materials_.insert(std::make_pair(material_name, std::move(material)));
        return true;
    } else {
        fprintf(stderr, "can not add material<%s>, name collison.\n",
                material_name.c_str());
        return false;
    }
}

bool MaterialManagerImpl::LoadMaterialAsset(const std::string& path) {
    xml::Document doc;
    auto* root(xml::LoadXML(doc, path, "materials"));
    if (!root) {
        fprintf(stderr, "MaterialManager: not material list xml file.\n");
        return false;
    }
    auto* xml_node = root->first_node();
    while (xml_node) {
        std::string name(xml_node->name());
        if (name == "material") {
            LoadMaterial(xml_node);
        } else {
            fprintf(stderr, "unknown xml node type: %s\n", name.c_str());
        }
        xml_node = xml_node->next_sibling();
    }
    return true;
}

bool MaterialManagerImpl::LoadMaterial(engine::xml::Node* xml_node) {
    // check name
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not load light with no name.\n");
        return false;
    }
    std::string material_name;
    material_name = attribute->value();
    // check exist
    if (materials_.find(material_name) != materials_.end()) {
        fprintf(stderr, "material<%s> already exist.\n", material_name.c_str());
        return false;
    }
    // create light
    std::unique_ptr<Material> material(
        std::make_unique<Material>(material_name));
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "ambient") {
                material->SetAmbient(value);
            } else if (name == "ambientcolor") {
                material->SetAmbientColor(ReadVec4(value, 0.0f));
            } else if (name == "diffuse") {
                material->SetDiffuse(value);
            } else if (name == "diffusecolor") {
                material->SetDiffuseColor(ReadVec4(value, 1.0f));
            } else if (name == "specular") {
                material->SetSpecular(value);
            } else if (name == "specularcolor") {
                material->SetSpecularColor(ReadVec4(value, 1.0f));
            } else if (name == "shininess") {
                material->SetShininess(std::stof(value));
            } else {
                fprintf(stderr, "load material: unknow attribute<%s=%s>\n",
                        name.c_str(), value.c_str());
            }
        }
        attribute = attribute->next_attribute();
    }
    materials_.insert(std::make_pair(material_name, std::move(material)));
    return true;
}

}  // namespace engine
