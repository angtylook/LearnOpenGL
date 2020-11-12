#ifndef SCENE_WORLD_H_
#define SCENE_WORLD_H_

#include <string>

#include "base/xml.h"

class World {
public:
    World();
    ~World();
    bool Load(const std::string& file);

private:
    void BuildModel(engine::xml::Node* xml_node);

    void BuildAxis(engine::xml::Node* xml_node);

    void BuildLight(engine::xml::Node* xml_node);

    void BuildCamera(engine::xml::Node* xml_node);

private:
    std::string name_;
};

#endif  // SCENE_WORLD_H_
