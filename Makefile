FLAGS=-lXi -lGLEW -lGLU -lm -lGL -lm -lpthread -ldl -ldrm -lXdamage -lX11-xcb -lxcb-glx -lxcb-dri2 -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -ljpeg
GLM=-I ./glm
all:
	g++ *.cpp -o hw3 -std=c++11 $(FLAGS) $(GLM)
debug:
	g++ *.cpp -o hw3 -std=c++11 $(FLAGS) $(GLM) -g

