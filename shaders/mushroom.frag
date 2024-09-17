uniform sampler2D sheet;
uniform vec4 rect1;
uniform vec4 rect2;

void main() {
    vec2 texCoord = gl_TexCoord[0].xy;
    vec2 texCoord1 = rect1.xy + texCoord * rect1.zw;
    vec2 texCoord2 = rect2.xy + texCoord * rect2.zw;

    vec4 color1 = texture2D(sheet, texCoord1);
    vec4 color2 = texture2D(sheet, texCoord2);
    // gl_FragColor = color1 * color2;
    gl_FragColor = gl_Color;
}
