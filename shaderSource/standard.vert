#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in uint aBlockTypeID;

out vec2 TexCoord;
flat out uint blockTypeID;
//out float texIndex;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    blockTypeID = aBlockTypeID;
    //if (aNormal.y > 0.0)
    //    texIndex = 0.0;  // Top face
    //else if (aNormal.y < 0.0)
    //    texIndex = 1.0;  // Bottom face
    //else
    //    texIndex = 2.0;  // Sides face
}