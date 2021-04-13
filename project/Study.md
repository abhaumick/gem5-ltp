# Study of Code

## Requests

http://doxygen.gem5.org/release/current/namespaceProbePoints.html

```c++
ProbePoints::PacketInfo
```
seems to have 


### Interesting Functions

```c++
//  Stats Reset
RubySystem::resetStats();

//  Write - 
// The function searches through all the buffers that exist in different
// cache, directory and memory controllers, and in the network components
// and writes the data portion of those that hold the address specified
// in the packet.
RubySystem::functionalWrite(PacketPtr pkt);

// Question - do we put out trace logic here?

```


### ALternatively - Modify `.sm` files

http://www.gem5.org/documentation/learning_gem5/part3/cache-intro/

`MSI-cache.sm`
```C++
in_port(mandatory_in, RubyRequest, mandatoryQueue);

```