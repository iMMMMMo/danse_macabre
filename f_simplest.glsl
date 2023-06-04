#version 330

uniform sampler2D textureMap0;
vec4 lightColor1;
vec4 lightColor2;

in vec4 ic;
in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;
in vec2 iTexCoord0;
in vec4 lC1;
in vec4 lC2;

out vec4 pixelColor;

void main(void) {

    vec4 ml1 = normalize(l1);
    vec4 ml2 = normalize(l2);
    vec4 mn = normalize(n);
    vec4 mv = normalize(v);
    vec4 mr1 = reflect(-ml1, mn); // reflected vector for the first light
    vec4 mr2 = reflect(-ml2, mn); // reflected vector for the second light

    vec4 ks = vec4(1, 1, 1, 1);
    vec4 kd = texture(textureMap0, iTexCoord0);

    float nl1 = clamp(dot(mn, ml1), 0, 1);
    float nl2 = clamp(dot(mn, ml2), 0, 1);
    float rv1 = pow(clamp(dot(mr1, mv), 0, 1), 50);
    float rv2 = pow(clamp(dot(mr2, mv), 0, 1), 50);

    pixelColor = vec4(kd.rgb * (nl1 * lC1.rgb + nl2 * lC2.rgb), kd.a) + vec4(ks.rgb * (rv1 * lC1.rgb + rv2 * lC2.rgb), 0);
}