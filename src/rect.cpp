
#include <rect.h>


Rect::Rect(){}

void Rect::init(GLFWwindow* window, float x, float y, float w, float h, glm::vec3 color){
    Shader rectShader;
    rectShader.init("res/rect.vs", "res/rect.fs");
    this->shader = rectShader;
    this->setXpos(x);
    this->setYpos(y);
    this->setColor(color);
    this->width = w;
    this->height = h;
    glfwGetWindowSize(window, &this->wW, &this->wH);
    this->shader.use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(wW), 0.0f, static_cast<float>(wH));
    glUniformMatrix4fv(glGetUniformLocation(this->shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Rect::setXpos(float x){
    this->xPos = x;
};

void Rect::setYpos(float y){
    this->yPos = y;
};

void Rect::setPos(float x, float y){
    this->xPos = x;
    this->yPos = y;
};


void Rect::setSize(float w, float h){
    this->width = w;
    this->height = h;
};


void Rect::setColor(glm::vec3 color){
    this->color = color;
};

void Rect::Render(){
    RenderRect(
        this->xPos,
        this->yPos,
        this->width,
        this->height,
        this->color
    );
}


void Rect::RenderRect(float x, float y, float w, float h, glm::vec3 color)
{
    this->shader.use();
    shader.setVec3("rectColor", color.x, color.y, color.z);
    
    float xpos = x;
    float ypos = y;

    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }
    };

    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

