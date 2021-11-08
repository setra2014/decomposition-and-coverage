#include "graphic.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";




int init_window(vector <list <Segment>> alpha_shape, int index_orig_obs)
{
    // glfw: ������������� � ����������������
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // glfw: �������� ����
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: �������� ���� ���������� �� OpenGL-�������
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // �������������� ����� ��������� ���������

    // ��������� ������
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // �������� �� ������� ������ �������������� ���������� �������
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ����������� ������
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // �������� �� ������� ������ �������������� ������������ �������
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ���������� ��������
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // �������� �� ������� ������ �������������� ���������� ��������
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ���������� ������ (� �������) � ��������� ��������� ���������
    list <Segment>:: iterator iter;
    vector <float> vertices_vec;
    vector <list <Segment>> ::iterator iter_seg;
    int counter = 0;
    float color_red = 0.0f, color_green = 0.0f, color_blue = 0.0f;

    for (iter_seg = alpha_shape.begin(); iter_seg != alpha_shape.end(); iter_seg++) {
        /*if (counter < 9) {
            counter++;
            continue;
            color_red = 1.0f;
        }*/
        if (counter == 9) {
            color_red = 1.0f;
        }
        /*if (counter == 10) {
            color_red = 0.0f;
            color_green = 1.0f;
        }*/
        for (iter = (*iter_seg).begin(); iter != (*iter_seg).end(); iter++) {
           vertices_vec.push_back((*iter).start().x() / 130 - 0.6); vertices_vec.push_back(-(*iter).start().y() / 120 + 0.5); //vertices_vec.push_back(0);
           vertices_vec.push_back(color_red); vertices_vec.push_back(color_green); vertices_vec.push_back(color_blue);
           vertices_vec.push_back((*iter).end().x() / 130 - 0.6); vertices_vec.push_back(-(*iter).end().y() / 120 + 0.5); // vertices_vec.push_back(0);
           vertices_vec.push_back(color_red); vertices_vec.push_back(color_green); vertices_vec.push_back(color_blue);
           
        }
        counter++;
    }
    /*
    for (auto iter = angles.begin(); iter != angles.end(); iter++) {
        if (counter == 9) {
            color_red = 1.0f;
        }

        vertices_vec.push_back((*iter).x() / 130 - 0.6); vertices_vec.push_back(-(*iter).y() / 120 + 0.5); //vertices_vec.push_back(0);
        vertices_vec.push_back(color_red); vertices_vec.push_back(color_green); vertices_vec.push_back(color_blue);
    }
    */
    const int size = vertices_vec.size();
    float vertices[4000];
    copy(vertices_vec.begin(), vertices_vec.end(), vertices);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // ������� ��������� ������ ���������� �������, ����� ��������� � ������������� ��������� �����(�), � ����� ������������� ��������� �������(�)
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ������������ ��������
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // �������� ��������
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // �������� ��������, ��� ������ �������� ���������, ����� glVertexAttribPointer() ��������������� VBO ��� ����������� ��������� �������� ������ ��� ���������� ��������, ��� ��� ����� ����� �� ����� �������� ��������� �������
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    // �� ������ �������� �������� VAO ����� �����, ����� ������ ������ VAO �������� �� �������� ���� VAO (�� �������� �������� ����� ���������).
    // ����������� ������ VAO ������� ������ glBindVertexArray(), ������� �� ������ �� ������� �������� VAO (��� VBO), ����� ��� �� ��������� ��������
    //glBindVertexArray(0);

    // ���������������� ��������� ������ ��� ��������� ��������� � ������ �������
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(shaderProgram);

    // ���� ����������
    while (!glfwWindowShouldClose(window))
    {
        // ��������� �����
        processInput(window);

        // ���������
        glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ������ ��� ������ �����������

        glBindVertexArray(VAO); // ��������� � ��� ���� ������ ���� VAO, �� ��� ������������� ��������� ��� ������ ��� (�� �� ������� ���, ����� �� ���� ������� ��������������)
        glPointSize(10);
        glLineWidth(2);
        glDrawArrays(GL_LINES, 0, index_orig_obs*2);
        //glDrawArrays(GL_POINTS, index_orig_obs*2, 1000);

        glfwSwapBuffers(window);
        glfwPollEvents();
        Sleep(1000);
    }

    // �����������: ����������� ��� �������, ��� ������ ��� ��������� ���� ��������������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: ����������, ������������ ���� ����� ��������������� GLFW-��������
    glfwTerminate();
    return 0;
}

// ��������� ���� ������� �����: ������ GLFW � �������/���������� ������ �� ���������� � ������ ����� � ��������������� ��������� ������ �������
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: ������ ���, ����� ���������� ������� ���� (������������� ��� ������������ ��������), ���������� ������ callback-�������
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // ����������, ��� ���� ��������� ������������� ����� �������� ����.
    // �������� ��������, ������ ���� �� Retina-�������� ����� ����������� ������, ��� ������� � ���������
    glViewport(0, 0, width, height);
}