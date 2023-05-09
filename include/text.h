
#include <glad/gl.h>
#include <glm/glm.hpp>

#include <map>
#include <shader.h>

struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class Text{
    public:
        Text();
        void init(Shader s, std::map<wchar_t, Character> &Characters, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO);
        void setXpos(float x);
        void setYpos(float y);
        void setPos(float x, float y);
        void setColor(glm::vec3 color);
        void setText(wchar_t* text);
        void setText(std::wstring text);
        void setText(std::string text);
        void setText(const char* text);
        std::wstring getText();
        void Render();
    private:
        void RenderText(Shader textShader, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO, std::map<wchar_t, Character> Characters);
        std::wstring text;
        glm::vec3 color;
        float xPos, yPos, scale;
        unsigned int VAO, VBO;
        Shader textShader;
        std::map<wchar_t, Character> Characters;
};
