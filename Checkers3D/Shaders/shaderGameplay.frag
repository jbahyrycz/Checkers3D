#version 450

in vec4 vCol;
in vec2 texCoord;

out vec4 color;

uniform sampler2D theTexture;

void main()
{
	color = texture(theTexture, texCoord);
	// * vec4(0.0f, 1.0f, 0.0f, 1.0f);
}