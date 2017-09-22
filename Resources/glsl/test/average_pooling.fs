precision highp float;

varying vec2 v_texCoord;
uniform vec2 isize;
uniform sampler2D texinput;
uniform ivec4 pad;
uniform ivec2 kernel;
uniform vec2 stride;

vec2 coord(vec2 coords, int x, int y)
{
	return ((coords - vec2(0.5, 0.5)) * stride + vec2(0.5, 0.5) + vec2(x, y)) *isize;
}

vec4 get_pixel(vec2 coords, int x, int y)
{
	vec2 sc = coord(coords, x, y);;
	if (sc.x < 0.0 || sc.x > float(pad.z) || sc.y < 0.0 || sc.y > float(pad.w)) return vec4(0.0);
	return texture2D(texinput, sc);
}
				 
void main(void)
{
	vec4 val = vec4(0.0);
	for (int y = -pad.y; y < kernel.y - pad.y; ++y)
	{
		for (int x = -pad.x; x < kernel.x - pad.x; ++x)
		{
			val += get_pixel(v_texCoord, x, y);
		}
	}

	//val += get_pixel(v_texCoord, x, y);

	val /= float(kernel.x * kernel.y);	
	gl_FragColor = val;
}
