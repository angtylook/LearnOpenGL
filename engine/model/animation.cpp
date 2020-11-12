#include "animation.h"
#include "application.h"
#include "glm/glm.hpp"
#include "glm/gtx/compatibility.hpp"

Animation::Animation() : loop_(false), anim_time_(0) {}

Animation::Animation(const std::string& name)
    : loop_(false), anim_time_(0), name_(name) {}

void Animation::SetName(const std::string& name) {
    name_ = name;
}

void Animation::AddFrame(const Skeleton& skeleton) {
    key_frames_.push_back(skeleton);
}

bool Animation::IsLoop() {
    return loop_;
}

void Animation::SetLoop(bool loop) {
    loop_ = loop;
}

int Animation::GetFrameRate() {
    return frame_rate_;
}

void Animation::SetFrameRate(int frame_rate) {
    frame_rate_ = frame_rate;
}

size_t Animation::GetAnimationFrames() {
    return key_frames_.size();
}

const Skeleton& Animation::GetCurrentSkeleton() const {
    return current_frame_;
}

void Animation::Update() {
    float delta = engine::application->GetDeltaTime() / 1000.0f;
    anim_time_ += delta;
    float frame_duration = 1.0f / frame_rate_;
    int frames = key_frames_.size();
    float duration = frame_duration * frames;
    while (anim_time_ > duration) {
        anim_time_ -= duration;
    }
    while (anim_time_ < 0.0f) {
        anim_time_ += duration;
    }

    float frame = anim_time_ * frame_rate_;
    int low_frame = static_cast<int>(std::floor(frame));
    int high_frame = static_cast<int>(std::ceil(frame));

    float interpolate = std::fmod(anim_time_, frame_duration);

    const Skeleton& low = key_frames_[low_frame % frames];
    const Skeleton& high = key_frames_[high_frame % frames];

    current_frame_.Reset();
    int joint_size = low.JointSize();
    for (int i = 0; i < joint_size; ++i) {
        const Joint& joint1 = low.GetJoint(i);
        const Joint& joint2 = high.GetJoint(i);
        Joint joint;
        joint.translate =
            glm::lerp(joint1.translate, joint2.translate, interpolate);
        joint.rotate = glm::mix(joint1.rotate, joint2.rotate, interpolate);
        current_frame_.AddJoint(joint, i);
    }
}

void Animation::ReserveAnimationFrames(int frames) {
    key_frames_.reserve(frames);
    boundings_.reserve(frames);
}

void Animation::AddBoundingBox(const BoundingBox& bounding) {
    boundings_.push_back(bounding);
}

BoundingBox Animation::GetBoundingBox(int frame) {
    return boundings_[frame];
}
