#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "concave_hull.h"
using namespace std;

#pragma comment(lib, "opengl32.lib")

// функция вывода экрана с картинкой
int init_window(vector <list <Segment>> alpha_shape, int index_orig_obs);

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Константы
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;