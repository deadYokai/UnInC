
#include <glad/gl.h>
#include <glm/glm.hpp>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <shader.h>


class Rect{
    public:
        Rect();
        void init(GLFWwindow* window, float x, float y, float w, float h, glm::vec3 color);
        void setXpos(float x);
        void setYpos(float y);
        void setPos(float x, float y);
        void setSize(float w, float h);
        void setWidth(float w);
        void setHeight(float h);
        void setColor(glm::vec3 color);
        void Render();
    private:
        void RenderRect(float x, float y, float w, float h, glm::vec3 color);
        glm::vec3 color;
        float xPos, yPos, width, height;
        int wW, wH;
        unsigned int VAO, VBO, EBO;
        Shader shader;
};
