#version 330

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform vec4 lightPos1;
uniform vec4 lightPos2;
uniform vec4 lightColor1;
uniform vec4 lightColor2;

in vec4 vertex;
in vec4 color;
in vec4 normal;
in vec2 texCoord0;

out vec4 ic;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0;
out vec4 l1;
out vec4 l2;
out vec4 lC1;
out vec4 lC2;

void main(void) {
    lC1 = lightColor1;
    lC2 = lightColor2;
    l1 = normalize(V * lightPos1 - V * M * vertex); // vector towards the first light in eye space
    l2 = normalize(V * lightPos2 - V * M * vertex); // vector towards the second light in eye space
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex); // vector towards the viewer in eye space
    n = normalize(V * M * normal); // normal vector in eye space

    ic = vec4(1, 1, 1, 0);
    iTexCoord0 = texCoord0;

    gl_Position = P * V * M * vertex;
}