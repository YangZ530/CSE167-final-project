#version 120
  
uniform mat4 depthMVP;
 
void main(){
	gl_Position =  depthMVP * gl_Vertex;
}