/* BlurFragmentShader.glsl */
//precision mediump float;
 
uniform lowp sampler2D s_texture;
 
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
    const mediump float M = 3.0;
    gl_FragColor = vec4(0.0);
    gl_FragColor += texture2D(s_texture, v_blurTexCoords0)*0.0044299121055113265 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords1)*0.00895781211794 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords2)*0.0215963866053 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords3)*0.0443683338718 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords4)*0.0776744219933 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords5)*0.115876621105 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords6)*0.147308056121 * M;
    gl_FragColor += texture2D(s_texture, v_texCoord      )*0.159576912161 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords7)*0.147308056121 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords8)*0.115876621105 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords9)*0.0776744219933 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords10)*0.0443683338718 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords11)*0.0215963866053 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords12)*0.00895781211794 * M;
    gl_FragColor += texture2D(s_texture, v_blurTexCoords13)*0.0044299121055113265 * M;
    gl_FragColor = v_color * min(gl_FragColor.a, 1.0);
}