#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 Position;

struct dirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define DIRLIGHT_COUNT 1
uniform dirLight dirLights[DIRLIGHT_COUNT];

uniform cameraPos;

vec3 computeDirLight(dirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);

    // diffuse
    float diff = max(dot(normal, lightDir), 0.0);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture2D(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture2D(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture2D(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

void main() {
    vec3 result = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - Position);

    // directional lights
    for (int i = 0; i < DIRLIGHT_COUNT; i++) {
        result += computeDirLight(dirLights[i], norm, viewDir);
    }

    color = vec4(result, 1.0);
}
