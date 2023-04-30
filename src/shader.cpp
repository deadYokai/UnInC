
#include <shader.h>
#include <set>
#include <algorithm>

#include <res.h>

std::string set_to_string(std::set<int> set) {
    std::stringstream out;
        
    if (!set.empty())
    {
        for_each(
            set.begin(), 
            --set.end(), 
            [&out](const int &value) 
            {
                out << value;
            }
        );
        
        out << *(--set.end());
    }
    
    out.flush();
    
    return out.str();
}

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    // ensure ifstream objects can throw exceptions:
    // vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    // fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    // gShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try 
    {

        // auto fs = cmrc::installer::get_filesystem();
        // open files
        auto vSh = fs.open(vertexPath);
        auto fSh = fs.open(fragmentPath);
        auto vSh_vec = std::vector<unsigned char>(vSh.begin(), vSh.end());
        auto fSh_vec = std::vector<unsigned char>(fSh.begin(), fSh.end());

        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        // vShaderStream.read(vSh_vec.data(), vSh_vec.size());
        // vShaderStream << vSh_vec;
        // fShaderStream.read(fSh_vec.data(), fSh_vec.size());
        // close file handlers
        // vShaderFile.close();
        // fShaderFile.close();
        // convert stream into string
        // vertexCode = vShaderStream.str();
        // fragmentCode = fShaderStream.str();			
        // if geometry shader path is present, also load a geometry shader
        if(geometryPath != nullptr)
        {

            auto gSh = fs.open(geometryPath);
            // auto gShaderFile = memstream ( const_cast<char*>(gSh.begin()),
            //                         const_cast<char*>(gSh.end()) );
            // std::stringstream gShaderStream;
            // gShaderStream << gShaderFile.rdbuf();
            // gShaderFile.close();
            // geometryCode = gShaderStream.str();
        }
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
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

