# VPI Network Stream
This is a simple high performance one-to-one network stream code for streaming extracted ORB features from each frame collected by the client to the server. The abstraction relies on ZeroMQ for the network communication and is written in C++.

The project is still under development and is not ready for use.

main.cpp is the initial client code for testing the network stream. The asynchronous sending message property is not
yet utilized. Such utilization will be the next step whereby we create a separate thread in parallel with the main
client thread to process the message asynchronously.

I've done some testing beforehand and found that allocating/deallocating memory for an array takes an insignificant
amount of time. Therefore, one feasible implementation idea for later is to just allocate a new array for each frame
instead of reusing the same array and locking it over and over again. Although it might also be better to somehow reuse the array (or use two arrays?) as we might queue up the stream and the client might not be able to catch up with the stream. This is a problem that needs to be addressed later.