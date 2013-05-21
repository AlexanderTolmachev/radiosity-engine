/*!
 *\file main.cpp
 *\brief Application entry point
 */

#include <iostream>
#include <QtCore/QCoreApplication>

#include "inputparameters.h"
#include "sceneloader.h"
#include "radiosityengine.h"

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

  std::cout << "Loading scene..." << std::endl; 

  SceneLoader sceneLoader;
  ScenePointer scene = sceneLoader.loadScene(inputParameters->sceneFilePath);
  if (scene == NULL) {
    std::cout << "Scene loading failed" << std::endl;
    return -1;    
  }

  std::cout << "Loading scene finished" << std::endl; 

  RadiosityEngine radiosityEngine;
  radiosityEngine.setScene(scene);
  radiosityEngine.setImageResolution(inputParameters->xResolution, inputParameters->yResolution);

  std::cout << "Rendering scene..." << std::endl;
  radiosityEngine.renderScene(inputParameters->iterationsNumber, inputParameters->patchSize);
  std::cout << "Rendering scene finished" << std::endl; 

  std::cout << "Saving image to file '" << inputParameters->outputFilePath.toUtf8().constData() << "'" << std::endl; 
  radiosityEngine.saveRenderedImageToFile(inputParameters->outputFilePath);
  std::cout << "Image is saved" << std::endl;

  return 0; 
}

void printUsage() {
  std::cout << "Usage: radiosity-engine.exe --scene=scene.xml --output=image.png --resolution_x=1280 --resolution_y=800 --num_iterations=30 --patch_size=1.0" << std::endl;
}