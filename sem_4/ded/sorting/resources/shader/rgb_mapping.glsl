uniform sampler2D texture;

uniform float red[255];
uniform float green[255];
uniform float blue[255];

uniform float max_rgb;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    pixel.x = red[int(pixel.x * max_rgb)];
    pixel.y = green[int(pixel.y * max_rgb)];
    pixel.z = blue[int(pixel.z * max_rgb)];

    gl_FragColor = pixel;
}
