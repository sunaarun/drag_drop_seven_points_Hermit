
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define WIDTH 640
#define HEIGHT 640

GLuint vao, vbo,vao2,vbo2, vbo3,vao3;
GLfloat t;
GLint i;
GLfloat step = 3.0f / 99.0f; // t \in [-1,2]

GLfloat points[21] = { -0.75f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.1f, 0.2f, 0.0f,0.2f,
0.3f, 0.0f,0.4f, 0.5f, 0.0f, 0.6f , 0.7f, 0.0f };
GLfloat HermiteCurve[300];
GLfloat newHermiteCurve[300];

GLint dragged = -1;

void drawHermit() {
    HermiteCurve[0] = points[0];
    HermiteCurve[1] = points[1];
    HermiteCurve[2] = 0.0f;
    for (i = 1; i < 100; i++) {
        t = -1 + i * step;
        HermiteCurve[i * 3] = 
            points[0] * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) +
            points[3] * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) +
            points[6] * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) +
            points[9] * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        HermiteCurve[i * 3 + 1] =
            points[1] * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) +
            points[4] * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) +
            points[7] * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) +
            points[10] * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        HermiteCurve[i * 3 + 2] = 0.0f;
    }

    HermiteCurve[297] = points[9];
    HermiteCurve[298] = points[10];
    HermiteCurve[299] = 0.0f;

}

void new_drawHermit() {
    newHermiteCurve[0] = points[9];
    newHermiteCurve[1] = points[10];
    newHermiteCurve[2] = 0.0f;
    for (i = 1; i < 100; i++) {
        t = -1 + i * step;
        newHermiteCurve[i * 3] =
            points[9] * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) +
            points[12] * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) +
            points[15] * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) +
            points[18] * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        newHermiteCurve[i * 3 + 1] = 
            points[10] * (-1.0f / 6.0f * t * t * t + 1.0f / 2.0f * t * t - 1.0f / 3.0f * t) +
            points[13] * (1.0f / 2.0f * t * t * t - t * t - 1.0f / 2.0f * t + 1) +
            points[16] * (-1.0f / 2.0f * t * t * t + 1.0f / 2.0f * t * t + t) +
            points[19] * (1.0f / 6.0f * t * t * t - 1.0f / 6.0f * t);
        newHermiteCurve[i * 3 + 2] = 0.0f;
    }

    newHermiteCurve[297] = points[18];
    newHermiteCurve[298] = points[19];
    newHermiteCurve[299] = 0.0f;

}
GLfloat dist2_2d(GLfloat P1x, GLfloat P1y, GLfloat P2x, GLfloat P2y) {

    GLfloat dx = P1x - P2x;
    GLfloat dy = P1y - P2y;
    return dx * dx + dy * dy;
}

GLint getActivePoint(GLfloat* p, GLfloat sensitivity, GLfloat x, GLfloat y) {

    GLfloat		s = sensitivity * sensitivity;
    GLfloat		xNorm = -1 + x / (WIDTH / 2);
    GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

    for (GLint i = 0; i < 7; i++)
        if (dist2_2d(p[i * 3], p[i * 3 + 1], xNorm, yNorm) < s)
            return i;

    return -1;
}

void cursorPosCallback(GLFWwindow* window, double x, double y) {

    if (dragged >= 0) {

        GLfloat		xNorm = -1 + x / (WIDTH / 2);
        GLfloat		yNorm = -1 + (HEIGHT - y) / (HEIGHT / 2);

        points[3 * dragged] = xNorm;  // x coord
        points[3 * dragged + 1] = yNorm;  // y coord

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, 21 * sizeof(GLfloat), points, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        
        drawHermit();
        glBindBuffer(GL_ARRAY_BUFFER, vbo2);
        glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        new_drawHermit();
        glBindBuffer(GL_ARRAY_BUFFER, vbo3);
        glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(GLfloat), newHermiteCurve, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double	x, y;

        glfwGetCursorPos(window, &x, &y);
        dragged = getActivePoint(points, 0.1f, x, y);
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        dragged = -1;
}


int main() {
    GLFWwindow* window = NULL;
    const GLubyte* renderer;
    const GLubyte* version;

    const char* vertex_shader =
        "#version 330\n"
        "in vec3 vp;"
        "void main () {"
        "  gl_Position = vec4(vp, 1.0);"
        "}";

    const char* fragment_shader =
        "#version 330\n"
        "out vec4 frag_colour;"
        "void main () {"
        "  frag_colour = vec4(0.0, 1.0, 1.0, 1.0);"// This line to change the color or the printed line or points
        "}";

    GLuint vert_shader, frag_shader;
    GLuint shader_programme;

    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Drag&Drop", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    drawHermit();
    new_drawHermit();
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(GLfloat), HermiteCurve, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //----------------------------------------------------


    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 21 * sizeof(GLfloat), points, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //----------------------------------------------------------------
   
    glGenBuffers(1, &vbo3);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(GLfloat), newHermiteCurve, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao3);
    glBindVertexArray(vao3);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo3);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //---------------------------------------------------------------
    vert_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert_shader, 1, &vertex_shader, NULL);
    glCompileShader(vert_shader);

    frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &fragment_shader, NULL);
    glCompileShader(frag_shader);

    shader_programme = glCreateProgram();
    glAttachShader(shader_programme, frag_shader);
    glAttachShader(shader_programme, vert_shader);
    glLinkProgram(shader_programme);

    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    glPointSize(15.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_programme);
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, 7);
        //glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(vao2);
        glDrawArrays(GL_LINE_STRIP, 0, 100);
        glBindVertexArray(vao3);
       glDrawArrays(GL_LINE_STRIP, 0, 100);
        glBindVertexArray(0);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
