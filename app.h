//
// Created by jxunhappy on 2017/7/1.
//

#ifndef APP_H_
#define APP_H_

#include "application.h"

class App : public engine::Application::Delegate {
public:
    App();

    virtual ~App();

    virtual std::string GetResourceRoot() override;

    virtual engine::math::ivec2 GetWindowSize() override;

    virtual std::string GetWindowTitle() override;

    virtual bool Initialize(engine::Application* application) override;

    virtual void WillEnter() override;

    virtual void Update() override;

    virtual bool Done() override;

    virtual void WillWindowClose() override;

    virtual void OnExit() override;

private:
    bool done_;
    engine::Application* application_;
};

#endif  // APP_H_
