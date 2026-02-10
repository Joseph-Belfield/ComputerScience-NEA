#version 410 core


layout(location=0) in vec3 aPosition;
layout(location=1) in vec3 aNormal;


out vec3 vNormal;
out vec3 vFragPos;

out vec3 vFragPosView;
out vec3 vNormalView;
out mat4 vView;


uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uPerspective;


void main()
{
    vNormal = mat3(transpose(inverse(uModel))) * aNormal;
    vFragPos = vec3(uModel * vec4(aPosition, 1.0f));

    vFragPosView = vec3(uView * uModel * vec4(aPosition, 1.0f));
    vNormalView = normalize(mat3(transpose(inverse(uModel * uView))) * aNormal);
    vView = uView;


    vec4 finalPosition = uPerspective * uView * uModel * vec4(aPosition, 1.0f);
    gl_Position = finalPosition;
}