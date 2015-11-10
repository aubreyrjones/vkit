/*
    nanogui/window.h -- Top-level window widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include <nanogui/widget.h>

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT Window : public Widget {
    friend class Popup;
public:
    Window(ref<Widget> parent, const std::string &title = "Untitled", bool closeable = true, bool rollable = true);

    /// Return the window title
    const std::string &title() const { return mTitle; }
    /// Set the window title
    void setTitle(const std::string &title) { mTitle = title; }

    /// Is this a model dialog?
    bool modal() const { return mModal; }
    /// Set whether or not this is a modal dialog
    void setModal(bool modal) { mModal = modal; }

    /// Dispose the window
    virtual void dispose();

    /// Center the window in the current \ref Screen
    virtual void center();

    /// Draw the window
    virtual void draw(NVGcontext *ctx) override;

	std::function<bool()> closeCallback() const { return mCloseCallback; }
	void setCloseCallback(std::function<bool()> callback) { mCloseCallback = callback; }

	std::function<bool(bool)> rollCallback() const { return mRollCallback; }
	void setRollCallback(std::function<bool(bool)> callback) { mRollCallback = callback; }

    /// Handle window drag events
    virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers);
    /// Handle mouse events recursively and bring the current window to the top
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);
    /// Accept scroll events and propagate them to the widget under the mouse cursor
    virtual bool scrollEvent(const Vector2i &p, const Vector2f &rel);
    /// Compute the preferred size of the widget
    virtual Vector2i preferredSize(NVGcontext *ctx);

	virtual void performLayout(NVGcontext *ctx);

	virtual void addChildWindow(ref<Window> win);
	virtual void removeChildWindow(ref<Window> win);

	virtual void associate() override;
protected:
    /// Internal helper function to maintain nested window position values; overridden in \ref Popup
    virtual void refreshRelativePlacement();

	void drawTitle(NVGcontext *ctx);
protected:
    std::string mTitle;
    bool mModal;
    bool mDrag;
	bool mCloseable;
	bool mRollable;
	bool mRolled = false;
	ref<Button> closeButton = nullptr;
	ref<Button> rollButton = nullptr;
	std::function<bool()> mCloseCallback;
	std::function<bool(bool)> mRollCallback;
	std::vector<ref<Window>> mChildWindows;
};

NAMESPACE_END(nanogui)
