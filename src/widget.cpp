
/*
    src/widget.cpp -- Base class of all widgets

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/widget.h>
#include <nanogui/layout.h>
#include <nanogui/theme.h>
#include <nanogui/window.h>
#include <nanogui/opengl.h>
#include <nanogui/screen.h>

NAMESPACE_BEGIN(nanogui)

std::atomic<int> Widget::idCounter {0};

Widget::Widget(ref<Widget> parent)
    : nid(idCounter.fetch_add(1)), mParent(), mTheme(nullptr), mLayout(nullptr),
      mPos(Vector2i(0)), mSize(Vector2i(0)),
      mFixedSize(Vector2i(0)), mVisible(true), mEnabled(true),
      mFocused(false), mMouseFocus(false), mTooltip(""), mFontSize(-1.0f),
      mCursor(Cursor::Arrow) {
    if (parent) {
		mParent = parent;
        mTheme = parent->mTheme;
    }
}

void Widget::associate() {
	ref<Widget> parent = mParent.lock();

	if (parent) {
		parent->addChild(shared_from_this());
	}
}

Widget::~Widget() {
}

int Widget::fontSize() const {
    return mFontSize < 0 ? mTheme->mStandardFontSize : mFontSize;
}

Vector2i Widget::preferredSize(NVGcontext *ctx){
    if (mLayout)
        return mLayout->preferredSize(ctx, shared_from_this());
    else
        return mSize;
}

void Widget::performLayout(NVGcontext *ctx) {
    if (mLayout) {
        mLayout->performLayout(ctx, shared_from_this());
    } else {
        for (auto c : mChildren) {
            Vector2i pref = c->preferredSize(ctx), fix = c->fixedSize();
            c->setSize(Vector2i(
                fix[0] ? fix[0] : pref[0],
                fix[1] ? fix[1] : pref[1]
            ));
            c->performLayout(ctx);
        }
    }
}

ref<Widget> Widget::findWidget(const Vector2i &p) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        ref<Widget> child = *it;
        if (child->visible() && child->contains(p - mPos))
            return child->findWidget(p - mPos);
    }
    return contains(p) ? shared_from_this() : nullptr;
}

bool Widget::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        ref<Widget> child = *it;
        if (child->visible() && child->contains(p - mPos) &&
            child->mouseButtonEvent(p - mPos, button, down, modifiers))
            return true;
    }
    if (button == GLFW_MOUSE_BUTTON_1 && down && !mFocused)
        requestFocus();
    return false;
}

bool Widget::mouseMotionEvent(const Vector2i &p, const Vector2i &rel, int button, int modifiers) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        ref<Widget> child = *it;
        if (!child->visible())
            continue;
        bool contained = child->contains(p - mPos), prevContained = child->contains(p - mPos - rel);
        if (contained != prevContained)
            child->mouseEnterEvent(p, contained);
        if ((contained || prevContained) &&
            child->mouseMotionEvent(p - mPos, rel, button, modifiers))
            return true;
    }
    return false;
}

bool Widget::scrollEvent(const Vector2i &p, const Vector2f &rel) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        ref<Widget> child = *it;
        if (!child->visible())
            continue;
        if (child->contains(p - mPos) && child->scrollEvent(p - mPos, rel))
            return true;
    }
    return false;
}

bool Widget::mouseDragEvent(const Vector2i &, const Vector2i &, int, int) {
    return false;
}

bool Widget::mouseEnterEvent(const Vector2i &, bool enter) {
    mMouseFocus = enter;
    return false;
}

bool Widget::focusEvent(bool focused) {
    mFocused = focused;
    return false;
}

bool Widget::keyboardEvent(int, int, int, int) {
    return false;
}

bool Widget::keyboardCharacterEvent(unsigned int) {
    return false;
}

void Widget::addChild(ref<Widget> widget) {
    mChildren.push_back(widget);
    widget->setParent(shared_from_this());
}

void Widget::removeChild(const ref<Widget> widget) {
    mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), widget), mChildren.end());
}

void Widget::removeChild(int index) {
    ref<Widget> widget = mChildren[index];
    mChildren.erase(mChildren.begin() + index);
}

ref<Window> Widget::window() {
    ref<Widget> widget = shared_from_this();
    while (true) {
        if (!widget)
            throw std::runtime_error(
                "Widget:internal error (could not find parent window)");
        ref<Window> window = dynamic_pointer_cast<Window>(widget);
        if (window)
            return window;
        widget = widget->parent();
    }
}

void Widget::requestFocus() {
    ref<Widget> widget = shared_from_this();
    while (widget->parent())
        widget = widget->parent();
    static_pointer_cast<Screen>(widget)->updateFocus(shared_from_this());
}

void Widget::drawBounds(NVGcontext *ctx, NVGcolor const& c) {
	nvgStrokeWidth(ctx, 1.0f);
	nvgBeginPath(ctx);
	nvgRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y);
	nvgStrokeColor(ctx, c);
	nvgStroke(ctx);
}


#define NANOGUI_SHOW_WIDGET_BOUNDS 0

void Widget::draw(NVGcontext *ctx) {
    #if NANOGUI_SHOW_WIDGET_BOUNDS
	if (mDebugBounds)
		drawBounds(ctx);
    #endif

    if (mChildren.empty())
        return;

    nvgTranslate(ctx, mPos.x, mPos.y);
    for (auto child : mChildren)
        if (child->visible())
            child->draw(ctx);
    nvgTranslate(ctx, -mPos.x, -mPos.y);
}

Vector2i Widget::absolutePosition() const {
	ref<Widget> parent = mParent.lock();
	if (!parent) {
		return mPos;
	}

	return mPos + parent->absolutePosition();
}

void Widget::translate(const Vector2i& rel) {
	mPos += rel;
}

NAMESPACE_END(nanogui)
