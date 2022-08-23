#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
  
out float vOpacity;
out vec3 vColor;
out vec2 vTexCoord;

uniform float _Opacity;
uniform float _Offset;
uniform vec3 _Color;

void main()
{
    gl_Position = vec4(aPos.x + _Offset, aPos.y, aPos.z, 1.0);
    vOpacity = _Opacity;
    vColor = _Color;
    vTexCoord = aTexCoord;
}       