uniform sampler2D texture;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec4 color = vec4(1.0 - pixel.x, 1.0 - pixel.y, 1.0 - pixel.z, pixel.w);

    gl_FragColor = color;
}
