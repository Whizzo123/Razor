#version 330 core


out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 entitycolor;

void main() 
{
    FragColor = vec4(entitycolor, 1.0);
}