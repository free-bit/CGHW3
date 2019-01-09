#include "helper.h"
#include <glm.hpp> // GL Math library header
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp> 
#include <iostream>

using namespace glm;

static GLFWwindow * win = NULL;

bool toggleFS=true;
int windowLastWidth;
int windowLastHeight;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

int widthTexture, heightTexture;

// Uniform variable
float speed=0;
float heightFactor=10.0;
vec4 cameraPosition;
vec4 gaze;

static void errorCallback(int error,
  const char * description) {
  fprintf(stderr, "Error: %s\n", description);
}

void incrHeightFactor(){
  heightFactor+=0.5;
  glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
}
void decrHeightFactor(){
  heightFactor-=0.5;
  glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
}
void toggleFullscreen(){
  if(toggleFS){
   glfwGetWindowSize(win, &windowLastWidth, &windowLastHeight);
   GLFWmonitor* monitor = glfwGetPrimaryMonitor();
   const GLFWvidmode* mode = glfwGetVideoMode(monitor);
   glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
   //glfwSetWindowMonitor(window, NULL, xpos, ypos, width, height, 0);
  }
  else{
    glfwSetWindowMonitor(win, NULL, 0, 0, windowLastWidth, windowLastHeight, 0);
  }
  toggleFS=!toggleFS;
}
void generateHeightMap(float* vertices, unsigned int* indices){
  int index=0;
  int vindex=0;
  int i;
  for(i=0; i<heightTexture; i++){
    int j;
    for(j=0; j<widthTexture; j++){
      // fill in the vertices array
      vertices[vindex++] = j;   // x
      vertices[vindex++] = 0;   // y
      vertices[vindex++] = i;   // z

      indices[index++]=i*(widthTexture+1)+j;
      // cout<<indices[index-1]<<" ";

      indices[index++]=(i+1)*(widthTexture+1)+j;
      // cout<<indices[index-1]<<" ";

      indices[index++]=i*(widthTexture+1)+j+1;
      // cout<<indices[index-1]<<endl;

      indices[index++]=i*(widthTexture+1)+j+1;
      // cout<<indices[index-1]<<" ";

      indices[index++]=(i+1)*(widthTexture+1)+j;
      // cout<<indices[index-1]<<" ";

      indices[index++]=(i+1)*(widthTexture+1)+j+1;
      // cout<<indices[index-1]<<endl<<endl;
    }
    // fill in the vertices array
    vertices[vindex++] = j;   // x
    //cout<<vertices[vindex-1]<<" ";
    vertices[vindex++] = 0;   // y
    //cout<<vertices[vindex-1]<<" ";
    vertices[vindex++] = i;   // z
    //cout<<vertices[vindex-1]<<endl<<endl;
  }
  //Final iteration for vertices:
  for(int j=0; j<=widthTexture; j++){
    // fill in the vertices array
    vertices[vindex++] = j;   // x
    //cout<<vertices[vindex-1]<<" ";
    vertices[vindex++] = 0;   // y
    //cout<<vertices[vindex-1]<<" ";
    vertices[vindex++] = i;   // z
    //cout<<vertices[vindex-1]<<endl<<endl;
  }
  //cout<<"Number of vertices*3: "<<vindex<<endl;
  //cout<<"Number of indices: "<<index<<endl;
}

//Added
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch(key) 
    {
      /*W, S for pitch change*/
      case GLFW_KEY_W:
        break;
      case GLFW_KEY_S:
        break;
      /*A, D for yaw change*/
      case GLFW_KEY_A:
        break;
      case GLFW_KEY_D:
        break;
      /*O, L for height change*/
      case GLFW_KEY_O:
        incrHeightFactor();
        break;
      case GLFW_KEY_L:
        decrHeightFactor();
        break;
      /*U, J for speed*/
      case GLFW_KEY_U:
        if(action == GLFW_PRESS)
          speed+=0.25;
        break;
      case GLFW_KEY_J:
        if(action == GLFW_PRESS && speed>=0.25)
          speed-=0.25;
        break;
      /*F for fullscreen*/
      case GLFW_KEY_F:
        if(action == GLFW_PRESS)
          toggleFullscreen();
        break;
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
      /*Otherwise*/
      default:
        break;
      }
}

int main(int argc, char * argv[]) {

  if (argc != 2) {
    printf("Only one texture image expected!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  win = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);

  if (!win) {
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(win);


  //Added
  glfwSetKeyCallback(win, keyCallback);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    glfwTerminate();
    exit(-1);
  }

  initShaders();
  glEnable(GL_TEXTURE_2D);
  glUseProgram(idProgramShader);

  initTexture(argv[1], & widthTexture, & heightTexture);

  cameraPosition = vec4((float)widthTexture / 2, (float)widthTexture / 10, - (float)widthTexture / 4, 1.0f);
  gaze = vec4(0.0f, 0.0f, 1.0f, 0.0f);


  glUniform4fv(glGetUniformLocation(idProgramShader, "cameraPosition"), 1, value_ptr(cameraPosition));
  glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
  glUniform1i(glGetUniformLocation(idProgramShader, "widthTexture"), widthTexture);
  glUniform1i(glGetUniformLocation(idProgramShader, "heightTexture"), heightTexture);

  //glUniform1i(glGetUniformLocation(idProgramShader, "rgbTexture"), 0);

  float *vertices=new float[(widthTexture+1)*(heightTexture+1)*3];//OK
  unsigned int *indices=new unsigned int[widthTexture*heightTexture*6];//OK
  generateHeightMap(vertices, indices);

  unsigned int VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, (widthTexture+1)*(heightTexture+1)*3*sizeof(float), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, widthTexture*heightTexture*6*sizeof(float), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe

  delete [] vertices;
  delete [] indices;
  glEnable(GL_DEPTH_TEST);
  while (!glfwWindowShouldClose(win)) {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    ratio = width / (float) height;

    mat4 proj = perspective(radians(57.5f), ratio, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);
    cameraPosition+=gaze*speed;
    //From camera look in the direction of gaze vector with up vector in the direction of y
    mat4 view = lookAt(vec3(cameraPosition), vec3(cameraPosition+gaze), vec3(0.0f, 1.0f, 0.0f));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mat4 mvp = proj * view;
    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MVP"), 1, GL_FALSE, value_ptr(mvp));
    glDrawElements(GL_TRIANGLES, widthTexture*heightTexture*6, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}