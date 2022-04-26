#version 330 core
out vec4 FragColor;
  
in vec3 TexCoords;
 
uniform samplerCube skybox;
 
void main()
{    
    FragColor = vec4(3.0f, 0.8f, 3.0f, 0.5f) * texture(skybox, TexCoords);
}
