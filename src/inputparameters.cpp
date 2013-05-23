/*!
 *\file inputparameters.cpp
 *\brief Contains InputParametersParser classes definition
 */

#include <iostream>

#include "inputparameters.h"

std::ostream& operator<< (std::ostream &out, const InputParameters &parameters) {
  out << "Scene file: " << parameters.sceneFilePath.toUtf8().constData() << std::endl;
  out << "Output file: " << parameters.outputFilePath.toUtf8().constData() << std::endl;
  out << "Image width: " << parameters.xResolution << std::endl;
  out << "Image height: " << parameters.yResolution << std::endl;
  out << "Iterations number: " << parameters.iterationsNumber << std::endl;
  out << "Patch size: " << parameters.patchSize << std::endl;
  out << "Sample points number per patch: " << parameters.samplePointsNumberPerPatch << std::endl;
  return out;
}

InputParametersParser::InputParametersParser() 
  : mSceneArgumentRegex("--scene=(\\S+)"),
    mOutputArgumentRegex("--output=(\\S+)"),
    mXResolutionArgumentRegex("--resolution_x=(\\d+)"),
    mYResolutionArgumentRegex("--resolution_y=(\\d+)"),
    mIterationsNumberArgumentRegex("--num_iterations=(\\d+)"),
    mPatchSizeArgumentRegex("--patch_size=(\\d+(\\.\\d+)?$)"),
    mSamplePointsNumberPerPatchArgumentRegex("--num_points_per_patch=(\\d+)") {
}

InputParametersParser::~InputParametersParser() {
}

InputParametersPointer InputParametersParser::parseInputParameters(QStringList args) const {
  InputParametersPointer inputParameters = InputParametersPointer(new InputParameters());
  bool isSceneParameterInitialized = false;
  bool isOutputParameterInitialized = false;
  bool isXResolutionParameterInitialized = false;
  bool isYResolutionParameterInitialized = false;
  bool isIterationsNumberParameterInitialized = false;
  bool isPatchSizeParameterInitialized = false;
  bool isSamplePointsNumberPerPatchParameterInitialized = false;

  for (int i = 1; i < args.size(); ++i) {
    if (mSceneArgumentRegex.indexIn(args.at(i)) != -1) {
      if (isSceneParameterInitialized) {
        std::cerr << "Input arguments parse error: 'scene' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->sceneFilePath = mSceneArgumentRegex.cap(1);
      isSceneParameterInitialized = true;
    } else if (mOutputArgumentRegex.indexIn(args.at(i)) != -1) {
      if (isOutputParameterInitialized) {
        std::cerr << "Input arguments parse error: 'output' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->outputFilePath = mOutputArgumentRegex.cap(1);
      isOutputParameterInitialized = true;
    } else if (mXResolutionArgumentRegex.indexIn(args.at(i)) != -1) {   
      if (isXResolutionParameterInitialized) {
        std::cerr << "Input arguments parse error: 'resolution_x' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->xResolution = mXResolutionArgumentRegex.cap(1).toInt();
      isXResolutionParameterInitialized = true;
    } else if (mYResolutionArgumentRegex.indexIn(args.at(i)) != -1) {
      if (isYResolutionParameterInitialized) {
        std::cerr << "Input arguments parse error: 'resolution_y' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->yResolution = mYResolutionArgumentRegex.cap(1).toInt();
      isYResolutionParameterInitialized = true;
    } else if (mIterationsNumberArgumentRegex.indexIn(args.at(i)) != -1) {
      if (isIterationsNumberParameterInitialized) {
        std::cerr << "Input arguments parse error: 'num_iterations' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->iterationsNumber = mIterationsNumberArgumentRegex.cap(1).toInt();
      isIterationsNumberParameterInitialized = true;
    } else if (mPatchSizeArgumentRegex.indexIn(args.at(i)) != -1) {
      if (isPatchSizeParameterInitialized) {
        std::cerr << "Input arguments parse error: 'patch_size' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->patchSize = mPatchSizeArgumentRegex.cap(1).toFloat();
      isPatchSizeParameterInitialized = true;    
    } else if (mSamplePointsNumberPerPatchArgumentRegex.indexIn(args.at(i)) != -1) {
      if (isSamplePointsNumberPerPatchParameterInitialized) {
        std::cerr << "Input arguments parse error: 'num_points_per_patch' argument occurred twice" << std::endl;
        return InputParametersPointer(NULL);
      }
      inputParameters->samplePointsNumberPerPatch = mSamplePointsNumberPerPatchArgumentRegex.cap(1).toInt();
      isSamplePointsNumberPerPatchParameterInitialized = true;
    } else {
      std::cerr << "Input arguments parse error: unknown argument " << args.at(i).toUtf8().constData() << std::endl;
      return InputParametersPointer(NULL);
    }
  }

  if (!isSceneParameterInitialized) {
    std::cerr << "Input arguments parse error: 'scene' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  } 
  if (!isOutputParameterInitialized) {
    std::cerr << "Input arguments parse error: 'output' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  } 
  if (!isXResolutionParameterInitialized) {
    std::cerr << "Input arguments parse error: 'resolution_x' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  } 
  if (!isYResolutionParameterInitialized) {
    std::cerr << "Input arguments parse error: 'resolution_y' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  }
  if (!isIterationsNumberParameterInitialized) {
    std::cerr << "Input arguments parse error: 'num_iterations' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  }
  if (!isPatchSizeParameterInitialized) {
    std::cerr << "Input arguments parse error: 'patch_size' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  }
  if (!isSamplePointsNumberPerPatchParameterInitialized) {
    std::cerr << "Input arguments parse error: 'num_points_per_patch' argument is not specified" << std::endl;
    return InputParametersPointer(NULL);
  }

  return inputParameters;
}