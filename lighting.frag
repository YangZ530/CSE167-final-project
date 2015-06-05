#version 120
#extension GL_EXT_gpu_shader4 : enable

varying vec4 position;
varying vec3 normal;

void main()
{
    //Declare variables for intermediate calculations
    vec4 finalColor;
    vec3 lightDir;
	vec3 halfVector;
    float attenuation, spotEffect;
    float distance;
	vec3 reflection;
	vec3 eye;

    //Declare and initialize variables for storing lighting results
    vec3 finalLighting = vec3(gl_FrontMaterial.emission);
    vec3 ambientReflection = vec3(0.0,0.0,0.0);
    vec3 diffuseReflection = vec3(0.0,0.0,0.0);
    vec3 specularReflection = vec3(0.0,0.0,0.0);
    
    //Loop through all 8 lights that the OpenGL fixed function pipeline supports
//    for (int i = 0; i < 8; ++i)
//    {
        //Calculate the light direction
        lightDir = gl_LightSource[2].position.xyz - position.xyz;
        
		halfVector = vec3(gl_LightSource[2].halfVector);
		halfVector = normalize(halfVector);

		eye = normalize(-vec3(position));
		reflection = normalize(-reflect(lightDir, normal));
        
        //If the light is behind the surface, continue
        //if(dot(normal, lightDir) < 0.0)
            //continue;

	    if(gl_LightSource[2].spotCosCutoff != -1)
		{
			spotEffect = dot(normalize(gl_LightSource[2].spotDirection), normalize(-lightDir));
			if (spotEffect > gl_LightSource[2].spotCosCutoff) {
				spotEffect = pow(spotEffect, gl_LightSource[2].spotExponent);

				//Set the default attenuation to none (by the multiplicative identity, this is 1.0)
				attenuation = 1.0;
       
				//If the light is not a directional light, then apply the appropriate attenuation
				if (0.0 != gl_LightSource[2].position.w)
				{
					distance = length(lightDir);
					attenuation = spotEffect / (gl_LightSource[2].constantAttenuation
											   + gl_LightSource[2].linearAttenuation * distance
											   + gl_LightSource[2].quadraticAttenuation * distance * distance);
				}
				//Add the ambient light factor to the ambient accumulator
				ambientReflection += vec3(gl_FrontMaterial.ambient) * vec3(gl_LightSource[2].ambient);
				//Calculate the diffuse reflectance and add it to the diffuse accumulator
				diffuseReflection += attenuation * vec3(gl_LightSource[2].diffuse) * vec3(gl_FrontMaterial.diffuse) * max(0.0, dot(normal, normalize(lightDir)));
				//specularReflection += attenuation * vec3(gl_LightSource[2].specular) * vec3(gl_FrontMaterial.specular) * pow(max(dot(halfVector, normal),0.0),0.3*gl_FrontMaterial.shininess);
				//if(gl_LightSource[i].position.w != 0)
				specularReflection += attenuation * vec3(gl_LightSource[2].specular) * vec3(gl_FrontMaterial.specular) * pow(max(dot(reflection, eye),0.0),0.3*gl_FrontMaterial.shininess);
			}
			else{
				//diffuseReflection += vec3(0.0,0.0,0.0);
				//specularReflection += vec3(0.0,0.0,0.0);
				//ambientReflection += vec3(0.0,0.0,0.0);
			}
		}
//	}
    
    //Sum all of the lighting together
	finalLighting = finalLighting + ambientReflection + diffuseReflection + specularReflection;
    
    //Return the final lighting
    gl_FragColor = vec4(finalLighting, 1.0);
}