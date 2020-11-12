#ifndef MODEL_ANIMATION_H_
#define MODEL_ANIMATION_H_

#include <map>
#include <string>
#include "math/bounding_box.h"
#include "skeleton.h"

class Animation {
public:
    Animation();
    Animation(const std::string& name);
    void SetName(const std::string& name);

    bool IsLoop();
    void SetLoop(bool loop);

    int GetFrameRate();
    void SetFrameRate(int frame_rate);

    size_t GetAnimationFrames();
    //
    const Skeleton& GetCurrentSkeleton() const;
    void Update();

    // for loader to reserve space for frames
    void ReserveAnimationFrames(int frames);
    void AddFrame(const Skeleton& skeleton);
    // bounding box
    void AddBoundingBox(const BoundingBox& bounding);
    BoundingBox GetBoundingBox(int frame);

private:
    bool loop_;
    int frame_rate_;
    float anim_time_;
    std::string name_;
    Skeleton current_frame_;
    SkeletonList key_frames_;
    std::vector<BoundingBox> boundings_;
};

typedef std::map<std::string, Animation> AnimationMap;

#endif  // MODEL_ANIMATION_H_
