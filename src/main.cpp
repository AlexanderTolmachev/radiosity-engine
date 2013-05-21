/*!
 *\file main.cpp
 *\brief Application entry point
 */

#include <iostream>
#include <QtCore/QCoreApplication>

#include "inputparameters.h"

void printUsage();

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  if (app.arguments().size() == 2 && app.arguments().at(1) == "--help") {
    printUsage();
    return 0;
  }

  InputParametersParser paramatersParser;
  InputParametersPointer inputParameters = paramatersParser.parseInputParameters(app.arguments());  
  if (inputParameters == NULL) {
    std::cout << "Invalid arguments passed to application" << std::endl;
    printUsage();
    return -1;
  }

  std::cout << *inputParameters;

  return 0; 
}

void printUsage() {
  std::cout << "Usage: radiosity-engine.exe --scene=scene.xml --output=image.png --resolution_x=1280 --resolution_y=800 --num_iterations=30 --patch_size=1.0" << std::endl;
}