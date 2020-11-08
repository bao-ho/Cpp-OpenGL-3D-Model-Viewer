//shader vertex
#version 330 core

layout(location = 0) in	vec3 positionFromBuffer;
layout(location = 1) in	vec2 textureCoordinateFromBuffer;

out vec2 textureCoordinateToFragment;
uniform mat4 model, view, projection;

void main()
{
	gl_Position		= projection*view*model * vec4(positionFromBuffer, 1.0);
	textureCoordinateToFragment = textureCoordinateFromBuffer;
}


//shader fragment
#version 330 core

in  vec2 textureCoordinateToFragment;
uniform sampler2D texture0;
uniform sampler2D texture1;
out vec4 color;
void main()
{
	//color = texture(texture0, textureCoordinateToFragment) * colorToFragment;
	//color = colorToFragment;
	color = mix(texture(texture0, textureCoordinateToFragment), texture(texture1, textureCoordinateToFragment), 0.2) ;
}