#version 150 core

uniform sampler2D tex;

in Vertex	{
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 gl_FragColor;

void main(void)	{	
	//gl_FragColor= IN.colour;
	gl_FragColor = texture(tex, IN.texCoord);

	//vec4 texColor = texture(tex, IN.texCoord) - IN.colour / 8;

	//gl_FragColor = texColor;
	//gl_FragColor = vec4(1,1,1,1);
}