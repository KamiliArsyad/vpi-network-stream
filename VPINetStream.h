#include <vpi/Array.h>
#include <vpi/Stream.h>

#include <bitset>
#include <string>

#include <zmq.hpp>

/**
 * Struct to hold the data of a single frame.
 */
struct FrameData
{
  int frameNumber;
  int numKeypoints; 
  std::bitset<256> *descriptors;
  VPIKeypointF32 *keypoints; 
};

/**
 * Function to encode the keypoints and their respective descriptors of a frame
 * into a single string. The format is as follows:
 * <numKeypoints>;<desc1>;<x1>,<y1>;<desc2>;<x2>,<y2>;...
 * The descriptors are encoded as 32 characters ASCII strings for efficiency.
 */
std::string EncodeKeypoints(VPIArray keypointsArray, VPIArray descriptorsArray, int numKeypoints);


/**
 * Function to decode the keypoints and their respective descriptors from an encoded string
 * into a FrameData struct.
*/
FrameData DecodeKeypoints(std::string encodedKeypoints);


/**
 * Object to handle all the streaming operations.
 */
class VPINetStream
{
  private:
    zmq::context_t context;
    zmq::socket_t socket;
    int port = 0;
    std::string ip;
  public:
    VPINetStream();
    ~VPINetStream();


    /**
     * Initialize an edge/node/worker stream.
     * @param port The port to listen to.
     * @return 0 if successful, -1 otherwise.
     */
    int Init(int port);

    /**
     * Initialize a cloud/processing stream.
     * @param ip The ip to connect to.
     * @param port The port to connect to.
     * @return 0 if successful, -1 otherwise.
     */
    int Init(std::string ip, int port);

    /**
     * Send an encoded frame. 
     * @param encodedFrame The encoded frame to send.
     * @return 0 if successful, -1 otherwise.
     */
    int SendFrame(std::string encodedFrame);

    /**
     * Function to receive a frame.
     * @return The received frame as FrameData struct.
     */
    FrameData ReceiveFrame();
};