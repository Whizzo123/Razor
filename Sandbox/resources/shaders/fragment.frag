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
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutoff;
	float outerCutoff;
	int lightType;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

vec3 CalcDirLight(Light dirLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-dirLight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	if(material.bUseTextures)
	{
		ambient = dirLight.ambient * texture(material.texture_diffuse, TexCoord).rgb;
		diffuse = dirLight.diffuse * diff * texture(material.texture_diffuse, TexCoord).rgb;
		specular = dirLight.specular * spec * texture(material.texture_specular, TexCoord).rgb;
	}
	else
	{
		ambient = dirLight.ambient * material.diffuse;
		diffuse = dirLight.diffuse * diff * material.diffuse;
		specular = dirLight.specular * spec * material.specular;
	}
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(Light spotLight, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-spotLight.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	if(theta > light.cutoff)
	{
		
		if(material.bUseTextures)
		{
			ambient = spotLight.ambient * texture(material.texture_diffuse, TexCoord).rgb;
			diffuse = spotLight.diffuse * diff * texture(material.texture_diffuse, TexCoord).rgb;
			specular = spotLight.specular * spec * texture(material.texture_specular, TexCoord).rgb;
		}
		else
		{
			ambient = spotLight.ambient * material.diffuse;
			diffuse = spotLight.diffuse * diff * material.diffuse;
			specular = spotLight.specular * spec * material.specular;
		}
		diffuse *= intensity;
		specular *= intensity;
		return (ambient + diffuse + specular);
	}
	else
	{
		return light.ambient * vec3(texture(material.texture_diffuse, TexCoord));	
	}
} 

vec3 CalcPointLight(Light pointLight, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));  
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
    // combine results
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
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

void main() 
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 lightDir;
	vec3 lightColor;
	switch(light.lightType)
		{
			case(0):
				lightColor = CalcDirLight(light, norm, viewDir);
			break;
			case(1):
				lightColor = CalcPointLight(light, norm, FragPos, viewDir);
			break;
			case(2):
				lightColor = CalcSpotLight(light, norm, viewDir);
		}
    FragColor = vec4(lightColor, 1.0);
}

 