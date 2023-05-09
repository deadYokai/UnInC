
#include <shader.h>
#include <set>
#include <algorithm>

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(installer);

Shader::Shader(){}

void Shader::init(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{

    auto fs = cmrc::installer::get_filesystem();    
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try 
    {
        auto vSh = fs.open(vertexPath);
        auto fSh = fs.open(fragmentPath);
        
        vertexCode = std::string(vSh.begin(), vSh.end());
        fragmentCode = std::string(fSh.begin(), fSh.end());
        // if geometry shader path is present, also load a geometry shader
        if(geometryPath != nullptr)
        {

            auto gSh = fs.open(geometryPath);
            geometryCode = std::string(gSh.begin(), gSh.end());
        }
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char * vShaderCode = vertexCode.c_str();
    const char * fShaderCode = fragmentCode.c_str();
    // 2. compile shaders
    unsigned int vertex, fragment;
    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // if geometry shader is given, compile geometry shader
    unsigned int geometry;
    if(geometryPath != nullptr)
    {
        const char * gShaderCode = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER_BIT);
        glShaderSource(geometry, 1, &gShaderCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    // shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    if(geometryPath != nullptr)
        glAttachShader(ID, geometry);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if(geometryPath != nullptr)
        glDeleteShader(geometry);

}

