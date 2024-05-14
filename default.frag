#version 330 core
// Outputs colors in RGBA
layout (location = 0) out vec4 FragColor;

// Inputs the color from the Vertex Shader
in vec3 color;
// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;
in float u_texIndex;
// Gets the Texture Unit from the main function
uniform sampler2D u_Textures[2];

void main()
{
	int index = int(u_texIndex);
	FragColor = texture(u_Textures[index],texCoord);
	//FragColor = vec4(texCoord, 0.0,texCoord);
	//FragColor = vec4(u_texIndex, u_texIndex, u_texIndex, texCoord);
}