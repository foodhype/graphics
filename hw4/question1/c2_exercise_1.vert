attribute vec4 vPosition;
attribute vec4 vColor;

varying vec4 color;

void main()
{
    color = vColor;
    gl_Position = vec4(vPosition.x, -vPosition.y, 0.0, 1.0);
}