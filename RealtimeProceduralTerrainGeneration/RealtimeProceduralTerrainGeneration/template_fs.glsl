#version 430
uniform vec3 light;
uniform float hMin;
uniform float hMax;
uniform bool useLight;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D rock2;
uniform sampler2D snow;

out vec4 fragcolor;           
in vec3 p;
in vec3 normal;
in vec3 uv;

float Ka = 0.5f, Kd = 2.0f;
void main(void)
{   
	float h = (p.y - hMin) / (hMax - hMin);
	if (useLight) {
		vec3 l = light - p;
		vec3 n = normalize(normal);
		float dist = length(l);
		l = normalize(l);
		float light = Ka + Kd * max(dot(l, n), 0) / (dist*dist);
		vec4 color;
		if (h < 0.25f) color = texture(grass, uv.xy);
		else if (h < 0.6f) color = texture(rock, uv.xy);
		else if (h < 0.8f) color = texture(rock2, uv.xy);
		else color = texture(snow, uv.xy);

		fragcolor = vec4(light*color.xyz, 1.0);

	}
	else {
		fragcolor = vec4(vec3(h), 1.0);
	}
}




















