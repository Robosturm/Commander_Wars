//shared data between vs/ps
varying lowp vec4 result_color;
varying mediump vec2 result_uv;
varying mediump vec2 result_uv2;

#ifdef VS
uniform highp mat4 mat;
uniform mediump vec3 msk[4];
attribute vec3 position;
attribute vec4 color;
attribute vec2 uv;
attribute vec2 uv2;

void program_main_vs()
{
	gl_Position = mat * vec4(position, 1.0); 
	result_color = color;
	result_uv = uv;
}
#endif

#ifdef PS
uniform lowp vec4 clip_mask;
uniform lowp vec4 add_color;
uniform lowp sampler2D base_texture;
uniform lowp sampler2D mask_texture;
uniform lowp sampler2D alpha_texture;

void program_main_ps()
{	
	gl_FragColor = texture2D(base_texture, result_uv) * result_color;
	gl_FragColor = gl_FragColor + add_color * gl_FragColor.a;
}
#endif
