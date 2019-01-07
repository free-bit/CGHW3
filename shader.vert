#version 410

layout(location = 0) in vec3 position;

// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform float heightFactor;

// Texture-related data
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;


// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{

    // get texture color value, compute height
    vec4 texture_color = texture2D(rgbTexture, textureCoordinate);
    float y = heightFactor*(0.2126*texture_color.r + 0.7152*texture_color.g + 0.0722*texture_color.b);

    // compute normal vector using also the heights of neighbor vertices
    //vertexNormal

    // compute toLight vector vertex coordinate in VCS
    vec3 lightPosition = vec3(widthTexture/2, widthTexture+heightTexture, heightTexture/2);
    ToLightVector = normalize(lightPosition-vec3(gl_Position));

   
   // set gl_Position variable correctly to give the transformed vertex position

   gl_Position = vec4(textureCoordinate.x, y, textureCoordinate.y,0); // this is a placeholder. It does not correctly set the position 
    
}
