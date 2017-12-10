#version 330 core

out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiny;
};
uniform Material material;

void main() {
    color = vec4(material.ambient, 1.0);
}
