#ifdef VERTEX_SHADER

in vec3 atbCoord;
in vec2 atbMaskTexCoord;
out vec2 texCoord;
out vec2 maskTexCoord;

void main()
{
    texCoord = vec2(0.5 * atbCoord.xy + 0.5);   
    maskTexCoord = atbMaskTexCoord;
    gl_Position = vec4(atbCoord, 1.0);
}

#elif FRAGMENT_SHADER

in vec2 maskTexCoord;
in vec2 texCoord;

uniform sampler2D uniTexture;
uniform sampler2D uniOriginalImage;
uniform sampler2D uniMask;

uniform vec4 color;

out vec4 frag_colour;

void main(void)
{
#ifdef TEXTURED
	frag_colour = texture(uniOriginalImage, texCoord);
#elif defined(BLEND)
	frag_colour = texture(uniTexture, texCoord) * texture(uniOriginalImage, texCoord);
#else
	frag_colour = vec4(1,0,0,1);
#endif
}

#endif

