###Standalone Tests
The tests contained within this folder are intended to test the interface of Chimera when there is no
backend driver present for a given feature. In general, Chimera supports a range of functionalities that
may or may not be present on all drivers, such as DMA, FMC, etc. These test will ensure that applications
built on top of Chimera will see a consistent interface even if the backend driver is not fully functional.