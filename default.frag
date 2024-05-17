#version 330 core
// Outputs colors in RGBA
layout (location = 0) out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec2 texCoord;
flat in int u_texIndex;
// Gets the Texture Unit from the main function
uniform sampler2D u_Textures[3];

void main()
{
	FragColor = texture(u_Textures[u_texIndex],texCoord);
	//FragColor = vec4(texCoord, 0.0,texCoord);
	//FragColor = vec4(u_texIndex, u_texIndex, u_texIndex, texCoord);
}