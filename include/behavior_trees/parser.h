#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include <unordered_map>
#include <ros/package.h>

extern std::unordered_map<std::string, double> global_vars;

int process_substring(std::string sub);

int parse_file(std::string param_name);

#endif
