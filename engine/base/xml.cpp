#include "xml.h"
#include "filesystem.h"

namespace engine {
namespace xml {

Node* LoadXML(Document& doc,
              const std::string& path,
              const std::string& root_name) {
    // load file
    if (path.empty() || !engine::fs::Exist(engine::fs::Path(path))) {
        return nullptr;
    }

    std::string xml(engine::fs::GetFileContent(engine::fs::Path(path)));
    if (xml.empty()) {
        return nullptr;
    }
    // parse xml
    try {
        doc.parse<0>(doc.allocate_string(xml.c_str()));
    } catch (rapidxml::parse_error error) {
        fprintf(stderr, "%s", error.what());
        return nullptr;
    }
    // get root node
    Node* root = doc.first_node(root_name.c_str());
    if (!root || root->name() != root_name) {
        fprintf(stderr, "can not find root node as \"desktop\" in xml: %s",
                path.c_str());
        return nullptr;
    }
    return root;
}

}  // namespace xml
}  // namespace engine
