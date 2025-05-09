#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;
flat in uint blockTypeID;
//in float texIndex;

uniform sampler2D tex0;

void main()
{
   FragColor = texture(tex0, TexCoord);  
   //if (blockTypeID == 1u) {
   //   FragColor = vec4(0.4, 0.25, 0.1, 1.0); // dirt
   //} else if (blockTypeID == 2u) {
   //   FragColor = vec4(0.1, 0.8, 0.1, 1.0); // grass
   //} else if (blockTypeID == 3u) {
   //   FragColor = vec4(0.5, 0.3, 0.1, 1.0); // wood
   //} else {
   //   FragColor = vec4(1.0, 0.0, 1.0, 1.0); // magenta: unknown type
   //}
}