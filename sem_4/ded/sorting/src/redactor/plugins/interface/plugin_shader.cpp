#include "plugin_shader.h"


PluginShader::PluginShader(const char *code, RShader::Type type, bool is_code) {
    if (is_code) {
        if (rshader.loadFromMemory(code, type)) {
            status = 0;
        }
    } else {
        if (rshader.loadFromFile(code, type)) {
            status = 0;
        }
    }
}

void PluginShader::set_uniform_int(const char *name, int  val) {
    rshader.setUniform(name, val);
}

void PluginShader::set_uniform_int_arr(const char *name, int *val, size_t cnt) {
    for (size_t i = 0; i < cnt; ++i) {
        ((float*)val)[i] = (float) val[i];
    }

    rshader.setUniformArray(name, (float*) val, cnt);

    for (size_t i = 0; i < cnt; ++i) {
        val[i] = (int) ((float*)val)[i];
    }
}


void PluginShader::set_uniform_float(const char *name, float  val) {
    rshader.setUniform(name, val);
}

void PluginShader::set_uniform_float_arr(const char *name, float *val, size_t cnt) {
    rshader.setUniformArray(name, val, cnt);
}

bool PluginShader::is_ok() {
    return status == 0;
}
