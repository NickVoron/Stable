precision highp float;

uniform vec2 u_size;
varying vec2 v_texCoord;
uniform sampler2D s_baseMap;

void main()
{
	vec4 baseColor = texture2D(s_baseMap, v_texCoord);
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
