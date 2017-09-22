precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput;
uniform vec2 isize;
uniform vec4 mean;

void main(void)
{
	vec4 value = texture2D(texinput, v_texCoord * isize) - mean;
	gl_FragColor = vec4(value.x*value.x, value.y*value.y, value.z*value.z, value.w*value.w);
}
