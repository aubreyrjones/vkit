cmake_minimum_required(VERSION 2.8.12)
project(vkit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include ext/glm ext/nanovg/src ext/glfw/include ${CMAKE_BINARY_DIR})

# disable all the tests, examples, and extraneous options

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "build glfw examples")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "build glfw docs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "build glfw tests")
add_subdirectory(ext/glfw)

set(NANOVG_SOURCE_FILES ext/nanovg/src/nanovg.c)

set(SOURCE_FILES
    include/nanogui/button.h
    include/nanogui/checkbox.h
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
    src/window.cpp

    "${CMAKE_BINARY_DIR}/resources.cpp")

set(RESOURCE_FILES resources/entypo.ttf resources/fontawesome.ttf resources/Roboto-Bold.ttf resources/Roboto-Regular.ttf)

add_custom_command(OUTPUT "resources.h" "resources.cpp"
        COMMAND "python" ARGS ${CMAKE_SOURCE_DIR}/resc "--ns" "_r" "--outdir" ${CMAKE_BINARY_DIR} "resources" ${RESOURCE_FILES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS ${CMAKE_SOURCE_DIR}/resc ${RESOURCE_FILES}
        VERBATIM)



add_library(vkit STATIC ext/glad.c ${SOURCE_FILES} ${NANOVG_SOURCE_FILES})
target_link_libraries(glfw)