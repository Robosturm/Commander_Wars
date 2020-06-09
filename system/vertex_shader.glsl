varying lowp vec4 result_color;
varying mediump vec2 result_uv;
uniform highp mat4 mat;
attribute vec3 position;
attribute vec4 color;
attribute vec2 uv;
void main()
{
	gl_Position = mat * vec4(position, 1.0); 
	result_color = color;
	result_uv = uv;
}