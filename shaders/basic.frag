#version 330 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    int type;

    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 vertColor;
in vec2 texCoords;
out vec4 outColor;
in vec3 vertNormal;
in vec3 FragPos;

uniform sampler2D ourTexture;
uniform bool wireframeMode;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    if(wireframeMode)
        outColor = vec4(vertColor, 1.0f);
    else
    {
        // direction light
        if(light.type == 1)
        {
            vec3 ambient = light.ambient * material.ambient;

            vec3 norm = normalize(vertNormal);
            vec3 lightDir = -light.position;
            float diff_koef = max(dot(norm, -lightDir), 0.0f);
            vec3 diffuse = light.diffuse * (diff_koef * material.diffuse);

            vec3 viewDir = normalize(FragPos - viewPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec_koef = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
            vec3 specular = light.specular * (spec_koef * material.specular);

            outColor = texture(ourTexture, texCoords) * vec4(ambient + diffuse + specular, 1.0f);
        }
        // point light
        else if(light.type == 2) 
        {
            float dist = distance(light.position, FragPos);
            float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

            vec3 ambient = light.ambient * material.ambient * attenuation;

            // diffuse
            vec3 norm = normalize(vertNormal);
            vec3 lightDir = normalize(FragPos - light.position);
            float diff_koef = max(dot(norm, -lightDir), 0.0f);
            vec3 diffuse = light.diffuse * (diff_koef * material.diffuse) * attenuation;

            // specular
            vec3 viewDir = normalize(FragPos - viewPos);
            vec3 reflectDir = reflect(-lightDir, norm);
            float spec_koef = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
            vec3 specular = light.specular * (spec_koef * material.specular) * attenuation;

            outColor = texture(ourTexture, texCoords) * vec4(ambient + diffuse + specular, 1.0f);
        }
        else
            outColor = vec4(vertColor, 1.0f);
    }
}
