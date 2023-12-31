#include "VPINetStream.h"
#include <iostream>

static std::bitset<256> decodeDescriptor(std::string descriptorStr)
{
  std::bitset<256> bits;
  for (std::size_t i = 0; i < descriptorStr.size(); ++i)
  {
    for (std::size_t j = 0; j < 8; ++j)
    {
      bits[i * 8 + j] = (descriptorStr[i] >> j) & 1;
    }
  }
  return bits;
}

FrameData DecodeKeypoints(std::string encodedKeypoints)
{
  FrameData frameData;

  // Get the frame number
  std::string frameNumberStr = encodedKeypoints.substr(0, encodedKeypoints.find(";"));
  frameData.frameNumber = std::stoi(frameNumberStr);
  encodedKeypoints.erase(0, frameNumberStr.length() + 1);

  // Get the number of keypoints
  std::string numKeypointsStr = encodedKeypoints.substr(0, encodedKeypoints.find(";"));
  frameData.numKeypoints = std::stoi(numKeypointsStr);
  encodedKeypoints.erase(0, numKeypointsStr.length() + 1);

  // Allocate memory for the keypoints and descriptors
  frameData.keypoints = new VPIKeypointF32[frameData.numKeypoints];
  frameData.descriptors = new std::bitset<256>[frameData.numKeypoints];

  // Decode the keypoints and descriptors
  for (int i = 0; i < frameData.numKeypoints; ++i)
  {
    // Decode the descriptor
    std::string descriptorStr = encodedKeypoints.substr(0, 32);
    encodedKeypoints.erase(0, 33);
    std::bitset<256> descriptor = decodeDescriptor(descriptorStr);
    frameData.descriptors[i] = descriptor;

    // Decode the keypoint
    std::string keypointStr = encodedKeypoints.substr(0, encodedKeypoints.find(";"));
    encodedKeypoints.erase(0, keypointStr.length() + 1);
    std::string xStr = keypointStr.substr(0, keypointStr.find(","));
    std::string yStr = keypointStr.substr(keypointStr.find(",") + 1, keypointStr.length());

    frameData.keypoints[i].x = std::stof(xStr);
    frameData.keypoints[i].y = std::stof(yStr);
  }

  return frameData;
}