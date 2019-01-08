#include "helper.h"
#include <glm.hpp> // GL Math library header
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp> 
#include <iostream>

using namespace glm;

static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;

int widthTexture, heightTexture;

// Uniform variable
float heightFactor=10.0;
vec4 cameraPosition;

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
void generateHeightMap(float* vertices, unsigned int* indices){
  int index=0;
  int vindex=0;

  for(int i=0; i<heightTexture; i++){
    for(int j=0; j<widthTexture; j++){
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
  }
  // cout<<index<<endl;
}

//Added
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if(action == GLFW_PRESS){
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
        break;
      case GLFW_KEY_J:
        break;
      /*F for fullscreen*/
      case GLFW_KEY_F:
        break;
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        break;
      /*Otherwise*/
      default:
        break;
      }
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

  glUniform4fv(glGetUniformLocation(idProgramShader, "cameraPosition"), 1, value_ptr(cameraPosition));
  glUniform1f(glGetUniformLocation(idProgramShader, "heightFactor"), heightFactor);
  glUniform1f(glGetUniformLocation(idProgramShader, "widthTexture"), widthTexture);
  glUniform1f(glGetUniformLocation(idProgramShader, "heightTexture"), heightTexture);

  glUniform1i(glGetUniformLocation(idProgramShader, "rgbTexture"), 0);

  float *vertices=new float[(widthTexture+1)*(heightTexture+1)*3];
  unsigned int *indices=new unsigned int[widthTexture*heightTexture*6];
  generateHeightMap(vertices, indices);

  unsigned int VBO, EBO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  delete [] vertices;
  delete [] indices;

  while (!glfwWindowShouldClose(win)) {
    float ratio;
    int width, height;
    glfwGetFramebufferSize(win, &width, &height);
    ratio = width / (float) height;

    mat4 proj = perspective(radians(45.0f), ratio, 0.1f, 1000.0f);
    glViewport(0, 0, width, height);

    vec3 d = vec3(cameraPosition);
    d.z = d.z + 1;
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    mat4 view = lookAt(vec3(cameraPosition), d, up);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    gluLookAt(cameraPosition.x, cameraPosition.y, cameraPosition.z,
//        cameraPosition.x, cameraPosition.y, cameraPosition.z + 1,
//        0, 1, 0);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluPerspective(radians(45.0f), ratio, 0.1f, 1000.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    mat4 mvp = proj * view;
    glUniformMatrix4fv(glGetUniformLocation(idProgramShader, "MVP"), 1, GL_FALSE, value_ptr(mvp));
    glDrawElements(GL_TRIANGLES, (widthTexture+1)*(heightTexture+1)*3, GL_UNSIGNED_INT, 0);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwDestroyWindow(win);
  glfwTerminate();
  return 0;
}
