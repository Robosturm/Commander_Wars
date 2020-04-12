/* BlurFragmentShader.glsl */
//precision mediump float;
 
mediump uniform sampler2D s_texture;
 
mediump varying vec2 v_texCoord;
 
void main()
{
    gl_FragColor = texture2D(s_texture, v_texCoord);
}