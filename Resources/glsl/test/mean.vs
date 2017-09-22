precision highp float;

uniform mat4 projection;
attribute vec4 a_position;
attribute vec2 a_texCoord;
varying vec2 v_texCoord;

void main()
{
	gl_Position = projection * a_position;
	v_texCoord = a_texCoord;
}