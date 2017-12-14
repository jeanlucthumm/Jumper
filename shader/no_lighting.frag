#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 Position;
in vec2 Uv;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiny;

    bool hasKaMap;
    bool hasKdMap;
    sampler2D kaMap;
    sampler2D kdMap;
};
uniform Material material;

void main() {
    if (material.hasKdMap) {
        color = vec4(vec3(texture2D(material.kdMap, Uv)), 1);
    }
    else {
        color = vec4(material.diffuse, 1);
    }
}