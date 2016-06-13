#include <iostream>
#include <unistd.h>
#include <signal.h>

#include "behavior_trees/parameters.h"
#include "behavior_trees/node.h"
#include "behavior_trees/parser_yaml.h"
#include "behavior_trees/display.h"
#include "behavior_trees/keystroke.h"
#include "behavior_trees/navigation.h"
#include "behavior_trees/robot_config.h"

NodeRoot *root;             // the root of the bt
Node *node_cursor = NULL;  // used for displaying bt

void signal_callback_handler(int signum)
{
  printf("Caught signal %d\n", signum);
  // Cleanup and close up stuff here
  // Terminate program
  exit(signum);
}

int main(int argc, char **argv)
{
  std::cout << "Hello, world!" << std::endl;

  // setup signal interrupt handler
  signal(SIGINT, signal_callback_handler);

  // specify which options are available as cmd line arguments
  setupCmdLineReader();

  // initialize the behavior tree client node
  ros::init(argc, argv, std::string("behavior_tree"));
  // Parse behavior tree
  root = parse_yaml("/behavior_tree");

  // initialize OpenGL engine for visualization
  glut_setup(argc, argv);

  // point to the root of the behavior tree
  node_cursor = root;
  node_cursor->set_highlighted(true);

  // print the data parsed from the specification file
  std::cout << "----------------- PRINT TREE -----------------" << std::endl;
  root->print_subtree();

  // start ticking the root of the tree at frequency: TICK_FREQUENCY
  while (ros::ok())
  {
    std::cout << "-------------- EXECUTE TREE --------------" << std::endl;
    root->execute();    // sending tick
    get_keypressed();  // processing keystrokes
    process_keypressed();
    glut_process();  // update visualization
    glutPostRedisplay();
    root->execute_reset_status();
    ros::Duration(1.0 / TICK_FREQUENCY).sleep();
  }

  // missing to clear the dynamically allocated tree
  // delete_tree();

  return 0;
}
