varying lowp vec4 result_color;
varying mediump vec2 result_uv;
uniform lowp vec4 add_color;
uniform lowp vec4 brightness_color;
uniform lowp sampler2D base_texture;
uniform sampler2D colorTable;
uniform mediump float gamma;

void main()
{
    mediump vec4 color = texture2D(base_texture, result_uv);
    gl_FragColor = texture2D(colorTable, vec2(color.r, 0.0))  * result_color;
    gl_FragColor.a = color.a * result_color.a;
    gl_FragColor = gl_FragColor + add_color * gl_FragColor.a;
    gl_FragColor.rgb = pow(gl_FragColor.rgb, vec3(1.0 / gamma));
    gl_FragColor = gl_FragColor - brightness_color;
}
