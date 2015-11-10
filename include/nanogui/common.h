/*
    nanogui/common.h -- common definitions used by NanoGUI

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#define GLM_SWIZZLE 1
#define GLM_FORCE_RADIANS 1

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdint.h>
#include <array>
#include <vector>
#include <memory>
#include <algorithm>

/* Set to 1 to draw boxes around widgets */
//#define NANOGUI_SHOW_WIDGET_BOUNDS 1

#if !defined(NAMESPACE_BEGIN)
#define NAMESPACE_BEGIN(name) namespace name {
#endif
#if !defined(NAMESPACE_END)
#define NAMESPACE_END(name) }
#endif

#if defined(NANOGUI_SHARED)
#  if defined(_WIN32)
#    if defined(NANOGUI_BUILD)
#      define NANOGUI_EXPORT __declspec(dllexport)
#    else
#      define NANOGUI_EXPORT __declspec(dllimport)
#    endif
#  elif defined(NANOGUI_BUILD)
#    define NANOGUI_EXPORT __attribute__ ((visibility("default")))
#  else
#    define NANOGUI_EXPORT
#  endif
#else
#    define NANOGUI_EXPORT
#endif

#if defined(_WIN32)
#if defined(NANOGUI_BUILD)
/* Quench a few warnings on when compiling NanoGUI on Windows */
#pragma warning(disable : 4127) // warning C4127: conditional expression is constant
#pragma warning(disable : 4244) // warning C4244: conversion from X to Y, possible loss of data
#endif
#pragma warning(disable : 4251) // warning C4251: class X needs to have dll-interface to be used by clients of class Y
#pragma warning(disable : 4714) // warning C4714: funtion X marked as __forceinline not inlined
#endif

struct NVGcontext;
struct NVGcolor;
struct NVGglyphPosition;
struct GLFWwindow;
struct GLFWcursor;

// Define command key for windows/mac/linux
#ifdef __APPLE__
#define SYSTEM_COMMAND_MOD GLFW_MOD_SUPER
#else
#define SYSTEM_COMMAND_MOD GLFW_MOD_CONTROL
#endif

NAMESPACE_BEGIN(nanogui)

/* Cursor shapes */
enum class Cursor {
    Arrow = 0,
    IBeam,
    Crosshair,
    Hand,
    HResize,
    VResize,
    CursorCount
};

/* Import some common Eigen types */

using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Vector4f = glm::vec4;

using Vector2i = glm::ivec2;
using Vector3i = glm::ivec3;
using Vector4i = glm::ivec4;
using Matrix3f = glm::mat3;
using Matrix4f = glm::mat4;

/// Stores an RGBA color value
class Color : public glm::vec4 {
    typedef glm::vec4 Base;
public:
	using glm::vec4::vec4;

    Color() : Vector4f(0, 0, 0, 0) {}

    Color(const glm::ivec3 &color, int alpha) : glm::vec4(glm::vec3(color) / 255.0f, alpha / 255.0f) { }

    Color(const glm::vec3 &color) : glm::vec4(color, 1.0f) {}

    Color(const glm::ivec3 &color) : glm::vec4(glm::vec3(color) / 255.0f, 1.0f) { }

    Color(const glm::ivec4 &color) : glm::vec4(glm::vec4(color) / 255.f) { }

    Color(float intensity, float alpha) : glm::vec4(glm::vec3(intensity), alpha) { }

    Color(int intensity, int alpha) : glm::vec4(glm::vec3(intensity / 255.f), alpha / 255.0f) { }

    Color(int r, int g, int b, int a) : glm::vec4(glm::vec4(r, g, b, a) / 255.0f) { }

    Color contrastingColor() const {
		Color seed = (*this) * Color(0.299f, 0.587f, 0.144f, 0.f);
        float luminance = std::accumulate(&seed[0], &seed[0] + 4, 0.0f);
        return Color(luminance < 0.5f ? 1.f : 0.f, 1.f);
    }

    inline operator const NVGcolor &() const;
};


/* Forward declarations */
template <typename T>
using ref = std::shared_ptr<T>;

template <typename T>
using weakref = std::weak_ptr<T>;

class AdvancedGridLayout;
class BoxLayout;
class Button;
class CheckBox;
class ColorWheel;
class ColorPicker;
class ComboBox;
class GLFramebuffer;
class GLShader;
class GridLayout;
class GroupLayout;
class ImagePanel;
class Label;
class Layout;
class MessageDialog;
class Object;
class Popup;
class PopupButton;
class ProgressBar;
class Screen;
class Slider;
class TextBox;
class Theme;
class ToolButton;
class VScrollPanel;
class Widget;
class Window;

template<typename T, typename... Args>
std::shared_ptr<T> makeref(Args&&... args)
{
	return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
}

template<typename T, typename... Args>
ref<T> makewidget(Args&&... args)
{
	ref<T> ret = std::shared_ptr<T>(new T(std::forward<Args>(args)...));

	ret->associate();

	return ret;
}

using std::dynamic_pointer_cast;
using std::static_pointer_cast;


/// Static initialization; should be called once before invoking any NanoGUI functions
extern NANOGUI_EXPORT void init();

/// Static shutdown; should be called before the application terminates
extern NANOGUI_EXPORT void shutdown();

/// Enter the application main loop
extern NANOGUI_EXPORT void mainloop();

/// Request the application main loop to terminate
extern NANOGUI_EXPORT void leave();

#if defined(__APPLE__)
/**
 * \brief Move to the application bundle's parent directory
 *
 * This is function is convenient when deploying .app bundles on OSX. It
 * adjusts the file path to the parent directory containing the bundle.
 */
extern NANOGUI_EXPORT void chdir_to_bundle_parent();
#endif

/**
 * \brief Convert a single UTF32 character code to UTF8
 *
 * NanoGUI uses this to convert the icon character codes
 * defined in entypo.h
 */
extern NANOGUI_EXPORT std::array<char, 8> utf8(int c);

/// Load a directory of PNG images and upload them to the GPU (suitable for use with ImagePanel)
extern NANOGUI_EXPORT std::vector<std::pair<int, std::string>>
    loadImageDirectory(NVGcontext *ctx, const std::string &path);

/// Convenience function for instanting a PNG icon from the application's data segment (via bin2c)
#define nvgImageIcon(ctx, name) nanogui::__nanogui_get_image(ctx, #name, name##_png, name##_png_size)
/// Helper function used by nvgImageIcon
extern NANOGUI_EXPORT int __nanogui_get_image(NVGcontext *ctx, const std::string &name, uint8_t *data, uint32_t size);

NAMESPACE_END(nanogui)
