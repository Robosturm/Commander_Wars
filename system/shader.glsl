//shared data between vs/ps
lowp varying vec4 result_color;
mediump varying vec2 result_uv;
mediump varying vec2 result_uv2;

#ifdef VS
highp uniform mat4 mat;
mediump uniform vec3 msk[4];
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
lowp uniform vec4 clip_mask;
lowp uniform vec4 add_color;
lowp uniform sampler2D base_texture;
lowp uniform sampler2D mask_texture;
lowp uniform sampler2D alpha_texture;

void program_main_ps()
{	
	gl_FragColor = texture2D(base_texture, result_uv) * result_color;
}
#endif
