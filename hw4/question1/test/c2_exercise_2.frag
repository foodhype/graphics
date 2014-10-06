varying  vec4 color;

void main()
{
    gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, 1.0);
    // or
    //gl_FragColor = vec4(1.0 - Color.x, 1.0 - Color.y, 1.0 - Color.z, 1.0);
    // or even
    //gl_FragColor = vec4(1.0 - Color, 1.0);
}