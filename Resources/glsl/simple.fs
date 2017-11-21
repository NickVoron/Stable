#version 400

in vec2 v_uv;
uniform sampler2D ourTexture;

out vec4 frag_colour;


void main(void)
{
	frag_colour = texture(ourTexture, v_uv);
	//frag_colour = vec4(1,1,0,1);
}
