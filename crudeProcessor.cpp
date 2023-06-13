#include "VPINetStream.h"
#include <iostream>
#include <zmq.hpp>

int main() 
{
  VPINetStream stream;
  stream.Init("localhost", 9999);

  while (true) {
    FrameData frameData = stream.ReceiveFrame();
    std::cout << "Received frame " << frameData.frameNumber << std::endl;
  }
}