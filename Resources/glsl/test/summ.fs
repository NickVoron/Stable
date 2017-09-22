precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput[8];
uniform int count;
				 
void main(void)
{
	vec4 val = vec4(0.0);

	for (int i = 0; i < count; ++i)
	{
		val += texture2D(texinput[i], v_texCoord);
	}			   
	
	gl_FragColor = val;
}
