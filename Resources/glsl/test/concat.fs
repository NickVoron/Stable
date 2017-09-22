precision highp float;

varying vec2 v_texCoord;
uniform sampler2D texinput;
uniform vec2 isize;
				 
void main(void)
{	
	gl_FragColor = texture2D(texinput, v_texCoord * isize);
}
