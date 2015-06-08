#version 120

varying vec4 position;
varying vec3 normal;
varying vec4 ShadowCoord;

uniform sampler2D shadowMap;
uniform mat4 depthBiasMVP;

void main()
{
    //Transform the normal into world coordinates
    normal = normalize(gl_NormalMatrix * gl_Normal);
    
    //Transform the vertex into world coordinates
    position = gl_ModelViewMatrix * gl_Vertex;
    
    //Set the gl position of this vertex
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	
	ShadowCoord= gl_TextureMatrix[7] * gl_Vertex;
}