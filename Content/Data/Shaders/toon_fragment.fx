varying vec3 vNormal;
varying vec3 vVertex;

void main() {
	float shininess = 20.0;
	
	vec4 color0 = vec4(0.8, 0.0, 0.0, 1.0);

	vec4 color1 = vec4(0.0, 0.0, 0.0, 1.0);

	vec4 color2 = vec4(0.8, 0.0, 0.0, 1.0);
	
	vec3 eyePos = vec3(0.0,0.0,5.0);
	vec3 lightPos = vec3(0.0,5.0,5.0);

	vec3 Normal = normalize(gl_NormalMatrix * vNormal);
	vec3 EyeVert = normalize(eyePos - vVertex);
	vec3 LightVert = normalize(lightPos - vVertex);
	vec3 EyeLight = normalize(LightVert+EyeVert);

	float sil = max(dot(Normal,EyeVert), 0.0);
	if(sil < 0.3) {
		gl_FragColor = color1;
	}
	else  {
		gl_FragColor = color0;

		float spec = pow(max(dot(Normal, EyeLight),0.0), shininess);

		if (spec < 0.2) {
			gl_FragColor *= 0.8;
		}
		else {
			gl_FragColor = color2;
		}

		float diffuse = max(dot(Normal, LightVert), 0.0);
		if(diffuse < 0.5) {
			gl_FragColor *= 0.8;
		}
	}
}