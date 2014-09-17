attribute vec4 vPosition;
attribute float vIndex;
varying vec4 color;
void
main()
{
gl_Position = vPosition;
    color =
        vec4( ( 1.0 + vPosition.x ) / 2.0, ( 1.0 + vPosition.y ) / 2.0,
              ( 1.0 + vPosition.z ) / 2.0, 1.0 );
             // if (vIndex%2.0==0) {
             // color =vec4( 1.0, 0.0, 0.0, 1.0 );
             // }else {
             // color =vec4( 0.0, 1.0, 0.0, 1.0 );
             // }
}
