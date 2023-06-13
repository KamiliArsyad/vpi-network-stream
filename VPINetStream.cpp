#include <VPINetStream.h>

VPINetStream::VPINetStream()
{
  context = zmq::context_t(1);
}

VPINetStream::~VPINetStream()
{
  socket.close();
}

/**
 * Quick note regarding both Init functions: You might notice that Init() can be called however many times
 * you want, this is because I've yet to thought of a way to make a proper C++ class that has a nice
 * factory method or perhaps different subclasses; am not used with OOP with C++ yet and I don't intend
 * to spend too much time on this.
 */

int VPINetStream::Init(int port)
{
  assert(this->port == 0); // Make sure the port hasn't been set yet (can't reinitialize)
  this->port = port;
  socket = zmq::socket_t(context, zmq::socket_type::rep);
  socket.bind("tcp://*:" + std::to_string(port));
  return 0;
}

int VPINetStream::Init(std::string ip, int port)
{
  assert(this->port == 0); // Make sure the port hasn't been set yet (can't reinitialize)
  this->ip = ip;
  this->port = port;
  socket = zmq::socket_t(context, zmq::socket_type::req);
  socket.connect("tcp://" + ip + ":" + std::to_string(port));
  return 0;
}

int VPINetStream::SendFrame(std::string encodedFrame)
{
  zmq::message_t request(encodedFrame.size());
  memcpy(request.data(), encodedFrame.c_str(), encodedFrame.size());

  try
  {
    socket.send(request, zmq::send_flags::none);
  }
  catch (zmq::error_t e)
  {
    std::cout << "Error sending frame: " << e.what() << std::endl;
    return -1;
  }
}

FrameData VPINetStream::ReceiveFrame()
{
  zmq::message_t reply;
  try
  {
    socket.recv(reply, zmq::recv_flags::none);
  }
  catch (zmq::error_t e)
  {
    std::cout << "Error receiving frame: " << e.what() << std::endl;
    FrameData frameData;
    frameData.frameNumber = -1;
    return frameData;
  }

  std::string encodedFrame = std::string(static_cast<char*>(reply.data()), reply.size());
  FrameData frameData = DecodeKeypoints(encodedFrame);
  return frameData;
}

