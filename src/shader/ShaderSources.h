//
// Created by Dmitri on 2026-11-17.
//

#ifndef IDEAL_GAS_LAW_SHADERSOURCES_H
#define IDEAL_GAS_LAW_SHADERSOURCES_H

inline const char* DEFAULT_VERTEX_SHADER_SOURCE = R"(
#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 lookAt;
uniform mat4 model;

out prop {
    vec3 normal;
    vec3 fragp;
} vs_out;

void main() {
    vs_out.normal = normalize(transpose(inverse(mat3(model))) * normal);
    vs_out.fragp = vec3(model * vec4(position, 1.0));

    gl_Position = projection * lookAt * model * vec4(position, 1.0);
}
)";


inline const char* DEFAULT_FRAGMENT_SHADER_SOURCE = R"(
#version 410 core

out vec4 fragc;

uniform vec3 color;
vec3 lightPosition = vec3(100.0);

in prop {
    vec3 normal;
    vec3 fragp;
} fs_in;

void main() {

    vec3 ambient = color * 0.4;

    vec3 lightDirection = normalize(lightPosition - fs_in.fragp);
    float diff = max(dot(fs_in.normal, lightDirection), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    fragc = vec4(color * (ambient + diffuse), 1.0);
}
)";


inline const char* DEFERRED_VERTEX_SHADER_SOURCE = R"(
#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 projection;
uniform mat4 lookAt;
uniform mat4 model;

out prop {
    vec3 normal;
    vec3 fragp;
} vs_out;

void main() {
    vs_out.normal = normalize(transpose(inverse(mat3(model))) * normal);
    vs_out.fragp = vec3(model * vec4(position, 1.0));

    gl_Position = projection * lookAt * model * vec4(position, 1.0);
}
)";


inline const char* DEFERRED_FRAGMENT_SHADER_SOURCE = R"(
#version 410 core

layout (location = 0) out vec4 fragp;
layout (location = 1) out vec4 dst;
layout (location = 2) out vec4 normal;
layout (location = 3) out vec4 fragc;

uniform vec3 cameraPosition;

uniform vec3 color;
vec3 lightPosition = vec3(100.0);

in prop {
    vec3 normal;
    vec3 fragp;
} fs_in;

void main() {

    vec3 ambient = color * 0.4;

    vec3 lightDirection = normalize(lightPosition - fs_in.fragp);
    float diff = max(dot(fs_in.normal, lightDirection), 0.0);
    vec3 diffuse = diff * vec3(1.0);

    fragc = vec4(color * (ambient + diffuse), 1.0);
    fragp = vec4(fs_in.fragp, 1.0);
    normal = vec4(fs_in.normal, 1.0);

    float linearDepth = length(fs_in.fragp);
    dst = vec4(linearDepth, 0.0, 0.0, 1.0);
}
)";





inline const char* QUAD_VERTEX_SHADER_SOURCE = R"(
#version 410 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out prop {
    vec2 uv;
} vs_out;

void main() {
    vs_out.uv = uv;
    gl_Position = vec4(position, 0.0, 1.0);
}
)";


inline const char* QUAD_FRAGMENT_SHADER_SOURCE = R"(
#version 410 core

out vec4 fragc;
uniform sampler2D scene;

uniform mat4 inverseProjection;
uniform mat4 inverseLookAt;
uniform vec3 cameraPosition;
uniform vec2 screenSize;

in prop {
    vec2 uv;
} fs_in;

vec3 zenithColor = vec3(0.05, 0.15, 0.4);
vec3 horizonColor = vec3(0.6, 0.7, 0.9);
vec3 groundColor = vec3(0.4, 0.35, 0.3);

vec3 computeRayDirection(vec2 fragp) {
    vec2 uv = (fragp / screenSize) * 2.0 - 1.0;
    vec4 clip = vec4(uv, -1.0, 1.0);
    vec4 view = inverseProjection * clip;
    view.z = -1.0;
    view.w = 0.0;
    vec4 world = inverseLookAt * view;
    return normalize(world.xyz);
}

void main() {

    vec4 albedo = texture(scene, fs_in.uv);
    float a = albedo.a;

    vec3 viewDir = computeRayDirection(gl_FragCoord.xy);
    float y = viewDir.y;

    vec3 skyColor;
    if (y > 0.0) {
        float t = pow(y, 0.65);
        skyColor = mix(horizonColor, zenithColor, t);
    }
    else {
        float t = pow(-y, 0.7);
        skyColor = mix(horizonColor, groundColor, t);
    }

    vec3 col = a == 0.0 ? skyColor : albedo.rgb;
    fragc = vec4(col, 1.0);
}
)";

#endif //IDEAL_GAS_LAW_SHADERSOURCES_H
