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
  //Added
  //glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
  //glUniform1i(glGetUniformLocation(idProgramShader, "texturemap"), 0);

  initTexture(argv[1], & widthTexture, & heightTexture);

  while (!glfwWindowShouldClose(win)) {
    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}