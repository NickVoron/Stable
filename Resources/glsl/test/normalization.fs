precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput;
uniform vec2 isize;
uniform vec4 mean;
uniform vec4 stdDev;
uniform float eps;

void main(void)
{
	gl_FragColor = (texture2D(texinput, v_texCoord * isize) - mean) / (stdDev + vec4(eps));
}
