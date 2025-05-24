#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in uint inNormalIndex;
layout (location = 2) in vec2 inTexCoords;
layout (location = 3) in uint inBlockTypeID;

out vec2 TexCoord;
flat out uint blockTypeID;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

const vec3 normals[6] = vec3[](
    vec3(1, 0, 0),
    vec3(-1, 0, 0),
    vec3(0, 1, 0),
    vec3(0, -1, 0),
    vec3(0, 0, 1),
    vec3(0, 0, -1)
);

vec3 normal = normals[inNormalIndex];

void main()
{
    gl_Position = camMatrix * modelMatrix * vec4(inPosition, 1.0);
    TexCoord = inTexCoords;
    blockTypeID = inBlockTypeID;
}