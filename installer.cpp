#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#define GLFW_INCLUDE_NONE
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <memory>

#include <shader.h>
#include <rect.h>
#include <text.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void RenderText(wchar_t* text, float x, float y, float scale, glm::vec3 color);

void character_callback(GLFWwindow *window, unsigned int codepoint);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

std::map<wchar_t, Character> Characters;
unsigned int textVAO, textVBO;

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(installer);
auto fs = cmrc::installer::get_filesystem();

Shader textShader;
Text inpText;

void InitFreeType2(){
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    textShader.use();
    glUniformMatrix4fv(glGetUniformLocation(textShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // FreeType
    // --------
    FT_Library ft;
    // All functions return a value different than 0 whenever an error occurred
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        exit(-1);
    }

	// find path to font
    std::string fpath = "res/e-Ukraine-Light.ttf";
    cmrc::file font_name = fs.open(fpath);
    // if (fs.exists(fpath))
    // {
    //     std::cout << "ERROR::FREETYPE: Failed to load font_name" << std::endl;
    //     exit(-1);
    // }

    std::string fstr = std::string(font_name.begin(), font_name.end());

    FT_Byte* fbytes = const_cast<FT_Byte*>(reinterpret_cast<const FT_Byte*>(fstr.c_str()));

	// load font as face
    FT_Face face;
    if (FT_New_Memory_Face(ft, fbytes, fstr.size(), 0, &face)) {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        exit(-1);
    }
    else {

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 32);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_Select_Charmap(face, FT_ENCODING_UNICODE);

        for(int i = 0; i < 2000; i++)
        {
            auto err = FT_Load_Char(face, i, FT_LOAD_RENDER);
            if (err)
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph: " << err << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<wchar_t, Character>(i, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

int main(int argc, char** argv)
{
    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "deadYokai встановшик", NULL, NULL);
    if (!window){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetCharCallback(window, character_callback);
    glfwSetKeyCallback(window, key_callback);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    gladLoadGL(glfwGetProcAddress);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textShader.init("res/text.vs", "res/text.fs");

    InitFreeType2();
    
    Text hellotext;
    hellotext.init(textShader, Characters, 50.0f, 50.0f, 1.0f, glm::vec3(1, 1, 1), textVAO, textVBO);
    hellotext.setText(L"Привіт, цей чудовий світ!");

    inpText.init(textShader, Characters, 55.0f, SCR_HEIGHT/2+((50+32)/2), 1.0f, glm::vec3(1, 1, 1), textVAO, textVBO);

    Rect s;
    s.init(window, 50.0f, SCR_HEIGHT/2+50.0f/2, SCR_WIDTH-100.0f, 50.0f, glm::vec3(0.3f, 0.3f, 0.3f));

    while(!glfwWindowShouldClose(window)){

        processInput(window);

        glClearColor(0.16f, 0.16f, 0.16f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        s.Render();

        hellotext.Render();
        inpText.Render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods){
    if(!glfwWindowShouldClose(window)){
        if(key == GLFW_KEY_BACKSPACE && action == GLFW_PRESS){
            std::wstring ws = inpText.getText();
            
            if(!ws.empty())
                ws.pop_back();
                
            inpText.setText(ws);
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void character_callback( GLFWwindow *window, unsigned int codepoint )
{
    if(!glfwWindowShouldClose(window)){
        std::wstring ws = inpText.getText();
        wchar_t b = static_cast<wchar_t>(codepoint);
        inpText.setText(ws + b);
    }
}
