#include <map>
#include <ros/package.h>
#include <iostream>

#include "behavior_trees/parser_yaml.h"

Node *walk_bt_yaml(YAML::Node yaml_node, Node *bt_parent)
{
  Node *bt_node = NULL;
  switch (yaml_node.Type()) {
    case YAML::NodeType::Sequence:
      for (auto it = yaml_node.begin(); it != yaml_node.end(); ++it)
      {
        walk_bt_yaml(*it, bt_parent);
      }
      break;
    case YAML::NodeType::Map:
      for (auto it = yaml_node.begin(); it != yaml_node.end(); ++it)
      {
        if (it->first.Type() != YAML::NodeType::Scalar)
        {
          throw std::runtime_error("YAML behavior tree node type must be string");
        }
        const std::string bt_node_type = it->first.as<std::string>();
        if (bt_node_type == "root")
        {
          bt_node = new NodeRoot();
          walk_bt_yaml(it->second, bt_node);
        } else if (bt_node_type == "sel")
        {
          bt_node = new NodeSelector(bt_parent);
          walk_bt_yaml(it->second, bt_node);
        } else if (bt_node_type == "sel_star")
        {
          bt_node = new NodeSelectorStar(bt_parent);
          walk_bt_yaml(it->second, bt_node);
        } else if (bt_node_type == "seq")
        {
          bt_node = new NodeSequence(bt_parent);
          walk_bt_yaml(it->second, bt_node);
        } else if (bt_node_type == "seq_star")
        {
          bt_node = new NodeSequenceStar(bt_parent);
          walk_bt_yaml(it->second, bt_node);
        } else if (bt_node_type == "par")
        {
          bt_node = new NodeParallel(bt_parent);
          walk_bt_yaml(it->second, bt_node);
        } else if (bt_node_type == "action")
        {
          if (it->second.Type() != YAML::NodeType::Scalar)
          {
            throw std::runtime_error("YAML action name must be string");
          }
          return new NodeROS(bt_parent, it->second.as<std::string>());
        } else if (bt_node_type == "cond")
        {
          throw std::runtime_error("Can't figure out how to support conditions yet.");
          // bt_node = new NodeCondition(bt_parent, )
        } else if (bt_node_type == "dec")
        {
          throw std::runtime_error("Can't figure out how to support decorators yet.");
        } else {
          throw std::runtime_error("Unknown behavior tree node type!");
        }
      }
      break;
    default:
      throw std::runtime_error("Unexpected YAML node type!");
  }
  return bt_node;
}

NodeRoot *parse_yaml(std::string param_name)
{
  std::string line;
  std::string bt_raw;

  if (!ros::param::get(param_name, bt_raw)) { 
    throw std::runtime_error("Couldn't get behavior tree ROS parameter");
  }

  std::cout << "Got parameter " << param_name << std::endl;

  YAML::Node bt = YAML::Load(bt_raw);
  return (NodeRoot *)walk_bt_yaml(bt, NULL);
}
