#version 430
uniform vec3 light;
out vec4 fragcolor;           
in vec3 p;
//in vec3 n;
//vec3 l;
//float Ka = 0.5f, Kd = 2.0f;
//vec3 color = vec3(0.6, 0.6, 0.6);
void main(void)
{   
	//l = light - p;
	//float dist = length(l);
	//l = normalize(l);
	//float light = Ka + Kd * dot(l, n) / (dist*dist);
	//fragcolor = vec4(light*color, 1.0);

	vec3 h = p.yyy;
	fragcolor = vec4(h, 1.0);
}




















