#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
  
out float vOpacity;

uniform float _Opacity;
uniform float _Offset;

void main()
{
    gl_Position = vec4(aPos.x + _Offset, aPos.y, aPos.z, 1.0);
    vOpacity = _Opacity;
}       