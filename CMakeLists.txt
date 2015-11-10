cmake_minimum_required(VERSION 3.3)
project(vkit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE_FILES
    include/nanogui/button.h
    include/nanogui/checkbox.h
    include/nanogui/colorpicker.h
    include/nanogui/colorwheel.h
    include/nanogui/combobox.h
    include/nanogui/common.h
    include/nanogui/divider.h
    include/nanogui/entypo.h
    include/nanogui/font_awesome.h
    include/nanogui/formhelper.h
    include/nanogui/glutil.h
    include/nanogui/graph.h
    include/nanogui/imagepanel.h
    include/nanogui/imageview.h
    include/nanogui/label.h
    include/nanogui/layout.h
    include/nanogui/messagedialog.h
    include/nanogui/nanogui.h
    include/nanogui/object.h
    include/nanogui/opengl.h
    include/nanogui/popup.h
    include/nanogui/popupbutton.h
    include/nanogui/progressbar.h
    include/nanogui/screen.h
    include/nanogui/slider.h
    include/nanogui/textbox.h
    include/nanogui/theme.h
    include/nanogui/toolbutton.h
    include/nanogui/vscrollpanel.h
    include/nanogui/widget.h
    include/nanogui/window.h
    src/button.cpp
    src/checkbox.cpp
    src/colorpicker.cpp
    src/colorwheel.cpp
    src/combobox.cpp
    src/common.cpp
    src/divider.cpp
    src/glutil.cpp
    src/graph.cpp
    src/imagepanel.cpp
    src/imageview.cpp
    src/label.cpp
    src/layout.cpp
    src/messagedialog.cpp
    src/popup.cpp
    src/popupbutton.cpp
    src/progressbar.cpp
    src/screen.cpp
    src/slider.cpp
    src/textbox.cpp
    src/theme.cpp
    src/vscrollpanel.cpp
    src/widget.cpp
    src/window.cpp)

set(RESOURCE_FILES resources/entypo.ttf resources/fontawesome.ttf resources/Roboto-Bold.ttf resources/Roboto-Regular.ttf)

add_custom_command(OUTPUT "assets.h" "assets.cpp"
        COMMAND "python" ARGS ${CMAKE_SOURCE_DIR}/resc "--ns" "_r" "--outdir" ${CMAKE_BINARY_DIR} "assets" ${RESOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/resc ${RESOURCE_FILES}
        VERBATIM)


add_library(vkit STATIC "${CMAKE_BINARY_DIR}/assets.cpp" ${SOURCE_FILES} )