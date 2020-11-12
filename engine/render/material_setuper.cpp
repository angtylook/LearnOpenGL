#include "material_setuper.h"
#include "application.h"
#include "material_manager.h"
#include "model/material.h"
#include "shader_manager.h"
#include "texture_manager.h"

using namespace engine;

MaterialSetuper::MaterialSetuper() {}

MaterialSetuper::~MaterialSetuper() {}

void MaterialSetuper::Reset() {
    has_uv_ = false;
    has_normal_ = false;
    has_texture_ = false;
}

void MaterialSetuper::SetHasUV(bool has_uv) {
    has_uv_ = has_uv;
}

void MaterialSetuper::SetHasNormal(bool has_normal) {
    has_normal_ = has_normal;
}
void MaterialSetuper::SetHasTexture(bool has_texture) {
    has_texture_ = has_texture;
}

void MaterialSetuper::SetupMaterial(const std::string& material_name) {
    std::string shader = application->shader_manager()->GetCurrentShaderName();
    if (shader == "model") {
        SetupMaterialModel(material_name);
    } else if (shader == "light") {
        SetupMaterialLight(material_name);
    } else if (shader == "camera") {
        SetupMaterialCamera(material_name);
    } else {
        fprintf(stderr, "unknown how to setup material for shader<%s>.\n",
                shader.c_str());
    }
}

void MaterialSetuper::SetupMaterialModel(const std::string& material_name) {
    Material* material = nullptr;
    if (material_name.empty()) {
        fprintf(stderr,
                "MaterialSetuper material name is empty, use default "
                "material<white>.\n");
        material = application->material_manager()->GetMaterial("white");
    }
    material = application->material_manager()->GetMaterial(material_name);
    if (has_normal_) {
        application->shader_manager()->SetShaderUniform("enable_light", 1);
    } else {
        application->shader_manager()->SetShaderUniform("enable_light", 0);
    }
    // no material
    if (!material) {
        fprintf(
            stderr,
            "MaterialSetuper material not found, use white color instead.\n");
        application->shader_manager()->SetShaderUniform("enable_material", 0);
        application->shader_manager()->SetShaderUniform("enable_texture", 0);
        application->shader_manager()->SetShaderUniform(
            "material_color.ambient", glm::vec4(1.0f));
        application->shader_manager()->SetShaderUniform(
            "material_color.diffuse", glm::vec4(1.0f));
        application->shader_manager()->SetShaderUniform(
            "material_color.specular", glm::vec4(1.0f));
        application->shader_manager()->SetShaderUniform(
            "material_color.shininess", 32);
        return;
    }
    // no texture uv
    if (!has_uv_ || !material->HasDiffuse()) {
        application->shader_manager()->SetShaderUniform("enable_material", 0);
        application->shader_manager()->SetShaderUniform("enable_texture", 0);
        application->shader_manager()->SetShaderUniform(
            "material_color.ambient", material->GetAmbientColor());
        application->shader_manager()->SetShaderUniform(
            "material_color.diffuse", material->GetDiffuseColor());
        application->shader_manager()->SetShaderUniform(
            "material_color.specular", material->GetSpecularColor());
        application->shader_manager()->SetShaderUniform(
            "material_color.shininess", material->GetShininess());
        return;
    }

    if (material->HasDiffuse()) {
        application->shader_manager()->SetShaderUniform("enable_material", 1);
        application->shader_manager()->SetShaderUniform("enable_texture", 1);
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, application->texture_manager()->GetTexture(
                                         material->GetDiffuse()));
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        application->shader_manager()->SetShaderUniform("material.diffuse", 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, application->texture_manager()->GetTexture(
                                         material->GetSpecular()));
        application->shader_manager()->SetShaderUniform("material.specular", 1);
        application->shader_manager()->SetShaderUniform(
            "material.shininess", material->GetShininess());
    }
}

void MaterialSetuper::SetupMaterialLight(const std::string& material_name) {
    Material* material = nullptr;
    if (material_name.empty()) {
        fprintf(stderr,
                "MaterialSetuper material name is empty, use default "
                "material<white>.\n");
        material = application->material_manager()->GetMaterial("white");
    }
    material = application->material_manager()->GetMaterial(material_name);
    if (has_uv_ && material->HasDiffuse()) {
        application->shader_manager()->SetShaderUniform("use_texture", 1);
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, application->texture_manager()->GetTexture(
                                         material->GetDiffuse()));
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        application->shader_manager()->SetShaderUniform("texture_sampler", 0);
    } else {
        application->shader_manager()->SetShaderUniform("use_texture", 0);
        application->shader_manager()->SetShaderUniform(
            "use_color", material->GetDiffuseColor());
    }
}

void MaterialSetuper::SetupMaterialCamera(const std::string& material_name) {
    Material* material = nullptr;
    if (material_name.empty()) {
        fprintf(stderr,
                "MaterialSetuper material name is empty, use default "
                "material<white>.\n");
        material = application->material_manager()->GetMaterial("white");
    }
    material = application->material_manager()->GetMaterial(material_name);
    if (has_uv_ && material->HasDiffuse()) {
        application->shader_manager()->SetShaderUniform("use_texture", 1);
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, application->texture_manager()->GetTexture(
                                         material->GetDiffuse()));
        // Set our "myTextureSampler" sampler to user Texture Unit 0
        application->shader_manager()->SetShaderUniform("texture_sampler", 0);
    } else {
        application->shader_manager()->SetShaderUniform("use_texture", 0);
        application->shader_manager()->SetShaderUniform(
            "use_color", material->GetDiffuseColor());
    }
}
