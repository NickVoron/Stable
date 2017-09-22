const int MAX_KERNEL_SIZE = 5;
const int MAX_CHANNELS = 40;
const int MAX_FILTERS_COUNT = 32;
const int MAX_TEXTURES = MAX_CHANNELS / 4;

precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput[MAX_TEXTURES];
uniform int texcount;

uniform ivec4 kernelSize;
uniform float kernel[MAX_KERNEL_SIZE * MAX_KERNEL_SIZE * MAX_CHANNELS];
uniform vec2 isize;
uniform vec4 scale;
uniform float bias[MAX_FILTERS_COUNT];
uniform vec2 stride;


vec2 coord(vec2 coords, int x, int y)
{
	return ((coords - vec2(0.5, 0.5)) * stride + vec2(0.5, 0.5) + vec2(x, y)) *isize;
}

float get_pixel(vec2 coords, int x, int y)
{
	float res = 0.0;
	for (int tex = 0; tex < texcount; ++tex)
	{		
		vec4 val = texture2D(texinput[tex], coord(coords, x, y)) * kernel[x + kernelSize.x * y];
		res += val.x + val.y + val.z + val.w;
	}
	return res;
}
			 
void main(void)
{
	float res = 0.0;

	for (int tex = 0; tex < texcount; ++tex)
	{
		for (int y = 0; y < kernelSize.y; ++y)
		{
			for (int x = 0; x < kernelSize.x; ++x)
			{
				res += get_pixel(v_texCoord, x, y);
			}
		}
	}
									   
	res += bias[kernelSize.w];
	gl_FragColor = vec4(res);
}
