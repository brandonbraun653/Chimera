## Configuration Setup
-----------------------
This solution is intended to be included as part of a larger project, primarily to cut down on how much effort is required 
to configure and compile Chimera. 


A few configuration settings for include paths and compiler defines are set by a global property sheet. If you open the 
Property Manager, you will see there is a sheet called "CommonProperties" that is shared across all configurations. One of 
these properties imports another property sheet called "ProjectWideConfiguration", whose location is defined at the top 
level solution directory. It is important to note that expansion of the `$(SolutionDir)` macro equals the Startup Project 
solution directory. 

To view the currently configured macros, follow the 
[Microsoft documentation](https://docs.microsoft.com/en-us/cpp/build/reference/common-macros-for-build-commands-and-properties?view=vs-2019).

### Configuration XML Block
---------------------------
File: *CommonProperties.props*
```xml
<ImportGroup Label="PropertySheets">
  <Import Project="$(SolutionDir)ProjectWideConfiguration.props" />
</ImportGroup>
```

## How to Add Configuration Macros
---------------------------------------
https://sites.google.com/site/pinyotae/Home/visual-studio-visual-c/create-user-defined-environment-variables-macros  
https://docs.microsoft.com/en-us/cpp/build/create-reusable-property-configurations?view=vs-2019  
https://stackoverflow.com/questions/4710084/visual-studio-where-to-define-custom-path-macros

### Required Configuration Macros
-------------------------------------

| Parameter                   | Explanation                                            |
|-----------------------------|--------------------------------------------------------|
| `$(ChimeraPortDir)`         | Directory that contains "chimeraPort.hpp"              |
| `$(ChimeraCfgDir)`          | Directory that contains "chimeraConfig.hpp"            |
| `$(ChimeraCompilerDefines)` | Compiler definitions needed to configure the code base |

### Optional Configuration Macros
-------------------------------------
| Parameter            | Explanation                                                     |
|----------------------|-----------------------------------------------------------------|
| `$(FreeRTOSPortDir)` | If FreeRTOS is enabled, points to the location of "portmacro.h" |