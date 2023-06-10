#version 330
uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform sampler2D textureMap2;
uniform sampler2D textureMap3;
vec4 lightColor1;
vec4 lightColor2;

in vec4 ic;
in vec4 n;
in vec4 l1;
in vec4 l2;
in vec4 v;
in vec2 iTexCoord0;
in vec2 iTexCoord1;
in vec2 iTexCoord2;
in vec2 iTexCoord3;
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
    vec4 tex0 = texture(textureMap0, iTexCoord0);
    vec4 tex1 = texture(textureMap1, iTexCoord1);
    
    // Uwzglêdnij teksturê typu normal (texCoord2) przy u¿yciu normal mappingu
    vec4 tex2 = texture(textureMap2, iTexCoord2);
    vec4 normalMap = vec4((tex2.rgb * 2.0) - 1.0, 0.0);
    vec3 finalNormal = normalize(mn.rgb + normalMap.rgb);
    
    vec4 tex3 = texture(textureMap3, iTexCoord3);
    vec4 mixResult = mix(tex0, tex1, 0.3);
    mixResult = mix(mixResult, tex3, 0.3);
    
    vec4 kd = mixResult;

    float nl1 = clamp((finalNormal.x * ml1.x) + (finalNormal.y * ml1.y) + (finalNormal.z * ml1.z), 0, 1);
    float nl2 = clamp((finalNormal.x * ml2.x) + (finalNormal.y * ml2.y) + (finalNormal.z * ml2.z), 0, 1);
    float rv1 = pow(clamp((mr1.x * mv.x) + (mr1.y * mv.y) + (mr1.z * mv.z), 0, 1), 50);
    float rv2 = pow(clamp((mr2.x * mv.x) + (mr2.y * mv.y) + (mr2.z * mv.z), 0, 1), 50);

    pixelColor = vec4(kd.rgb * (nl1 * lC1.rgb + nl2 * lC2.rgb), kd.a) + vec4(ks.rgb * (rv1 * lC1.rgb + rv2 * lC2.rgb), 0);
}
