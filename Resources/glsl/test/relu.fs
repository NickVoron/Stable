precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput;
uniform float alpha;
uniform vec2 isize;

				 
void main(void)
{
	vec4 val = texture2D(texinput, v_texCoord * isize);
	gl_FragColor = max(alpha * val, val);
}
