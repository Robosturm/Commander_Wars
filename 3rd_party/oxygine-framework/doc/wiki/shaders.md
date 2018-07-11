#Shaders
Default Oxygine shading is based on "Uber Shaders" system.

Shader source is located here:
*oxygine/system_data/original/system/shader.glsl*

It is already embedded into Oxygine source code and can't be modified directly.
If you want to change it then copy shader to Application data folder (working directory).

##Example

*examples/Demo/src/TestUserShader.h* has example how oxygine rendering system could be extended.

*examples/Demo/src/TestUserShader2.h* has example with custom shaders using lightning and normal mapping.