/* VBlurVertexShader.glsl */
attribute  vec3 position;
attribute  vec4 color;
attribute  vec2 uv;
 
uniform mediump float step;

mediump varying vec4 v_color;
mediump varying vec2 v_texCoord;
mediump varying vec2 v_blurTexCoords0;
mediump varying vec2 v_blurTexCoords1;
mediump varying vec2 v_blurTexCoords2;
mediump varying vec2 v_blurTexCoords3;
mediump varying vec2 v_blurTexCoords4;
mediump varying vec2 v_blurTexCoords5;
mediump varying vec2 v_blurTexCoords6;
mediump varying vec2 v_blurTexCoords7;
mediump varying vec2 v_blurTexCoords8;
mediump varying vec2 v_blurTexCoords9;
mediump varying vec2 v_blurTexCoords10;
mediump varying vec2 v_blurTexCoords11;
mediump varying vec2 v_blurTexCoords12;
mediump varying vec2 v_blurTexCoords13;
 
 
void main()
{
    gl_Position = vec4(position.xy, 0, 1.0);
    mediump float z = step;

    v_color = color;
    v_texCoord = uv;
    v_blurTexCoords0 = v_texCoord + vec2(0.0, -z * 7.0);
    v_blurTexCoords1 = v_texCoord + vec2(0.0, -z * 6.0);
    v_blurTexCoords2 = v_texCoord + vec2(0.0, -z * 5.0);
    v_blurTexCoords3 = v_texCoord + vec2(0.0, -z * 4.0);
    v_blurTexCoords4 = v_texCoord + vec2(0.0, -z * 3.0);
    v_blurTexCoords5 = v_texCoord + vec2(0.0, -z * 2.0);
    v_blurTexCoords6 = v_texCoord + vec2(0.0, -z);
    v_blurTexCoords7 = v_texCoord + vec2(0.0,  z);
    v_blurTexCoords8 = v_texCoord + vec2(0.0,  z * 2.0);
    v_blurTexCoords9 = v_texCoord + vec2(0.0,  z * 3.0);
    v_blurTexCoords10 = v_texCoord + vec2(0.0,  z * 4.0);
    v_blurTexCoords11 = v_texCoord + vec2(0.0,  z * 5.0);
    v_blurTexCoords12 = v_texCoord + vec2(0.0,  z * 6.0);
    v_blurTexCoords13 = v_texCoord + vec2(0.0,  z * 7.0);
}