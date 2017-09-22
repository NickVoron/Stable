#version 400

in vec2 v_uv;
out vec4 frag_colour;

void main(void)
{
	frag_colour = vec4(v_uv, 1.0, 1.0);
}
