#ifndef RENDER_MATERIAL_SETUPER_H_
#define RENDER_MATERIAL_SETUPER_H_

#include <string>
#include <vector>
#include "base/micros.h"

class MaterialSetuper {
public:
    MaterialSetuper();
    ~MaterialSetuper();

public:
    void Reset();
    void SetHasUV(bool has_uv);
    void SetHasNormal(bool has_normal);
    void SetHasTexture(bool has_texture);

    void SetupMaterial(const std::string& material_name);

private:
    void SetupMaterialModel(const std::string& material_name);
    void SetupMaterialLight(const std::string& material_name);
    void SetupMaterialCamera(const std::string& material_name);

private:
    bool has_uv_;
    bool has_normal_;
    bool has_texture_;

private:
    DISALLOW_COPY_AND_ASSIGN(MaterialSetuper)
};

#endif  // RENDER_MATERIAL_SETUPER_H_
