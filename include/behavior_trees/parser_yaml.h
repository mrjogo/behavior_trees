#ifndef PARSER_YAML_H_
#define PARSER_YAML_H_

#include <string>
#include <yaml-cpp/yaml.h>

#include "behavior_trees/node.h"

void walk_bt_yaml(YAML::Node node);

NodeRoot *parse_yaml(std::string param_name);

#endif
