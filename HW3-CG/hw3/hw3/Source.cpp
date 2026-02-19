#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// ===============================
// „ €Ì—«  «· Õﬂ„ (Global)
// ===============================
float moveY = 0.0f;
float alphaValue = 0.6f;
float zOffset = 0.0f;
int colorMode = 0;
bool wireframeMode = false;
bool wireKeyPressed = false; // Õ Ï „« Ì ﬂ—— »”—⁄…


// ==========================================
// [ ⁄œÌ· —ﬁ„ 1]:  ÕœÌÀ Vertex Shader
// ›Ì «·ﬂÊœ «·√’·Ì: ﬂ‰« ‰„—— «·„Êﬁ⁄ ›ﬁÿ.
// «· ⁄œÌ·: «” ﬁ»·‰« "«··Ê‰" (location 1) ·‰„——Â ··‘Ìœ— «· «·Ì.
// ==========================================
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"uniform float moveY;\n"        // Õ—ﬂ… ⁄„ÊœÌ…
"uniform float zOffset;\n"      //  Õﬂ„ »„ÕÊ— Z
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y + moveY, aPos.z + zOffset, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

// ==========================================
// [ ⁄œÌ· —ﬁ„ 2]:  ÕœÌÀ Fragment Shader
// ›Ì «·ﬂÊœ «·√’·Ì: ﬂ«‰ «··Ê‰ À«» « (»— ﬁ«·Ì).
// «· ⁄œÌ·: «” ﬁ»«· «··Ê‰ „‰ «·‹ Vertex + „ €Ì— ··‘›«›Ì… (Alpha).
// ==========================================
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"uniform float uAlpha;\n"
"uniform int colorMode;\n"
"void main()\n"
"{\n"
"   vec3 finalColor = ourColor;\n"
"   if(colorMode == 1)\n"
"       finalColor = vec3(0.0, 1.0, 0.0); // √Œ÷—\n"
"   if(colorMode == 2)\n"
"       finalColor = vec3(1.0, 1.0, 0.0); // √’›—\n"
"   FragColor = vec4(finalColor, uAlpha);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Õ—ﬂ…
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveY += 0.002f;

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveY -= 0.002f;

    // “— ≈⁄«œ… ··Ê÷⁄ «·√’·Ì
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        moveY = 0.0f;
        zOffset = 0.0f;
        alphaValue = 0.6f;
    }

    // “Ì«œ… «·‘›«›Ì…
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        alphaValue += 0.02f;
        if (alphaValue > 1.0f) alphaValue = 1.0f;
    }

    //  ﬁ·Ì· «·‘›«›Ì…
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        alphaValue -= 0.005f;
        if (alphaValue < 0.0f) alphaValue = 0.0f;
    }

    //  €ÌÌ— «··Ê‰
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        colorMode++;
        if (colorMode > 2)
            colorMode = 0;
    }
    //  »œÌ· Ê÷⁄ «·—”„ »“— M
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !wireKeyPressed)
    {
        wireframeMode = !wireframeMode;
        wireKeyPressed = true;
    }

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
    {
        wireKeyPressed = false;
    }

}



int main()
{
    // ---  ÂÌ∆… GLFW (‰›” «·ﬂÊœ «·√’·Ì) ---
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef APPLE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 4: Advanced Control", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ---  ÂÌ∆… GLEW (‰›” «·ﬂÊœ «·√’·Ì) ---
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // ==========================================
    // [ ⁄œÌ· —ﬁ„ 4]:  ›⁄Ì· ﬁœ—«  OpenGL «·„ ﬁœ„…
    // «·„ﬂ«‰: ÌÃ» √‰  ÷«› Â‰« (»⁄œ glewInit Êﬁ»· Õ·ﬁ… «·—”„).
    // ==========================================
    glEnable(GL_DEPTH_TEST); //  ›⁄Ì· «Œ »«— «·⁄„ﬁ (·„‰⁄  œ«Œ· «·—”„)
    glEnable(GL_BLEND);      //  ›⁄Ì· «·œ„Ã (··‘›«›Ì…)
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // „⁄«œ·… «·œ„Ã

    // --- »‰«¡ Ê Ã„Ì⁄ «·‘Ìœ— (‰›” «·ŒÿÊ«  «·√’·Ì…) ---
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader); // ( „ «Œ ’«— ›Õ’ «·√Œÿ«¡ ·· —ﬂÌ“)

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ==========================================
    // [ ⁄œÌ· —ﬁ„ 5]:  ÕœÌÀ «·»Ì«‰«  («·„À·À« )
// «· €ÌÌ—: »œ·« „‰ 3 —ƒÊ”° √’»Õ ·œÌ‰« 6 —ƒÊ” („À·ÀÌ‰).
    // «·≈÷«›…: √÷›‰« 3 ﬁÌ„ ··√·Ê«‰ (RGB) »Ã«‰» ﬂ· „Êﬁ⁄ (XYZ).
    // «· ‰”Ìﬁ «·ÃœÌœ ··”ÿ—: X, Y, Z, R, G, B
    // ==========================================
float vertices[] = {
    // «·„À·À «·√Ê· (√Õ„— - ﬁ—Ì» Z=0.0)
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
     0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,

     // «·„À·À «·À«‰Ì (√“—ﬁ - »⁄Ìœ Z=0.5)
     -0.2f, -0.2f, 0.5f,  0.0f, 0.0f, 1.0f,
      0.8f, -0.2f, 0.5f,  0.0f, 0.0f, 1.0f,
      0.3f,  0.8f, 0.5f,  0.0f, 0.0f, 1.0f
};

unsigned int VBO, VAO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

// ==========================================
// [ ⁄œÌ· —ﬁ„ 6]:  ÕœÌÀ ﬁ—«¡… «·»Ì«‰«  (Stride & Pointers)
// «·”»»:  €Ì— ÕÃ„ «·»Ì«‰«  ›Ì «·„’›Ê›… √⁄·«Â.
// ==========================================

// «·”„… 0: «·„Êﬁ⁄ (3 floats) | «·ŒÿÊ… (Stride) √’»Õ  6 * float
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// «·”„… 1: «··Ê‰ (3 floats) |  »œ√ »⁄œ √Ê· 3 √—ﬁ«„ (Offset)
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
glEnableVertexAttribArray(1);

// --- Õ·ﬁ… «·—”„ ---
while (!glfwWindowShouldClose(window))

{
    processInput(window);

    // ==========================================
    // [ ⁄œÌ· —ﬁ„ 7]:  ‰ŸÌ› «·‹ Buffer
    // «·≈÷«›…: ÌÃ» „”Õ GL_DEPTH_BUFFER_BIT ›Ì ﬂ· ›—Ì„.
    // ==========================================
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (wireframeMode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  
    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "colorMode");
    glUniform1i(colorLoc, colorMode);

    int moveLoc = glGetUniformLocation(shaderProgram, "moveY");
    glUniform1f(moveLoc, moveY);

    int zLoc = glGetUniformLocation(shaderProgram, "zOffset");
    glUniform1f(zLoc, zOffset);

    glBindVertexArray(VAO);

    // ==========================================
    // [ ⁄œÌ· —ﬁ„ 8]: «·—”„ «·„‰›’· Ê«· Õﬂ„ »«·„ €Ì—« 
    // »œ·« „‰ —”„… Ê«Õœ… (glDrawArrays)° ‰—”„ ⁄·Ï „—Õ· Ì‰ · €ÌÌ— «·‘›«›Ì….
    // ==========================================

    // «·Õ’Ê· ⁄·Ï „Êﬁ⁄ „ €Ì— «·‘›«›Ì…
    int alphaLoc = glGetUniformLocation(shaderProgram, "uAlpha");

    // √. —”„ «·„À·À «·√“—ﬁ («·»⁄Ìœ) √Ê·«
    glUniform1f(alphaLoc, 1.0f); // ﬁÌ„… √·›« = 1 (€Ì— ‘›«›)
    glDrawArrays(GL_TRIANGLES, 3, 3); // «»œ√ „‰ «·—√” 3° «—”„ 3 —ƒÊ”

    // ». —”„ «·„À·À «·√Õ„— («·ﬁ—Ì») À«‰Ì«
    glUniform1f(alphaLoc, alphaValue);
    glDrawArrays(GL_TRIANGLES, 0, 3); // «»œ√ „‰ «·—√” 0° «—”„ 3 —ƒÊ”

    glfwSwapBuffers(window);
    glfwPollEvents();
}

glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteProgram(shaderProgram);

glfwTerminate();
return 0;
}