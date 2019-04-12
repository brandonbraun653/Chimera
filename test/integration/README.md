###Integration Tests
The tests contained within this folder are designed in such a way that they can check a backend driver for conformance
to the Chimera interface specification. The idea is that the user will pull these tests in, link and configure Chimera 
to the backend driver using chimeraPort.hpp, and see if they behave correctly. This will not be able to check if the 
driver functions correct internally, only that the interface is the same under common operating conditions.