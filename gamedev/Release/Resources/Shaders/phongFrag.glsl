#version 150 core

uniform sampler2D tex;

uniform vec3 cameraPos;

uniform vec3 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

uniform float specularity;

in Vertex {
	vec3 worldPos;
	vec2 texCoord;
	vec3 normal;
} IN;

out vec4 gl_FragColor;

void main(void){
	//Normalized direction vector between the light and interpolated world position
	vec3 incident	= normalize(lightPos - IN.worldPos);

	//Normalized direction vector between camera and interpolated world position
	vec3 viewDir	= normalize(cameraPos - IN.worldPos);
	
	//The half way vector between the incident and view direction vector
	vec3 halfDir	= normalize(incident + viewDir);

	//The current fragments distance from the light position
	float dist	= length(lightPos - IN.worldPos);

	//Attenuation value from length by light radius, and clamping between 1 and 0
	float atten	= 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	//Lambertian reflectance value calculated  by taking dot product of incident vector with
	//the normal (clamped as negatives would mean a face that is facing away from the light)
	float lambert	= max(0.0, dot(incident, IN.normal));

	//Specular factor is dot product of half direction and the normal then,
	float rFactor	= max(0.0, dot(halfDir, IN.normal));

	//raising it to the specular power (depends how shiny) MAKE THE FACTOR A UNIFORM
	float sFactor	= pow(rFactor, specularity);

	//Now calculate fragment colour!
	vec4 texCol		= texture(tex, IN.texCoord);
	vec3 ambient	= texCol.rgb * lightColour * 0.1;
	vec3 diffuse	= texCol.rgb * lightColour * lambert * atten;
	vec3 specular	= lightColour * sFactor * atten;

	gl_FragColor	= vec4(ambient + diffuse + specular, texCol.a);
	//gl_FragColor = vec4(1,1,1,1);
}