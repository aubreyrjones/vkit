/*
    nanogui/widget.h -- Base class of all widgets

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include <nanogui/object.h>
#include <vector>
#include <nanovg.h>
#include <atomic>

NAMESPACE_BEGIN(nanogui)

enum class Cursor;

/**
 * \brief Base class of all widgets
 *
 * \ref Widget is the base class of all widgets in \c nanogui. It can
 * also be used as an panel to arrange an arbitrary number of child
 * widgets using a layout generator (see \ref Layout).
 */
class NANOGUI_EXPORT Widget : public std::enable_shared_from_this<Widget> {
public:
    /// Construct a new widget with the given parent widget
    Widget(ref<Widget> parent);

	/// Associate this widget with its parent, which happens after construction is complete.
	virtual void associate();

    /// Return the parent widget
    ref<Widget> parent() { return mParent.lock(); }
    /// Return the parent widget
    const ref<Widget> parent() const { return mParent.lock(); }
    /// Set the parent widget
    void setParent(ref<Widget> parent) { mParent = parent; }

    /// Return the used \ref Layout generator
    ref<Layout> layout() { return mLayout; }
    /// Return the used \ref Layout generator
    const ref<Layout> layout() const { return mLayout; }
    /// Set the used \ref Layout generator
    void setLayout(ref<Layout> layout) { mLayout = layout; }

    /// Return the \ref Theme used to draw this widget
    ref<Theme> theme() { return mTheme; }
    /// Return the \ref Theme used to draw this widget
    const ref<Theme> theme() const { return mTheme; }
    /// Set the \ref Theme used to draw this widget
    void setTheme(ref<Theme> theme) { mTheme = theme; }

    /// Return the position relative to the parent widget
    const Vector2i &position() const { return mPos; }
    /// Set the position relative to the parent widget
    void setPosition(const Vector2i &pos) { mPos = pos; }

	/// Translate this widget.
	void translate(const Vector2i &rel);

    /// Return the absolute position on screen
    Vector2i absolutePosition() const;

    /// Return the size of the widget
    const Vector2i &size() const { return mSize; }
    /// set the size of the widget
    void setSize(const Vector2i &size) { mSize = size; }

    /// Return the width of the widget
    int width() const { return mSize.x; }
    /// Set the width of the widget
    void setWidth(int width) { mSize.x = width; }

    /// Return the height of the widget
    int height() const { return mSize.y; }
    /// Set the height of the widget
    void setHeight(int height) { mSize.y = height; }

    /**
     * \brief Set the fixed size of this widget
     *
     * If nonzero, components of the fixed size attribute override any values
     * computed by a layout generator associated with this widget. Note that
     * just setting the fixed size alone is not enough to actually change its
     * size; this is done with a call to \ref setSize or a call to \ref performLayout()
     * in the parent widget.
     */
    void setFixedSize(const Vector2i &fixedSize) { mFixedSize = fixedSize; }

    /// Return the fixed size (see \ref setFixedSize())
    const Vector2i &fixedSize() const { return mFixedSize; }

    // Return the fixed width (see \ref setFixedSize())
    int fixedWidth() const { return mFixedSize.x; }
    // Return the fixed height (see \ref setFixedSize())
    int fixedHeight() const { return mFixedSize.y; }
    /// Set the fixed width (see \ref setFixedSize())
    void setFixedWidth(int width) { mFixedSize.x = width; }
    /// Set the fixed height (see \ref setFixedSize())
    void setFixedHeight(int height) { mFixedSize.y = height; }

    /// Return whether or not the widget is currently visible (assuming all parents are visible)
    virtual bool visible() const { return mVisible; }
    /// Set whether or not the widget is currently visible (assuming all parents are visible)
    virtual void setVisible(bool visible) { mVisible = visible; }

    /// Check if this widget is currently visible, taking parent widgets into account
    bool visibleRecursive() {
        bool visible = true;
        ref<Widget> widget = shared_from_this();
        while (widget) {
            visible &= widget->visible();
            widget = widget->parent();
        }
        return visible;
    }

    /// Return the number of child widgets
    int childCount() const { return (int) mChildren.size(); }

    /// Return the list of child widgets of the current widget
    const std::vector<ref<Widget> > &children() const { return mChildren; }

    /**
     * \brief Add a child widget to the current widget
     *
     * This function almost never needs to be called by hand,
     * since the constructor of \ref Widget automatically
     * adds the current widget to its parent
     */
    void addChild(ref<Widget> widget);

    /// Remove a child widget by index
    void removeChild(int index);

    /// Remove a child widget by value
    void removeChild(const ref<Widget> widget);

    // Walk up the hierarchy and return the parent window
    ref<Window> window();

    /// Associate this widget with an ID value (optional)
    void setId(const std::string &id) { mId = id; }
    /// Return the ID value associated with this widget, if any
    const std::string &id() const { return mId; }

    /// Return whether or not this widget is currently enabled
    bool enabled() const { return mEnabled; }
    /// Set whether or not this widget is currently enabled
    void setEnabled(bool enabled) { mEnabled = enabled; }

    /// Return whether or not this widget is currently focused
    bool focused() const { return mFocused; }
    /// Set whether or not this widget is currently focused
    void setFocused(bool focused) { mFocused = focused; }
    /// Request the focus to be moved to this widget
    void requestFocus();

    const std::string &tooltip() const { return mTooltip; }
    void setTooltip(const std::string &tooltip) { mTooltip = tooltip; }

    /// Return current font size. If not set the default of the current theme will be returned
    int fontSize() const;
    /// Set the font size of this widget
    void setFontSize(int fontSize) { mFontSize = fontSize; }
    /// Return whether the font size is explicitly specified for this widget
    bool hasFontSize() const { return mFontSize > 0; }

    /// Return a pointer to the cursor of the widget
    Cursor cursor() const { return mCursor; }
    /// Set the cursor of the widget
    void setCursor(Cursor cursor) { mCursor = cursor; }

    /// Check if the widget contains a certain position
    bool contains(const Vector2i &p) const {
		auto d = p - mPos;

		return glm::all(glm::greaterThan(d, glm::ivec2(0))) &&
				glm::all(glm::lessThan(d, mSize));
    }

    /// Determine the widget located at the given position value (recursive)
    ref<Widget> findWidget(const Vector2i &p);

    /// Handle a mouse button event (default implementation: propagate to children)
    virtual bool mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers);

    /// Handle a mouse motion event (default implementation: propagate to children)
    virtual bool mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers);

    /// Handle a mouse drag event (default implementation: do nothing)
    virtual bool mouseDragEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers);

    /// Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
    virtual bool mouseEnterEvent(const Vector2i &p, bool enter);

    /// Handle a mouse scroll event (default implementation: propagate to children)
    virtual bool scrollEvent(const Vector2i &p, const Vector2f &rel);

    /// Handle a focus change event (default implementation: record the focus status, but do nothing)
    virtual bool focusEvent(bool focused);

    /// Handle a keyboard event (default implementation: do nothing)
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

    /// Handle text input (UTF-32 format) (default implementation: do nothing)
    virtual bool keyboardCharacterEvent(unsigned int codepoint);

    /// Compute the preferred size of the widget
    virtual Vector2i preferredSize(NVGcontext *ctx);

    /// Invoke the associated layout generator to properly place child widgets, if any
    virtual void performLayout(NVGcontext *ctx);

    /// Draw the widget (and all child widgets)
    virtual void draw(NVGcontext *ctx);
	virtual void drawBounds(NVGcontext *ctx, NVGcolor const& c = nvgRGBA(255, 0, 0, 128));

	bool mDebugBounds = false;

	const int nid;

	/// Free all resources used by the widget and any children
	virtual ~Widget();

protected:

protected:
    weakref<Widget> mParent;
    ref<Theme> mTheme;
    ref<Layout> mLayout;
    std::string mId;
    Vector2i mPos, mSize, mFixedSize;
    std::vector<ref<Widget>> mChildren;
    bool mVisible, mEnabled;
    bool mFocused, mMouseFocus;
    std::string mTooltip;
    int mFontSize;
    Cursor mCursor;

	static std::atomic<int> idCounter;
};

NAMESPACE_END(nanogui)
