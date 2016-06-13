#ifndef PARSER_YAML_H_
#define PARSER_YAML_H_

// #include <vector>
#include <yaml-cpp/yaml.h>
#include <string>

#include "behavior_trees/node.h"

Node *walk_bt_yaml(YAML::Node node);

NodeRoot *parse_yaml(std::string param_name);

#endif
