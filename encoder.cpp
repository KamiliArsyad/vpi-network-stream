#include <VPINetStream.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>

#include <zmq.hpp>

#include <vpi/Array.h>
#include <vpi/Stream.h>

std::string EncodeKeypoints(VPIArray keypointsArray, VPIArray descriptorsArray, int numKeypoints)
{
  std::ostringstream ss;
  ss << numKeypoints << ";";

  // Lock the arrays to access their data
  VPIArrayData keypointsData, descriptorsData;
  vpiArrayLockData(keypointsArray, VPI_LOCK_READ, VPI_ARRAY_BUFFER_HOST_AOS, &keypointsData);
  vpiArrayLockData(descriptorsArray, VPI_LOCK_READ, VPI_ARRAY_BUFFER_HOST_AOS, &descriptorsData);
  vpiArrayUnlock(keypointsArray);
  vpiArrayUnlock(descriptorsArray);

  VPIKeypointF32 *keypoints = (VPIKeypointF32 *)keypointsData.buffer.aos.data;

  // Encode the keypoints and descriptors
  for (int i = 0; i < numKeypoints; ++i)
  {
    // Encode the descriptor to 32 characters
    std::bitset<256> descriptor(((uint16_t *)descriptorsData.buffer.aos.data)[i]);
    for (int j = 0; j < 32; ++j)
    {
      unsigned long byte = descriptor.to_ulong() >> j & 0xFF;
      char c = static_cast<char>(byte);
      ss << c;
    }
    ss << ";";

    // Encode the keypoint
    ss << keypoints[i].x << "," << keypoints[i].y << ";";
  }

  return ss.str();
}