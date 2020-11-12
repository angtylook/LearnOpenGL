#include "helper.h"

#include <algorithm>
#include <ctime>

ElapsedTime::ElapsedTime(float maxTimeStep /* = 0.03333f */)
    : m_fMaxTimeStep(maxTimeStep)
    , m_fPrevious(std::clock() / (float)CLOCKS_PER_SEC) {}

float ElapsedTime::GetElapsedTime() const {
    float fCurrentTime = std::clock() / (float)CLOCKS_PER_SEC;
    float fDeltaTime = fCurrentTime - m_fPrevious;
    m_fPrevious = fCurrentTime;

    // Clamp to the max time step
    fDeltaTime = std::min(fDeltaTime, m_fMaxTimeStep);

    return fDeltaTime;
}

void ScopeGuard::Dismiss() {
    dismissed_ = true;
}

ScopeGuard::~ScopeGuard() {
    if (!dismissed_) {
        func_();
    }
}

ScopeGuard::ScopeGuard(std::function<void()> func)
    : func_(func), dismissed_(false) {}
