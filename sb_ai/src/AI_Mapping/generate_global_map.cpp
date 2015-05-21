#include "generate_vision_map.hpp"
#include "math.h"

GenerateGlobalMap::GenerateGlobalMap(){

  _imageSubscriber = _n.subscribe("vision_node", 1000, LocalMapSubscriberCallback);

}

GenerateGlobalMap::~GenerateGlobalMap(){

}

void GenerateGlobalMap::TransformLocalToGlobal(){

  // Loop through the vision map
  for(int index = 0; index < _visionMapSize; index++) {

    // If there is an obstacle, then update the global map
    if(_imageMsg.data[index] == 1) {

      xGlobalVisionCoord = cos(_poseMsg.angle) * ConvertIndexToXCoord(index) - sin(_poseMsg.angle) * ConvertIndexToYCoord(index) +            _globalMap.info.origin.position.x;
      yGlobalVisionCoord = sin(_poseMsg.angle) * ConvertIndexToXCoord(index) + cos(_poseMsg.angle) * ConvertIndexToYCoord(index) + _globalMap.info.origin.position.y;

    }

    // Update global map with 1 to show that an obstacle exists
    _globalMap.data[ConvertXYCoordToIndex(xGlobalVisionCoord, yGlobalVisionCoord, _globalMap.data.info.width)] = 1;

  }

}

uint8_t GenerateGlobalMap::ConvertIndexToXCoord(uint8_t index){

   return index % _imageMsg->width;

}

uint8_t GenerateGlobalMap::ConvertIndexToXCoord(uint8_t index){

   return index / _imageMsg->width;

}

uint8_t GenerateGlobalMap::ConvertXYCoordToIndex(uint8_t x, uint8_t y, uint8_t width) {

  return y * width + x;

}

void GenerateGlobalMap::LocalMapSubscriberCallback(const sensor_msgs::Image::ConstPtr& imageMsg){

  _imageMsg.height = imageMsg->height; // Number of rows
  _imageMsg.width = imageMsg->width; // Number of columns
  _imageMsg.step = imageMsg->step;
  _mapSize = imageMsg->step * imageMsg->height;

  for(int index = 0; index < _mapSize; index++){

    _imageMsg.data[index] = imageMsg.data[index];

  }

}

void GenerateGlobalMap::CompassSubscriberCallback(const int angle){
  
  _poseMsg.theta = angle; // probably isn't the angle of the robot w.r.t to the global map, need to fix this

}

void GenerateGlobalMap::testDoSomething() {

  printf("Did something");

}
