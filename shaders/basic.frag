#version 330 core
in vec3 vertColor;
in vec2 texCoords;
out vec4 outColor;
in vec3 vertNormal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform bool wireframeMode;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientColor;

void main()
{
    vec3 ambient = ambientColor * 0.1f;

    vec3 norm = normalize(vertNormal);
    vec3 lightDir = normalize(FragPos - lightPos);

    float diff_koef = max(dot(norm, -lightDir), 0.0f);
    vec3 diffuse = diff_koef * lightColor;

    vec3 viewDir = normalize(FragPos - viewPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float specular_strength = 2.0f;
    float spec_koef = pow(max(dot(viewDir, reflectDir), 0.0f), 1000);
    vec3 specular = specular_strength * spec_koef * lightColor;


    if(wireframeMode)
        outColor = vec4(vertColor, 1.0f);
    else
        outColor = texture(ourTexture, texCoords) * 
                   vec4(diffuse + ambient + specular, 1.0f);
}
