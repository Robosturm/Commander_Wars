#include "ShaderProgramGL.h"
#include "VertexDeclarationGL.h"
#include "../gamewindow.h"

namespace oxygine
{
    ShaderProgramGL::ShaderProgramGL(GLuint p) : _program(p)
    {

    }


    ShaderProgramGL::ShaderProgramGL(GLuint vs, GLuint ps, const VertexDeclarationGL* decl)
    {
        _program = createProgram(vs, ps, decl);
    }

    ShaderProgramGL::~ShaderProgramGL()
    {
        if (_program)
        {
            GameWindow* window = oxygine::GameWindow::getWindow();
            window->glDeleteProgram(_program);
        }
    }

    unsigned int ShaderProgramGL::getID() const
    {
        return _program;
    }

    int ShaderProgramGL::getUniformLocation(const char* id) const
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int i = window->glGetUniformLocation(_program, id);
        //if (i == -1)
        return i;
    }

    bool ShaderProgramGL::getShaderBuildLog(GLuint shader, std::string& str)
    {
        GLint length = 0;
        GLint success = GL_TRUE;
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length)
        {
            str.resize(length);
            window->glGetShaderInfoLog(shader, (int)str.size(), NULL, &str[0]);
        }
        else
            str.clear();

        GLint status = GL_TRUE;
        window->glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

        return status == GL_TRUE;
    }


    bool ShaderProgramGL::getProgramBuildLog(GLuint program, std::string& str)
    {
        GLint length = 0;
        GLint success = GL_TRUE;
        GameWindow* window = oxygine::GameWindow::getWindow();
        window->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length)
        {
            str.resize(length);
            window->glGetProgramInfoLog(program, (int)str.size(), NULL, &str[0]);
        }
        else
            str.clear();

        GLint status = GL_TRUE;
        window->glGetProgramiv(program, GL_LINK_STATUS, &status);

        return status == GL_TRUE;

    }

    unsigned int ShaderProgramGL::createShader(unsigned int type, const char* data, const char* prepend, const char* append, error_policy ep)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        GLuint shader = window->glCreateShader(type);

        const char* sources[16];
        const char** ptr = &sources[0];

        bool gles = false;
        if (!gles)
        {
            //qDebug("not gles version");

            static const char nonGLES[] =
                "#define lowp\n"
                "#define mediump\n"
                "#define highp\n";

            *ptr = nonGLES;
            ptr++;
        }

        if (prepend)
        {
            *ptr = prepend;
            ptr++;
        }

        *ptr = data;
        ptr++;

        if (append)
        {
            *ptr = append;
            ptr++;
        }

        int num = (int)(ptr - sources);
        window->glShaderSource(shader, num, sources, 0);
        window->glCompileShader(shader);

        std::string log;
        bool success = getShaderBuildLog(shader, log);

        if (success)
        {
            qDebug("compiled shader: %s", log.c_str());
        }
        else
        {
            handleErrorPolicy(ep, "can't compile shader: %s", log.c_str());

            qDebug("shader source code:");
            for (int i = 0; i < num; ++i)
                qDebug(sources[i]);
            qDebug(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

        }

        return shader;
    }


    unsigned int ShaderProgramGL::createProgram(int vs, int fs, const VertexDeclarationGL* decl, bool deleteAttachedShaders)
    {
        GameWindow* window = oxygine::GameWindow::getWindow();
        int p = window->glCreateProgram();
        window->glAttachShader(p, vs);
        window->glAttachShader(p, fs);

        for (int i = 0; i < decl->numElements; ++i)
        {
            window->glBindAttribLocation(p, decl->elements[i].index, decl->elements[i].name.toStdString().c_str());
        }

        window->glLinkProgram(p);


        std::string log;
        bool success = getProgramBuildLog(p, log);

        if (success)
        {
            //qDebug("compiled shader: %s", log.c_str());
            window->glDetachShader(p, vs);
            window->glDetachShader(p, fs);
            if (deleteAttachedShaders)
            {
                window->glDeleteShader(vs);
                window->glDeleteShader(fs);
            }
        }
        else
        {
            qCritical("can't link gl program: %s", log.c_str());
            window->glDeleteProgram(p);
            p = 0;
        }

        return p;
    }
}
