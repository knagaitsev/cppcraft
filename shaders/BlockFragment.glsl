#version 330
in vec2 Texcoord;
in vec3 Normal;
out vec4 outColor;
uniform sampler2D tex;
void main()
{
	vec4 color = texture(tex, Texcoord);
	if (color.a == 0) {
        discard;
    }

	vec3 light = normalize(vec3(-1.0, -1.0, -1.0));
	float nDot = dot(Normal, light);
	float brightness = max(nDot, 0.0) * 0.5 + 0.5;
	if (color.a < 1.0) {
		brightness = 1.0;
	}

    outColor = color * brightness;
}
