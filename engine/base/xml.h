#ifndef BASE_XML_H_
#define BASE_XML_H_

#include <string>
#include "rapidxml/rapidxml.hpp"

namespace engine {
namespace xml {
using Node = rapidxml::xml_node<char>;
using Document = rapidxml::xml_document<char>;
using Attribute = rapidxml::xml_attribute<char>;

Node* LoadXML(Document& doc,
              const std::string& path,
              const std::string& root_name);
}  // namespace xml
}  // namespace engine

#endif  // BASE_XML_H_
