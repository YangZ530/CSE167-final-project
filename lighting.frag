#version 120
#extension GL_EXT_gpu_shader4 : enable

varying vec4 position;
varying vec3 normal;

#define MAXLIGHTS 8

void main()
{
   vec3 N = normalize(normal);
   vec3 E = normalize(-position);
   vec3 L, R;
   float att;
   vec4 finalColor;
   vec3 totalLighting;

   for(int i = 0; i < MAXLIGHTS; i++){
		if(0.0 == gl_LightSource[i].position.w){
			att = 1.0;
			L = normalize(gl_LightSource[i].position.xyz);
		}
		else{
			L = gl_LightSource[i].position.xyz - position.xyz
			float dist = length(L);
			L = normalize(L);
			
			att = 1.0 / (gl_LightSource[i].constantAttenuation
						+ gl_LightSource[i].linearAttenuation * dist
						+ gl_LightSource[i].quadraticAttenuation * dist * dist);
		
			if(gl_LightSource[i].spotCutoff <= 90.0){
				float cosine = max(0.0, dot(-L, normalize(gl_LightSource[i].spotDirection)));
				if(cosine < gl_LightSource[i].spotCosCutoff){
					att = 0.0;
				}
				else{
					att = att * pow(cosine, gl_LightSource[i].spotExponent);
				}
			}
		}
		vec3 ambientReflection = vec3(gl_LightSource[i].ambient)
									* vec3(gl_FrontMaterial.ambient);
		
		vec3 diffuseReflection = att * vec3(gl_LightSource[i].diffuse)
										* vec3(gl_FrontMaterial.diffuse)
										* max(0.0, dot(N, L));
		
		vec3 specularReflection;
		if(dot(N, L) < 0.0){
			specularReflection = vec3(0.0, 0.0, 0.0);
		}
		else{
			specularReflection = att * vec3(gl_LightSource[i].specular)
									* vec3(gl_FrontMaterial.specular)
									* pow(max(0.0, dot(reflect(-L, N), E)), gl_FrontMaterial.shininess);
		}
		
		totalLighting = totalLighting + ambientReflection + diffuseReflection + specularReflection;
	}
	
	gl_FragColor = vec4(totalLighting, 1.0);
}