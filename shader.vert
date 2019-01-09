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
//out vec3 vertexNormal; // For Lighting computation
//out vec3 ToLightVector; // Vector from Vertex to Light;
//out vec3 ToCameraVector; // Vector from Vertex to Camera;


void main()
{
	float u=position.x/widthTexture;
	float v=position.z/heightTexture;
    textureCoordinate = vec2 (u, v);
    /*
    // get texture color value, compute height
    vec4 texture_color = texture(rgbTexture, textureCoordinate);
    float y = heightFactor*(0.2126*texture_color.r + 0.7152*texture_color.g + 0.0722*texture_color.b);
    vec3 neigh0, neigh1, neigh2, neigh3, neigh4, neigh5;
    vec3 normal0, normal1, normal2, normal3, normal4, normal5;
    vec4 color;

    //Bottom-left: 3,4,5
    if(position.x==0 && position.z==0){
        //3th
        neigh3.x=position.x;
        neigh3.z=position.z+1;
        color = texture2D(rgbTexture, vec2(neigh3.x, neigh3.z));
        neigh3.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //4th
        neigh4.x=position.x+1;
        neigh4.z=position.z+1;
        color = texture2D(rgbTexture, vec2(neigh4.x, neigh4.z));
        neigh4.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //5th
        neigh5.x=position.x+1;
        neigh5.z=position.z;
        color = texture2D(rgbTexture, vec2(neigh5.x, neigh5.z));
        neigh5.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        // compute normal vector using also the heights of neighbor vertices
        normal3=normalize(cross(neigh4-position, neigh3-position));
        normal4=normalize(cross(neigh5-position, neigh4-position));
        vertexNormal=(normal3+normal4)/2;
    }
    //Top-left: 0,5
    else if(position.x==0 && position.z==heightTexture){
        //0th
        neigh0 = vec3(position.x, 0, position.z-1);
        vec4 color = texture2D(rgbTexture, vec2(neigh0.x, neigh0.z));
        neigh0.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //5th
        neigh5.x=position.x+1;
        neigh5.z=position.z;
        color = texture2D(rgbTexture, vec2(neigh5.x, neigh5.z));
        neigh5.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        // compute normal vector using also the heights of neighbor vertices
        vertexNormal=normalize(cross(neigh0-position, neigh5-position));
    }
    //Top-right: 0,1,2
    else if(position.x==widthTexture && position.z==heightTexture){
        //0th
        neigh0 = vec3(position.x, 0, position.z-1);
        vec4 color = texture2D(rgbTexture, vec2(neigh0.x, neigh0.z));
        neigh0.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //1th
        neigh1.x=position.x-1;
        neigh1.z=position.z-1;
        color = texture2D(rgbTexture, vec2(neigh1.x, neigh1.z));
        neigh1.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //2th
        neigh2.x=position.x-1;
        neigh2.z=position.z;
        color = texture2D(rgbTexture, vec2(neigh2.x, neigh2.z));
        neigh2.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        // compute normal vector using also the heights of neighbor vertices
        normal0=normalize(cross(neigh1-position, neigh0-position));
        normal1=normalize(cross(neigh2-position, neigh1-position));
        vertexNormal=(normal0+normal1)/2;

    }
    //Bottom-right: 2,3
    else if(position.x==widthTexture && position.y==0){
        //2th
        neigh2.x=position.x-1;
        neigh2.z=position.z;
        color = texture2D(rgbTexture, vec2(neigh2.x, neigh2.z));
        neigh2.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //3th
        neigh3.x=position.x;
        neigh3.z=position.z+1;
        color = texture2D(rgbTexture, vec2(neigh3.x, neigh3.z));
        neigh3.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        // compute normal vector using also the heights of neighbor vertices
        vertexNormal=normalize(cross(neigh3-position, neigh2-position));
    }
    else{
        //0th
        neigh0 = vec3(position.x, 0, position.z-1);
        color = texture2D(rgbTexture, vec2(neigh0.x, neigh0.z));
        neigh0.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //1th
        neigh1.x=position.x-1;
        neigh1.z=position.z-1;
        color = texture2D(rgbTexture, vec2(neigh1.x, neigh1.z));
        neigh1.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //2th
        neigh2.x=position.x-1;
        neigh2.z=position.z;
        color = texture2D(rgbTexture, vec2(neigh2.x, neigh2.z));
        neigh2.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //3th
        neigh3.x=position.x;
        neigh3.z=position.z+1;
        color = texture2D(rgbTexture, vec2(neigh3.x, neigh3.z));
        neigh3.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //4th
        neigh4.x=position.x+1;
        neigh4.z=position.z+1;
        color = texture2D(rgbTexture, vec2(neigh4.x, neigh4.z));
        neigh4.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        //5th
        neigh5.x=position.x+1;
        neigh5.z=position.z;
        color = texture2D(rgbTexture, vec2(neigh5.x, neigh5.z));
        neigh5.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

        // compute normal vector using also the heights of neighbor vertices
        normal0=normalize(cross(neigh1-position, neigh0-position));
        normal1=normalize(cross(neigh2-position, neigh1-position));
        normal2=normalize(cross(neigh3-position, neigh2-position));
        normal3=normalize(cross(neigh4-position, neigh3-position));
        normal4=normalize(cross(neigh5-position, neigh4-position));
        normal5=normalize(cross(neigh0-position, neigh5-position));
        vertexNormal=(normal0+normal1+normal2+normal3+normal4+normal5)/6;
    }
    // compute toLight vector vertex coordinate in VCS
    ToLightVector = normalize(vec3 (widthTexture / 2 - position.x, widthTexture + heightTexture - y, heightTexture / 2 - position.z));

    // compute toCamera vector vertex coordinate in VCS
    ToCameraVector = normalize(vec3 (cameraPosition.x - position.x, cameraPosition.y - y, cameraPosition.z - position.z));
   */
   // set gl_Position variable correctly to give the transformed vertex position

   gl_Position = MVP * vec4(position.x, position.y, position.z, 1);

}
