#version 330 core

out vec4 color;

in vec3 Normal;
in vec3 Position;
in vec2 Uv;

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define DIRLIGHT_COUNT 1
uniform DirLight dirLights[DIRLIGHT_COUNT];

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define POINTLIGHT_COUNT 6
uniform PointLight pointLights[POINTLIGHT_COUNT];

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

uniform vec3 cameraPos;

vec3 computeDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);

    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiny);

    vec3 ambient;
    if (material.hasKaMap) {
        ambient = light.ambient * vec3(texture(material.kaMap, Uv));
    }
    else {
        ambient = light.ambient * material.ambient;
    }
    vec3 diffuse;
    if (material.hasKdMap) {
        diffuse = light.diffuse * diff * vec3(texture(material.kdMap, Uv));
    }
    else {
        diffuse = light.diffuse * diff * material.diffuse;
    }
    vec3 specular;
    if (dot(normal, lightDir) < 0.0) {
        // light on the wrong side
        specular = vec3(0.0, 0.0, 0.0);
    }
    else {
        specular = light.specular * spec * material.specular;
    }

//    return (ambient + diffuse + specular);
    return diffuse + specular;
}

vec3 computePointLight(PointLight light, vec3 normal, vec3 position, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - position);
  vec3 reflectDir = reflect(-lightDir, normal);
  float distance = length(light.position - position);

  float diff = max(dot(normal, lightDir), 0.0);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiny);
  float attenuation = 1.0 / (
        light.constant +
        light.linear * distance +
        light.quadratic * (distance * distance));

  vec3 ambient;
  if (material.hasKaMap) {
      ambient = light.ambient * vec3(texture(material.kaMap, Uv));
  }
  else {
      ambient = light.ambient * material.ambient;
  }
  vec3 diffuse;
  if (material.hasKdMap) {
      diffuse = light.diffuse * diff * vec3(texture(material.kdMap, Uv));
  }
  else {
      diffuse = light.diffuse * diff * material.diffuse;
  }
  vec3 specular = light.specular * spec * material.specular;

  ambient *= attenuation;
  diffuse *= attenuation;
  specular *= attenuation;
//  return (ambient + diffuse + specular);
  return (diffuse + specular);
}

void main() {
    vec3 result = vec3(0.0);

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - Position);

    // directional lights
    for (int i = 0; i < DIRLIGHT_COUNT; i++) {
        result += computeDirLight(dirLights[i], norm, viewDir);
    }

    // point lights
    for (int i = 0; i < POINTLIGHT_COUNT; i++) {
        result += computePointLight(pointLights[i], norm, Position, viewDir);
    }

    color = vec4(result, 1.0);
}
