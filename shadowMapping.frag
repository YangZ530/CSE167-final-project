#version 120
#extension GL_EXT_gpu_shader4 : enable

varying vec4 position;
varying vec3 normal;
varying vec4 ShadowCoord;

uniform sampler2D ShadowMap;
uniform sampler2D Texture;

#define MAXLIGHTS 8
#define OUTER_ANGLE 0.9397

void main()
{
	vec3 N = normalize(normal);
	vec3 E = normalize(-position).xyz;
	vec3 L, R;
	float att;
	vec3 totalLighting;
	float visibility = 1.0;
	float shadow;
	
	vec3 texColor = texture2D(Texture, gl_TexCoord[0].xy / gl_TexCoord[0].w).rgb;
	
	vec2 poissonDisk[4] = vec2[](
		vec2( -0.94201624, -0.39906216 ),
		vec2( 0.94558609, -0.76890725 ),
		vec2( -0.094184101, -0.92938870 ),
		vec2( 0.34495938, 0.29387760 )
		);

	for(int i = 0; i < MAXLIGHTS; i++){
		att = 1.0;
		
		if(0.0 == gl_LightSource[i].position.w){
			att = 1.0;
			L = normalize(gl_LightSource[i].position.xyz);
		}
		else{
			L = gl_LightSource[i].position.xyz - position.xyz;
			float dist = length(L);
			L = normalize(L);
			R = reflect(-L, N);
			
			vec4 shadowCoordinateWdivide = ShadowCoord / ShadowCoord.w ;
	
			// Used to lower moir?? pattern and self-shadowing
			shadowCoordinateWdivide.z -= 0.0005;
	
	
			float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.st).z;
	
	
			shadow = 1.0;
			if (ShadowCoord.w > 0.0)
				shadow = distanceFromLight < shadowCoordinateWdivide.z ? 0.5 : 1.0 ;
			
			/*
			shadow = 1.0;
			for (int i=0;i<4;i++){
				if ( texture2D( ShadowMap, shadowCoordinateWdivide.st + poissonDisk[i]/700.0 ).z  <  shadowCoordinateWdivide.z ){
					shadow-=0.2;
				}
			}
//			*/
			
			att = 1.0 / (gl_LightSource[i].constantAttenuation
						+ gl_LightSource[i].linearAttenuation * dist
						+ gl_LightSource[i].quadraticAttenuation * dist * dist);
		
			if(gl_LightSource[i].spotCutoff <= 90.0){
				float cosine = max(0.0, dot(-L, normalize(gl_LightSource[i].spotDirection)));
				
				float delta_angle = gl_LightSource[i].spotCosCutoff - OUTER_ANGLE;
				
				if(cosine < gl_LightSource[i].spotCosCutoff){
					att = 0.0;
				}
				else{
					att = att * pow(cosine, gl_LightSource[i].spotExponent);
				}
				att = clamp((cosine - OUTER_ANGLE)/delta_angle, 0.0, 1.0);
				att *= shadow;
			}
		}
		//vec3 globalAmbient = vec3(gl_FrontLightModelProduct.sceneColor)
		//						* vec3(gl_FrontMaterial.ambient);
		
		vec3 ambientReflection = vec3(gl_LightSource[i].ambient)
									* vec3(gl_FrontMaterial.ambient);
		
		vec3 specularReflection;
		vec3 diffuseReflection;
		if(dot(N, L) < 0.0){
			diffuseReflection = vec3(0.0, 0.0, 0.0);
			specularReflection = vec3(0.0, 0.0, 0.0);
		}
		else{
			//diffuseReflection = att * vec3(gl_LightSource[i].diffuse)
			//						* vec3(gl_FrontMaterial.diffuse)
			//						* max(0.0, dot(N, L));
			diffuseReflection = att * vec3(gl_LightSource[i].diffuse)
									* texColor
									* max(0.0, dot(N, L));
			specularReflection = att * vec3(gl_LightSource[i].specular)
									* vec3(gl_FrontMaterial.specular)
									* pow(max(0.0, dot(R, E)), gl_FrontMaterial.shininess);
		}
		
		//totalLighting = totalLighting + globalAmbient + ambientReflection
		//					+ diffuseReflection + specularReflection;
		/*
		if(0.0 == gl_LightSource[i].position.w){
			specularReflection = vec3(0.0,0.0,0.0);
		}
		//*/
		totalLighting = totalLighting + ambientReflection
							+ diffuseReflection + specularReflection;
	}
	
	gl_FragColor = vec4(totalLighting, 1.0);
}