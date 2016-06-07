#ifndef ROBOT_CONFIG_H_
#define ROBOT_CONFIG_H_

#include <boost/program_options.hpp>

namespace po = boost::program_options;

void setupCmdLineReader();
void addCmdLineOption(std::string argumentName);
std::string readCmdLineOption(std::string argumentName);

#endif
