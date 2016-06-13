#include "behavior_trees/parser.h"
#include "behavior_trees/node.h"

#include <sstream>

Node *node;
bool action_detected = false;
bool condition_detected = false;
bool variable_detected = false;
bool decorator_detected = false;

std::unordered_map<std::string, double> global_vars;

int process_substring(std::string sub)
{
  bool star_detected = false;
  if (sub.length() == 2)
  {
    if (sub.at(1) == '*')
    {
      star_detected = true;
      sub = sub.at(0);
    }
  }
  switch (*(sub.c_str()))
  {
    case '{':
      std::cout << "Open  Brace" << std::endl;
      if (!star_detected)
        node = new NodeSelector(node);
      else
        node = new NodeSelectorStar(node);
      break;
    case '}':
      std::cout << "Close Brace" << std::endl;
      node = node->get_parent();
      break;
    case '[':
      std::cout << "Open  Bracket" << std::endl;
      if (!star_detected)
        node = new NodeSequence(node);
      else
        node = new NodeSequenceStar(node);
      break;
    case ']':
      std::cout << "Close Bracket" << std::endl;
      node = node->get_parent();
      break;
    case '/':
      std::cout << "Open  Slash" << std::endl;
      node = new NodeParallel(node);
      break;
    case '|':
      std::cout << "Close Slash" << std::endl;
      node = node->get_parent();
      break;
    case 'A':
      std::cout << "ROS Action Detected" << std::endl;
      action_detected = true;
      break;
    case 'C':
      std::cout << "Condition Detected" << std::endl;
      condition_detected = true;
      break;
    case 'V':
      std::cout << "Variable Detected" << std::endl;
      variable_detected = true;
      break;
    case 'd':
      std::cout << "Decorator Open Detected" << std::endl;
      decorator_detected = true;
      break;
    case 'D':
      std::cout << "Decorator Close Detected" << std::endl;
      node = node->get_parent();
      break;

    default:
      break;
  }
  return 0;
}

int parse_file(std::string param_name, Node *root)
{
  std::string line;
  std::string bt_raw;

  node = root;

  if (!ros::param::get(param_name, bt_raw)) { 
    std::cout << "Couldn't Open File" << std::endl;
    return 1;
  }

  std::cout << "Got parameter " << param_name << std::endl;

  std::istringstream bt_data(bt_raw);
  while (bt_data.good())
  {
    getline(bt_data, line);
    std::cout << "Reading line: " << line << std::endl;
    std::istringstream iss(line);
    int i = 0;
    do
    {
      std::string sub;
      iss >> sub;
      std::cout << "Substring: "
                << " i: " << i << " n: " << sub << std::endl;
      process_substring(sub);
      if (action_detected)
      {
        std::string actionname;
        iss >> actionname;
        std::cout << "ROS Action Detected: " << actionname << std::endl;
        node = new NodeROS(node, actionname);
        node = node->get_parent();
        action_detected = false;
      }
      if (condition_detected)
      {
        std::string varlabel;
        std::string relation;
        std::string constant;
        iss >> varlabel;
        iss >> relation;
        iss >> constant;
        std::cout << "Condition Detected: " << varlabel << relation << constant << std::endl;
        node = new NodeCondition(node, varlabel, relation, constant);
        node = node->get_parent();
        condition_detected = false;
      }
      if (variable_detected)
      {
        std::string varlabel;
        std::string initialval;
        iss >> varlabel;
        iss >> initialval;
        std::cout << "Variable Detected: " << varlabel << initialval << std::endl;
        global_vars[varlabel] = std::stod(initialval);
        variable_detected = false;
      }
      if (decorator_detected)
      {
        std::string next_state;
        std::string curr_state;
        std::string prev_status;
        iss >> next_state;
        iss >> curr_state;
        iss >> prev_status;
        std::cout << "Decorator Detected: " << next_state << prev_status << std::endl;
        node = new NodeDecorator(node, next_state, curr_state, prev_status);
        decorator_detected = false;
      }
      i++;
    } while (iss);
  }
  return 0;
}
