#include "world.h"

#include "application.h"
#include "axis_node.h"
#include "base/string_util.h"
#include "camera_node.h"
#include "light_node.h"
#include "model_node.h"
#include "scene_manager.h"

using namespace engine;

World::World() {}

World::~World() {}

bool World::Load(const std::string& file) {
    engine::xml::Document doc;
    auto* root(engine::xml::LoadXML(doc, file, "world"));
    if (!root)
        return false;
    auto* name_attribute = root->first_attribute("name");
    if (name_attribute) {
        name_ = name_attribute->value();
        fprintf(stdout, "load world<%s>\n", name_.c_str());
    } else {
        fprintf(stderr, "warning, world with no name!!!\n");
    }

    auto* xml_node = root->first_node();
    while (xml_node) {
        std::string name(xml_node->name());
        if (name == "entity") {
            BuildModel(xml_node);
        } else if (name == "axis") {
            BuildAxis(xml_node);
        } else if (name == "light") {
            BuildLight(xml_node);
        } else if (name == "camera") {
            BuildCamera(xml_node);
        } else {
            fprintf(stderr, "unknown world type: %s\n", name.c_str());
        }

        xml_node = xml_node->next_sibling();
    }

    return false;
}

void World::BuildModel(engine::xml::Node* xml_node) {
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not create model with no name.\n");
        return;
    }
    std::unique_ptr<ModelNode> node(
        std::make_unique<ModelNode>(attribute->value()));
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "pos") {
                node->Translate(ReadVec3(value, 0));
            } else if (name == "rotate") {
                glm::vec4 rotate(ReadVec4(value, 1));
                node->Rotate(rotate[3], glm::vec3(rotate));
            } else if (name == "scale") {
                node->Scale(ReadVec3(value, 1));
            } else if (name == "material") {
                node->SetMaterial(value);
            } else if (name == "model") {
                node->SetModel(value);
            } else {
                fprintf(stderr, "unknow attribute<%s=%s>\n", name.c_str(),
                        value.c_str());
            }
        }
        attribute = attribute->next_attribute();
    }
    application->scene_manager()->AddSceneNode(std::move(node));
}

void World::BuildAxis(engine::xml::Node* xml_node) {
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not create axis with no name.\n");
        return;
    }
    std::unique_ptr<AxisNode> node(
        std::make_unique<AxisNode>(attribute->value()));
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "pos") {
                node->Translate(ReadVec3(value, 0));
            } else if (name == "rotate") {
                glm::vec4 rotate(ReadVec4(value, 1));
                node->Rotate(rotate[4], glm::vec3(rotate));
            } else if (name == "scale") {
                node->Scale(ReadVec3(value, 1));
            } else {
                fprintf(stderr, "unknow attribute<%s=%s>\n", name.c_str(),
                        value.c_str());
            }
        }

        attribute = attribute->next_attribute();
    }
    application->scene_manager()->AddSceneNode(std::move(node));
}

void World::BuildLight(engine::xml::Node* xml_node) {
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not create model with no name.\n");
        return;
    }
    std::unique_ptr<LightNode> node(
        std::make_unique<LightNode>(attribute->value()));
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "pos") {
                node->Translate(ReadVec3(value, 0));
            } else if (name == "rotate") {
                glm::vec4 rotate(ReadVec4(value, 1));
                node->Rotate(rotate[3], glm::vec3(rotate));
            } else if (name == "scale") {
                node->Scale(ReadVec3(value, 1));
            } else if (name == "model") {
                node->SetModel(value);
            } else if (name == "material") {
                node->SetMaterial(value);
            } else if (name == "light") {
                node->SetLight(value);
            } else {
                fprintf(stderr, "unknow attribute<%s=%s>\n", name.c_str(),
                        value.c_str());
            }
        }
        attribute = attribute->next_attribute();
    }
    application->scene_manager()->AddSceneNode(std::move(node));
}

void World::BuildCamera(engine::xml::Node* xml_node) {
    auto* attribute = xml_node->first_attribute("name");
    if (!attribute || attribute->value_size() == 0) {
        fprintf(stderr, "can not create camera with no name.\n");
        return;
    }
    std::unique_ptr<CameraNode> node(
        std::make_unique<CameraNode>(attribute->value()));
    // other attributes
    attribute = attribute->next_attribute();
    while (attribute) {
        std::string name(attribute->name());
        std::string value(attribute->value());
        if (!name.empty() && !value.empty()) {
            if (name == "pos") {
                node->Translate(ReadVec3(value, 0));
            } else if (name == "rotate") {
                glm::vec4 rotate(ReadVec4(value, 1));
                node->Rotate(rotate[3], glm::vec3(rotate));
            } else if (name == "scale") {
                node->Scale(ReadVec3(value, 1));
            } else if (name == "model") {
                node->SetModel(value);
            } else if (name == "material") {
                node->SetMaterial(value);
            } else {
                fprintf(stderr, "unknow attribute<%s=%s>\n", name.c_str(),
                        value.c_str());
            }
        }

        attribute = attribute->next_attribute();
    }
    application->scene_manager()->AddSceneNode(std::move(node));
}
