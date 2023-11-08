#version 330 core

struct Material
{
	sampler2D texture_diffuse;
	sampler2D texture_specular;
	sampler2D texture_normal;
	sampler2D texture_height;
	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
	float shininess;
	bool bUseTextures;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main() 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	if(material.bUseTextures)
	{
		ambient = light.ambient * texture(material.texture_diffuse, TexCoord).rgb;
		diffuse = light.diffuse * diff * texture(material.texture_diffuse, TexCoord).rgb;
		specular = light.specular * spec * texture(material.texture_specular, TexCoord).rgb;
	}
	else
	{
		ambient = light.ambient * material.diffuse;
		diffuse = light.diffuse * diff * material.diffuse;
		specular = light.specular * spec * material.specular;
	}
	
	

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}