
#include <text.h>

Text::Text(){}

void Text::init(Shader s, std::map<wchar_t, Character> &Characters, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO){
    this->textShader = s;
    this->Characters = Characters;
    this->setXpos(x);
    this->setYpos(y);
    this->setColor(color);
    this->scale = scale;
    this->VAO = VAO;
    this->VBO = VBO;
}

void Text::setXpos(float x){
    this->xPos = x;
};

void Text::setYpos(float y){
    this->yPos = y;
};

void Text::setPos(float x, float y){
    this->xPos = x;
    this->yPos = y;
};

void Text::setColor(glm::vec3 color){
    this->color = color;
};

void Text::setText(const char* text){
    std::string s(text);
    this->setText((std::string)s);
};

void Text::setText(std::string text){
    std::wstring t(text.begin(), text.end());
    this->setText((std::wstring)t);
};

void Text::setText(std::wstring text){
    this->text = text;
};

void Text::setText(wchar_t* text){
    std::wstring s(text);
    this->setText((std::wstring)s);
};


std::wstring Text::getText(){
    std::wstring text = this->text;
    // if(!text) text = L"";
    return text;
}

void Text::Render(){
    RenderText(
        this->textShader,
        this->xPos,
        this->yPos,
        this->scale,
        this->color,
        this->VAO,
        this->VBO,
        this->Characters
    );
}



void Text::RenderText(Shader textShader, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO, std::map<wchar_t, Character> Characters)
{
    std::wstring text = this->text;
    // if(!text) text = L"\0";
    // activate corresponding render state
    textShader.use();
    glUniform3f(glGetUniformLocation(textShader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    std::wstring::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

