#version 430
uniform vec3 light;
uniform float hMin;
uniform float hMax;
uniform bool useLight;
out vec4 fragcolor;           
in vec3 p;
in vec3 normal;

float Ka = 0.5f, Kd = 2.0f;
vec3 color = vec3(0.6, 0.6, 0.6);
void main(void)
{   
	if (useLight) {
		vec3 l = light - p;
		vec3 n = normalize(normal);
		float dist = length(l);
		l = normalize(l);
		float light = Ka + Kd * max(dot(l, n), 0) / (dist*dist);
		fragcolor = vec4(light*color, 1.0);
	}
	else {
		float color = (p.y - hMin) / (hMax - hMin);
		fragcolor = vec4(vec3(color), 1.0);
	}
}




















