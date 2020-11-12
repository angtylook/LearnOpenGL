#include "light_setuper.h"
#include "application.h"
#include "model/light.h"
#include "shader_manager.h"

const int kMaxPointLight = 4;
const int kMaxDirectLight = 1;
const int kMaxSpotLight = 1;

using namespace engine;
LightSetuper::LightSetuper()
    : point_light_count_(0), direct_light_count_(0), spot_light_count_(0) {}

LightSetuper::~LightSetuper() {}

void LightSetuper::Reset() {
    point_light_count_ = 0;
    direct_light_count_ = 0;
    spot_light_count_ = 0;
}

void LightSetuper::SetupLightPont(Light* light) {
    if (point_light_count_ >= kMaxPointLight) {
        fprintf(stderr, "too much point lights.\n");
        return;
    }
    std::string point_light("point_light[" +
                            std::to_string(point_light_count_) + "].");
    application->shader_manager()->SetShaderUniform(point_light + "position",
                                                    light->GetPosition());
    // distance reduce params
    application->shader_manager()->SetShaderUniform(point_light + "constant",
                                                    light->GetConstant());
    application->shader_manager()->SetShaderUniform(point_light + "linear",
                                                    light->GetLinear());
    application->shader_manager()->SetShaderUniform(point_light + "quadratic",
                                                    light->GetQuadratic());
    // light's color
    application->shader_manager()->SetShaderUniform(point_light + "ambient",
                                                    light->GetAmbient());
    application->shader_manager()->SetShaderUniform(point_light + "diffuse",
                                                    light->GetDiffuse());
    application->shader_manager()->SetShaderUniform(point_light + "specular",
                                                    light->GetSpecular());
    point_light_count_++;
}

void LightSetuper::SetupLightDirect(Light* light) {
    if (direct_light_count_ >= kMaxDirectLight) {
        fprintf(stderr, "too much direct lights.\n");
        return;
    }
    std::string direct_light("direct_light[" +
                             std::to_string(direct_light_count_) + "].");
    application->shader_manager()->SetShaderUniform(direct_light + "direction",
                                                    light->GetDirection());
    // light's color
    application->shader_manager()->SetShaderUniform(direct_light + "ambient",
                                                    light->GetAmbient());
    application->shader_manager()->SetShaderUniform(direct_light + "diffuse",
                                                    light->GetDiffuse());
    application->shader_manager()->SetShaderUniform(direct_light + "specular",
                                                    light->GetSpecular());
    direct_light_count_++;
}

void LightSetuper::SetupLightSpot(Light* light) {
    if (spot_light_count_ >= kMaxSpotLight) {
        fprintf(stderr, "too much spot lights.\n");
        return;
    }
    std::string spot_light("spot_light[" + std::to_string(spot_light_count_) +
                           "].");
    application->shader_manager()->SetShaderUniform(spot_light + "position",
                                                    light->GetPosition());
    application->shader_manager()->SetShaderUniform(spot_light + "direction",
                                                    light->GetDirection());
    // distance reduce params
    application->shader_manager()->SetShaderUniform(spot_light + "constant",
                                                    light->GetConstant());
    application->shader_manager()->SetShaderUniform(spot_light + "linear",
                                                    light->GetLinear());
    application->shader_manager()->SetShaderUniform(spot_light + "quadratic",
                                                    light->GetQuadratic());
    // cut off
    application->shader_manager()->SetShaderUniform(
        spot_light + "inner_cut_off", light->GetInnerCut());
    application->shader_manager()->SetShaderUniform(
        spot_light + "outer_cut_off", light->GetOuterCut());
    // light's color
    application->shader_manager()->SetShaderUniform(spot_light + "ambient",
                                                    light->GetAmbient());
    application->shader_manager()->SetShaderUniform(spot_light + "diffuse",
                                                    light->GetDiffuse());
    application->shader_manager()->SetShaderUniform(spot_light + "specular",
                                                    light->GetSpecular());
    spot_light_count_++;
}

void LightSetuper::AfterSetup() {
    if (point_light_count_ == 0 && direct_light_count_ == 0 &&
        spot_light_count_ == 0) {
        application->shader_manager()->SetShaderUniform("enable_light",
                                                        (GLint)0);
    } else {
        application->shader_manager()->SetShaderUniform("enable_light",
                                                        (GLint)1);
    }
    application->shader_manager()->SetShaderUniform("point_light_counter",
                                                    (GLint)point_light_count_);
    application->shader_manager()->SetShaderUniform("direct_light_counter",
                                                    (GLint)direct_light_count_);
    application->shader_manager()->SetShaderUniform("spot_light_counter",
                                                    (GLint)spot_light_count_);
}
