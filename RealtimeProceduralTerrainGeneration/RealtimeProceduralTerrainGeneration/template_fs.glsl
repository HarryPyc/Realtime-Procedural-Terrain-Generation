#version 430
uniform vec3 light;
uniform float hMin;
uniform float hMax;
uniform bool useLight;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D rock2;
uniform sampler2D snow;
uniform sampler2D mud;

out vec4 fragcolor;           
in vec3 p;
in vec3 normal;
in vec3 uv;

float Ka = 0.3f, Kd = 1.0f;
void main(void)
{   
	float h = (p.y - hMin) / (hMax - hMin);
	if (useLight) {
		vec3 l = -normalize(light);
		vec3 n = normalize(normal);
		float lightColor = Ka + Kd * max(dot(l, n), 0);
		vec4 color;
//		if (h < 0.25f) color = texture(grass, uv.xy);
//		else if (h < 0.6f) color = texture(rock, uv.xy);
//		else if (h < 0.8f) color = texture(rock2, uv.xy);
//		else color = texture(snow, uv.xy);
		vec4 cGrass = texture(grass, uv.xy);
		vec4 cRock = texture(rock, uv.xy);
		vec4 cRock2 = texture(rock2, uv.xy);
		vec4 cSnow = texture(snow, uv.xy);
		vec4 cMud = texture(mud, uv.xy);
		cGrass = mix(cGrass,cMud,0.5);
		if(h<0.3f)
			color = mix(cGrass, cRock, h / 0.3f);
		else if(h<0.6f)
			color = mix(cRock, cRock2, (h-0.3)/0.4);
		else
			color = mix(cRock2, cSnow, (h-0.6)/0.4);
		fragcolor = color*lightColor;

	}
	else {
		fragcolor = vec4(vec3(h), 1.0);
	}
}




















