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
	float x_diff=2/widthTexture;
	float z_diff=2/heightTexture;
    // get texture color value, compute height
    vec4 texture_color = texture2D(rgbTexture, position);
    float y = heightFactor*(0.2126*texture_color.r + 0.7152*texture_color.g + 0.0722*texture_color.b);
    vec3 neigh0, neigh1, neigh2, neigh3, neigh4, neigh5;
    vec3 normal0, normal1, normal2, normal3, normal4, normal5;
    //Bottom-left: 3,4,5
    if(position.x==-1 && position.z==-1){
    	//3th
    	neigh3.x=position.x;
    	neigh3.z=position.z+z_diff;
    	color = texture2D(rgbTexture, neigh3);
    	neigh3.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);
    	
    	//4th
    	neigh4.x=position.x+x_diff;
    	neigh4.z=position.z+z_diff;
    	color = texture2D(rgbTexture, neigh4);
    	neigh4.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//5th
    	neigh5.x=position.x+x_diff;
    	neigh5.z=position.z;
    	color = texture2D(rgbTexture, neigh5);
    	neigh5.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	// compute normal vector using also the heights of neighbor vertices
    	normal3=normalize(cross(neigh4-position, neigh3-position));
    	normal4=normalize(cross(neigh5-position, neigh4-position));
    	vertexNormal=(normal3+normal4)/2;
    }
    //Top-left: 0,5
    else if(position.x==-1 && position.z==1){
    	//0th
    	neigh0 = vec3(position.x, 0, position.z-z_diff);
    	vec4 color = texture2D(rgbTexture, neigh0);
    	neigh0.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//5th
    	neigh5.x=position.x+x_diff;
    	neigh5.z=position.z;
    	color = texture2D(rgbTexture, neigh5);
    	neigh5.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	// compute normal vector using also the heights of neighbor vertices
    	vertexNormal=normalize(cross(neigh0-position, neigh5-position));
    }
    //Top-right: 0,1,2
    else if(position.x==1 && position.z==1){
	    //0th
	    neigh0 = vec3(position.x, 0, position.z-z_diff);
	    vec4 color = texture2D(rgbTexture, neigh0);
	    neigh0.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

	    //1th
	    neigh1.x=position.x-x_diff;
	    neigh1.z=position.z-z_diff;
	    color = texture2D(rgbTexture, neigh1);
	    neigh1.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

	    //2th
	    neigh2.x=position.x-x_diff;
	    neigh2.z=position.z;
	    color = texture2D(rgbTexture, neigh2);
	    neigh2.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

	    // compute normal vector using also the heights of neighbor vertices
    	normal0=normalize(cross(neigh1-position, neigh0-position));
    	normal1=normalize(cross(neigh2-position, neigh1-position));
    	vertexNormal=(normal0+normal1)/2;

    }
    //Bottom-right: 2,3
    else if(position.x==1 && position.y==-1){
	    //2th
	    neigh2.x=position.x-x_diff;
	    neigh2.z=position.z;
	    color = texture2D(rgbTexture, neigh2);
	    neigh2.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

	    //3th
	    neigh3.x=position.x;
	    neigh3.z=position.z+z_diff;
	    color = texture2D(rgbTexture, neigh3);
	    neigh3.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

	    // compute normal vector using also the heights of neighbor vertices
    	vertexNormal=normalize(cross(neigh3-position, neigh2-position));
    }
    else{
    	//0th
    	neigh0 = vec3(position.x, 0, position.z-z_diff);
    	vec4 color = texture2D(rgbTexture, neigh0);
    	neigh0.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//1th
    	neigh1.x=position.x-x_diff;
    	neigh1.z=position.z-z_diff;
    	color = texture2D(rgbTexture, neigh1);
    	neigh1.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//2th
    	neigh2.x=position.x-x_diff;
    	neigh2.z=position.z;
    	color = texture2D(rgbTexture, neigh2);
    	neigh2.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//3th
    	neigh3.x=position.x;
    	neigh3.z=position.z+z_diff;
    	color = texture2D(rgbTexture, neigh3);
    	neigh3.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//4th
    	neigh4.x=position.x+x_diff;
    	neigh4.z=position.z+z_diff;
    	color = texture2D(rgbTexture, neigh4);
    	neigh4.y=heightFactor*(0.2126*color.r + 0.7152*color.g + 0.0722*color.b);

    	//5th
    	neigh5.x=position.x+x_diff;
    	neigh5.z=position.z;
    	color = texture2D(rgbTexture, neigh5);
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
    vec3 lightPosition = vec3();

    ToLightVector = normalize(0, widthTexture+heightTexture, 0);

   
   // set gl_Position variable correctly to give the transformed vertex position

   gl_Position = vec4(0,0,0,0); // this is a placeholder. It does not correctly set the position 
    
}
