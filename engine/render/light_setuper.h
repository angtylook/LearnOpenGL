#ifndef SCENE_LIGHT_SETUPER_H_
#define SCENE_LIGHT_SETUPER_H_

#include "base/micros.h"
#include "model/light.h"

class LightSetuper {
public:
    LightSetuper();
    ~LightSetuper();

public:
    void Reset();
    void SetupLightPont(Light* light);
    void SetupLightDirect(Light* light);
    void SetupLightSpot(Light* light);
    void AfterSetup();

private:
    int point_light_count_;
    int direct_light_count_;
    int spot_light_count_;

private:
    DISALLOW_COPY_AND_ASSIGN(LightSetuper)
};

#endif  // SCENE_LIGHT_SETUPER_H_
