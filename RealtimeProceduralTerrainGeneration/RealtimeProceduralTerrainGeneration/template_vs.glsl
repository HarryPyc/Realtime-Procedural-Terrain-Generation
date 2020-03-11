#version 430            
uniform mat4 PVM;

uniform mat4 M;
layout(location = 0)in vec3 pos_attrib;
layout(location = 1)in vec3 normal_attrib;

out vec3 p;
out vec3 normal;

void main(void)
{
   gl_Position = PVM*vec4(pos_attrib, 1.0);
   normal = vec3(M*vec4(normal_attrib, 0));
   p = vec3(M*vec4(pos_attrib, 1.0));
}