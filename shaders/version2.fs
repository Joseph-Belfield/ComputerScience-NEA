#version 410 core


in vec3 vNormal;
in vec3 vFragPos;

in vec3 vFragPosView;
in vec3 vNormalView;
in mat4 vView;


out vec4 FragColor;


uniform vec3 uObjectColor;
uniform vec3 uLightColor;

uniform float uAmbience;
uniform vec3 uLightPos;
uniform float uSpecularStrength;


void main()
{
    vec3 ambient = uLightColor * uAmbience;


    vec3 normal = normalize(vNormal);
    vec3 lightDirection = normalize(uLightPos - vFragPos);

    float diff = max(dot(normal, lightDirection), 0.0f);
    vec3 diffuse = diff * uLightColor;


    vec3 viewPos = vec3(0.0f);
    vec3 lightDirectionView = normalize(vec3(vView * vec4(lightDirection, 0.0f)));

    vec3 viewDir = normalize(viewPos - vFragPosView);
    vec3 reflectDir = reflect(-lightDirectionView, vNormalView);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = uSpecularStrength * spec * uLightColor;


    vec3 finalColor = (ambient + diffuse + specular) * uObjectColor;
	FragColor = vec4(finalColor, 1.0f);
}