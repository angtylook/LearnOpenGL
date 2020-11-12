set(base
        ${CMAKE_CURRENT_SOURCE_DIR}/base/event.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/event.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/filesystem.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/filesystem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/helper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/helper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/log.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/micros.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/string_util.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/string_util.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/window.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/window.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/base/xml.h
        ${CMAKE_CURRENT_SOURCE_DIR}/base/xml.cpp
        )

set(impl
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/input_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/input_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/light_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/light_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/material_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/material_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/model_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/model_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/scene_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/scene_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/shader_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/shader_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/texture_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/texture_manager_impl.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/ui_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/impl/ui_manager_impl.cpp
        )

set(lua
        ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua_manager_impl.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lua/lua_manager_impl.cpp
        )

set(math
        ${CMAKE_CURRENT_SOURCE_DIR}/math/bounding_box.h
        ${CMAKE_CURRENT_SOURCE_DIR}/math/bounding_box.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/math/math_util.h
        ${CMAKE_CURRENT_SOURCE_DIR}/math/math_util.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/math/rect.h
        ${CMAKE_CURRENT_SOURCE_DIR}/math/vec.h
        ${CMAKE_CURRENT_SOURCE_DIR}/math/vec.cpp
        )

set(model
        ${CMAKE_CURRENT_SOURCE_DIR}/model/animation.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/animation.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/bone.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/bone.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/curve.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/curve.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/light.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/light.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/material.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/material.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/mesh.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/mesh.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/model.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/model.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/skeleton.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/skeleton.cpp
        )

set(model_loader
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/md5_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/md5_loader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/model_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/model_loader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/obj_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/obj_loader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/skeleton_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/skeleton_loader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/sweep_loader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/model/loader/sweep_loader.cpp
        )

set(render
        ${CMAKE_CURRENT_SOURCE_DIR}/render/base_render.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/base_render.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/curve_render.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/curve_render.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/frame_buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/frame_buffer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/gl_debug.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/gl_debug.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/gl_shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/gl_shader.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/gl_state.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/gl_state.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/light_setuper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/light_setuper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/material_setuper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/material_setuper.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/mesh_render.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/mesh_render.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/post_buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/post_buffer.h
        ${CMAKE_CURRENT_SOURCE_DIR}/render/skybox.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/render/skybox.h
        )

set(scene
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/axis_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/axis_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/camera_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/camera_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/image_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/image_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/light_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/light_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/model_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/model_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/scene_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/text_node.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/text_node.h
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/world.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/scene/world.h
        )

set(ui
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/button.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/button.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/desktop.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/desktop.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/flags.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/flags.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/label.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/label.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/widget.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/widget.h
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/widget_builder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/ui/widget_builder.h
        )

set(root
        application.cpp
        application.h
        camera.cpp
        camera.h
        engine.cpp
        engine.h
        graphics.cpp
        graphics.h
        input_manager.h
        light_manager.h
        lua_manager.h
        material_manager.h
        model_manager.h
        render2d.cpp
        render2d.h
        scene_manager.h
        shader_manager.h
        texture_manager.h
        ui_manager.h
        )

source_group("engine\\base" FILES ${base})
source_group("engine\\impl" FILES ${impl})
source_group("engine\\lua" FILES ${lua})
source_group("engine\\math" FILES ${math})
source_group("engine\\model" FILES ${model})
source_group("engine\\model\\loader" FILES ${model_loader})
source_group("engine\\render" FILES ${render})
source_group("engine\\scene" FILES ${scene})
source_group("engine\\ui" FILES ${ui})
source_group("engine" FILES ${root})
