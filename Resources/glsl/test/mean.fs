precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput;
uniform vec2 isize;

vec2 coord(vec2 coords, int x, int y)
{
	return ((coords - vec2(0.5, 0.5)) * 2.0 + vec2(0.5, 0.5) + vec2(x, y)) * isize;
}

vec4 get_pixel(vec2 coords, int x, int y)
{
	return texture2D(texinput, coord(coords, x, y));
}

void main(void)
{
	vec4 p00 = get_pixel(v_texCoord, 0, 0);
	vec4 p10 = get_pixel(v_texCoord, 1, 0);
	vec4 p01 = get_pixel(v_texCoord, 0, 1);
	vec4 p11 = get_pixel(v_texCoord, 1, 1);
	
	gl_FragColor = (p00 + p10 + p01 + p11) * 0.25;
}
