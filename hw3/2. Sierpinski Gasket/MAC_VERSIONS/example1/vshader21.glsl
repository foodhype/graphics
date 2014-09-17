attribute vec4 vPosition;

void
main()
{
    gl_Position = vPosition;
    //gl_Position = vec4(vPosition.x*2.0, vPosition.y, vPosition.z, 1.1 );
}
