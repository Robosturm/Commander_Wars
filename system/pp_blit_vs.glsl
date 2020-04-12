/* VBlurVertexShader.glsl */
attribute  vec3 position;
attribute  vec4 color;
attribute  vec2 uv;
 
mediump uniform float step;

mediump varying vec2 v_texCoord;
mediump varying vec4 v_color;
 
void main()
{
    gl_Position = vec4(position.xyz, 1.0);
    v_texCoord = uv;
    v_color = color;
}