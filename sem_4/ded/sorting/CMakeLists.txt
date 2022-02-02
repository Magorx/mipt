cmake_minimum_required(VERSION 3.15)
project(moga_engine LANGUAGES CXX)

set(CMAKE_CXX_COMPILER clang++)

set(CMAKE_CXX_STANDARD 20)
set(SFML_BUILD_GRAPHICS true)

set(WARNING_FLAGS "-Wall -Wextra -Wno-multichar -Wno-unused-result")

include_directories(src
                    src/engine
                    src/visual
                    src/physics
                    src/collections)

set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_CXX_FLAGS   "-O3 ${WARNING_FLAGS} -fdeclspec -march=native") #-fsanitize=address

find_package(SFML 2 COMPONENTS system window graphics network audio REQUIRED)
find_package(OpenGL)

add_executable(moga

        # utils
        src/utils/vec3d.cpp
        src/utils/timer.cpp

        src/utils/interpolator/catmullrom.cpp

        src/utils/logger.cpp

        src/utils/text_line/line.cpp
        src/utils/text_line/line_pos.cpp
        src/utils/text_line/cursor.cpp
        src/utils/text_line/line_history.cpp

        src/utils/state_history/history.cpp

        # event

        # interface
        src/view/view.cpp

        # engine
        src/engine/object.cpp
        src/engine/moga_engine.cpp

        # devices
        src/device/keyboard.cpp

        # visual
        src/visual/renderer/gate_type_aliases.cpp

        src/visual/color/color.cpp
        src/visual/color/rgba.cpp
        src/visual/color/smart_color.cpp
        src/visual/renderer/renderer.cpp
        src/visual/animation/animation.cpp

        src/visual/renderer/appearence/texture.cpp
        src/visual/renderer/appearence/animation.cpp

        # physics
        src/physics/physics_engine.cpp
        src/physics/solid_body/solid_body.cpp

        src/physics/collision/coll_circle_circle.cpp
        src/physics/collision/coll_circle_line.cpp
        
        src/physics/solid_body/sb_circle.cpp
        src/physics/solid_body/sb_line.cpp

        # collections
        src/collections/smart_colors/smart_colors.cpp
        src/collections/renderables/renderables.cpp
        src/collections/objects/objects.cpp

        # styles

        src/collections/styles/button.cpp
        src/collections/styles/util_bar.cpp
        src/collections/styles/window.cpp
        src/collections/styles/text.cpp

        # views

        src/collections/views/button.cpp
        src/collections/views/toggler.cpp
        src/collections/views/highlighter.cpp
        src/collections/views/mouse_catcher.cpp
        src/collections/views/vertical_layout.cpp
        src/collections/views/horizontal_layout.cpp
        src/collections/views/text.cpp
        src/collections/views/hideable.cpp
        src/collections/views/menu.cpp
        src/collections/views/submenu.cpp
        src/collections/views/labeled_view.cpp
        src/collections/views/repeater.cpp
        src/collections/views/magnetic.cpp
        src/collections/views/utility_tab.cpp
        src/collections/views/window.cpp
        src/collections/views/dialog_window.cpp

        src/collections/views/text_field.cpp
        src/collections/views/spline.cpp

        # redactor views

        src/collections/redactor_views/canvas.cpp
        src/collections/redactor_views/plot.cpp
        src/collections/redactor_views/color_picker.cpp


        src/collections/actions/toggle.cpp

        # effects

        src/collections/effects/rgb_splined.cpp
        src/collections/effects/layer_permanent.cpp

        # components
        src/visual/visual_engine.cpp

        src/engine/tickable.h

        # resources
        src/engine/Resources.cpp

        # redactor

        src/redactor/tools/brush.cpp
        src/redactor/tools/color_pipette.cpp
        src/redactor/tools/plugin_tool.cpp
        src/redactor/tools/tool_manager.cpp
        src/redactor/canvas/canvas.cpp
        src/redactor/canvas/canvas_history.cpp

        # plugins

        src/redactor/plugins/plugin.cpp
        src/redactor/plugins/plugin_manager.cpp
        
        src/redactor/plugins/interface/app_plugin_interface.cpp
        src/redactor/plugins/interface/target.cpp
        src/redactor/plugins/interface/plugin_shader.cpp
        src/redactor/plugins/interface/target_factory.cpp
        src/redactor/plugins/interface/widget_factory.cpp
        src/redactor/plugins/interface/shader_factory.cpp

        src/redactor/plugins/interface/widgets/pwidget.cpp
        src/redactor/plugins/interface/widgets/pwindow.cpp
        src/redactor/plugins/interface/widgets/pbutton.cpp
        src/redactor/plugins/interface/widgets/pslider.cpp
        
        src/redactor/plugins/interface/widgets/plabel.cpp
        src/redactor/plugins/interface/widgets/ptext_field.cpp
        src/redactor/plugins/interface/widgets/pcolor_picker.cpp

        # extension

        src/redactor/extensions/extension_manager.cpp
        src/redactor/plugins/interface/plugin_extension.cpp

        src/redactor/layer/layer.cpp

        src/redactor/engine.cpp

        # chemistry
        # src/chemistry/chem_engine.cpp
        # src/chemistry/chem_reactions.cpp

        src/main.cpp
        )

target_link_libraries(moga sfml-graphics sfml-window sfml-system)
target_link_libraries(moga ${OPENGL_LIBRARIES} ${CMAKE_DL_LIBS})

# aux_source_directory(src/collection/plugins ERASER_SOURCES)
# add_library(sharpy SHARED src/collections/plugins/sharpy.cpp)
# set_target_properties(sharpy PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/tool/kctf_sharpy")

# add_library(rainbow_stamp SHARED src/collections/plugins/rainbow_stamp.cpp)
# set_target_properties(rainbow_stamp PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/tool/kctf_rainbow_stamp")

# add_library(shdr SHARED src/collections/plugins/shdr.cpp)
# set_target_properties(shdr PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/tool/kctf_shdr")

# add_library(negative SHARED src/collections/plugins/negative.cpp)
# set_target_properties(negative PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/effect/kctf_negative")

# add_library(bloor SHARED src/collections/plugins/bloor.cpp)
# set_target_properties(bloor PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/effect/kctf_bloor")

# add_library(sharp SHARED src/collections/plugins/sharp.cpp)
# set_target_properties(sharp PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/effect/kctf_sharp")

# add_library(kctf_widget_library_extension SHARED src/collections/plugins/kctf_widget_library_extension.cpp)
# set_target_properties(kctf_widget_library_extension PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/extension/kctf_widget_library_extension")

# add_library(invasion SHARED src/collections/plugins/invasion.cpp)
# set_target_properties(invasion PROPERTIES PREFIX "" OUTPUT_NAME "resources/plugin/extension/invasion")


# add_custom_target(rebuild_plugins
#         COMMAND convert image.ppm image.png
#         )
