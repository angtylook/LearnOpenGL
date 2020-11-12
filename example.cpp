//
// Created by jxunhappy on 2017/7/1.
//

#include <vector>
#include <iostream>
#include "app.h"

int main(int argc, char* argv[]) {
    App app;
    engine::Application application(&app);
    return application.Run();
}
