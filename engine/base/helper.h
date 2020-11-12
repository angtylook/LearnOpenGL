#ifndef COMMON_HELPER_H_
#define COMMON_HELPER_H_

#include <functional>

// Helper class to count frame time
class ElapsedTime {
public:
    ElapsedTime(float maxTimeStep = 0.03333f);
    float GetElapsedTime() const;

private:
    float m_fMaxTimeStep;
    mutable float m_fPrevious;
};

class ScopeGuard {
public:
    explicit ScopeGuard(std::function<void()> func);
    ~ScopeGuard();
    void Dismiss();

private:
    std::function<void()> func_;
    bool dismissed_;

private:  // noncopyable
    ScopeGuard(ScopeGuard const&) = delete;
    ScopeGuard& operator=(ScopeGuard const&) = delete;
};

#endif  // COMMON_HELPER_H_
