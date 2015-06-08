#version 120

void main(){
    // Not really needed, OpenGL does it anyway
    gl_FragDepth = gl_FragCoord.z;
}