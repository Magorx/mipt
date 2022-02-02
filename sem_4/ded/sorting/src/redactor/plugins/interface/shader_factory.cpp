#include "shader_factory.h"


PUPPY::Shader *ShaderFactory::compile(const char *code, PUPPY::ShaderType type_, bool is_code) const {
    auto type = type_ == PUPPY::VERTEX ? RShader::Vertex : type_ == PUPPY::FRAGMENT ? RShader::Fragment : RShader::Geometry;
    if (type == RShader::Geometry) {
        return nullptr;
    }

    auto ret = new PluginShader(code, type, is_code);
    if (!ret->is_ok()) {
        delete ret;
        return nullptr;
    }

    return ret;
}
