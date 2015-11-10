/*
    src/window.cpp -- Top-level window widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/window.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <nanogui/screen.h>
#include <nanogui/button.h>
#include <iostream>
#include <nanogui/entypo.h>
#include <nanogui/font_awesome.h>

NAMESPACE_BEGIN(nanogui)

Window::Window(ref<Widget> parent, const std::string &title, bool closeable, bool rollable)
    : Widget(parent), mTitle(title), mModal(false), mDrag(false), mCloseable(closeable), mRollable(rollable) {
}

void Window::associate() {
	Widget::associate();

	ref<Button> close = makeref<Button>(nullptr, "", FA_TIMES); // not makewidget, no auto associate! This isn't a real child.
	int bh = mTheme->mWindowHeaderHeight - 8;

	close->setSize(Vector2i(bh, bh));
	close->setParent(shared_from_this());
	//close->setIconTypeface("icons");
	close->setTheme(this->theme());
	close->setFontSize(bh);
	close->setCallback([this] {
		if (mCloseCallback) {
			if (mCloseCallback()) {
				this->dispose();
			}
		}
		else {
			this->dispose();
		}
	});
	closeButton = close;

	rollButton = makeref<Button>(nullptr, "", FA_MINUS);
	rollButton->setFlags(Button::Flags::ToggleButton);
	rollButton->setSize(Vector2i(bh, bh));
	rollButton->setParent(shared_from_this());
	//rollButton->setIconTypeface("icons");
	rollButton->setTheme(this->theme());
	rollButton->setFontSize(bh);
	rollButton->setChangeCallback([this](bool clicked) {
		if (mRollCallback) {
			if (mRollCallback(clicked)) {
				this->mRolled = clicked;
			}
		}
		else {
			this->mRolled = clicked;
		}
	});
}

void Window::performLayout(NVGcontext *ctx) {
	Widget::performLayout(ctx);

	Vector2i closeButtonPos = Vector2i(mSize.x() - closeButton->width() - 4, 4);
	closeButton->setPosition(closeButtonPos);

	closeButtonPos -= Vector2i(rollButton->width() + 2, 0);
	rollButton->setPosition(closeButtonPos);
}

Vector2i Window::preferredSize(NVGcontext *ctx) {
    Vector2i result = Widget::preferredSize(ctx);

    nvgFontSize(ctx, 18.0f);
    nvgFontFace(ctx, "sans-bold");
    float bounds[4];
    nvgTextBounds(ctx, 0, 0, mTitle.c_str(), nullptr, bounds);

    return result.cwiseMax(Vector2i(bounds[2]-bounds[0] + closeButton->width() + rollButton->width() + 6, bounds[3]-bounds[1]));
}

void Window::drawTitle(NVGcontext *ctx) {

	int ds = mTheme->mWindowDropShadowSize;
	int cr = mTheme->mWindowCornerRadius;
	int hh = mTheme->mWindowHeaderHeight;

	nvgSave(ctx);

	/* Draw a drop shadow */
	NVGpaint shadowPaint = nvgBoxGradient(
			ctx, mPos.x(), mPos.y(), mSize.x(), hh, cr * 2, ds * 2,
			mTheme->mDropShadow, mTheme->mTransparent);

	nvgBeginPath(ctx);
	nvgRect(ctx, mPos.x() - ds, mPos.y() - ds, mSize.x() + 2 * ds, hh + 2 * ds);
	nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(), hh, cr);
	nvgPathWinding(ctx, NVG_HOLE);
	nvgFillPaint(ctx, shadowPaint);
	nvgFill(ctx);


	if (!mTitle.empty()) {
		/* Draw header */
		NVGpaint headerPaint = nvgLinearGradient(
				ctx, mPos.x(), mPos.y(), mPos.x(),
				mPos.y() + hh,
				mTheme->mWindowHeaderGradientTop,
				mTheme->mWindowHeaderGradientBot);

		nvgBeginPath(ctx);
		nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(), hh, cr);

		nvgFillPaint(ctx, headerPaint);
		nvgFill(ctx);

		nvgBeginPath(ctx);
		nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(), hh, cr);
		nvgStrokeColor(ctx, mTheme->mWindowHeaderSepTop);
		nvgScissor(ctx, mPos.x(), mPos.y(), mSize.x(), 0.5f);
		nvgStroke(ctx);
		nvgResetScissor(ctx);

		nvgBeginPath(ctx);
		nvgMoveTo(ctx, mPos.x() + 0.5f, mPos.y() + hh - 1.5f);
		nvgLineTo(ctx, mPos.x() + mSize.x() - 0.5f, mPos.y() + hh - 1.5);
		nvgStrokeColor(ctx, mTheme->mWindowHeaderSepBot);
		nvgStroke(ctx);

		nvgFontSize(ctx, 18.0f);
		nvgFontFace(ctx, "sans-bold");
		nvgTextAlign(ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);

		nvgFontBlur(ctx, 2);
		nvgFillColor(ctx, mTheme->mDropShadow);
		nvgText(ctx, mPos.x() + mSize.x() / 2,
				mPos.y() + hh / 2, mTitle.c_str(), nullptr);

		nvgFontBlur(ctx, 0);
		nvgFillColor(ctx, mFocused ? mTheme->mWindowTitleFocused
								   : mTheme->mWindowTitleUnfocused);
		nvgText(ctx, mPos.x() + mSize.x() / 2, mPos.y() + hh / 2 - 1,
				mTitle.c_str(), nullptr);
	}

	nvgTranslate(ctx, mPos.x(), mPos.y());
	if (mCloseable && closeButton) {
		closeButton->draw(ctx);
	}

	if (mRollable && rollButton) {
		rollButton->draw(ctx);
	}
	nvgRestore(ctx);
}

void Window::draw(NVGcontext *ctx) {
	if (!mVisible) {
		return;
	}

	int ds = mTheme->mWindowDropShadowSize, cr = mTheme->mWindowCornerRadius;

	if (mRollable && mRolled) {
		drawTitle(ctx);
		return;
	}

    /* Draw window */
    nvgSave(ctx);
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y(), cr);

    nvgFillColor(ctx, mMouseFocus ? mTheme->mWindowFillFocused
                                  : mTheme->mWindowFillUnfocused);
    nvgFill(ctx);

    /* Draw a drop shadow */
    NVGpaint shadowPaint = nvgBoxGradient(
        ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y(), cr*2, ds*2,
        mTheme->mDropShadow, mTheme->mTransparent);

    nvgBeginPath(ctx);
    nvgRect(ctx, mPos.x()-ds,mPos.y()-ds, mSize.x()+2*ds, mSize.y()+2*ds);
    nvgRoundedRect(ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y(), cr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, shadowPaint);
    nvgFill(ctx);

	drawTitle(ctx);

	Widget::draw(ctx);
}

void Window::addChildWindow(ref<Window> win) {
	if (std::find(mChildWindows.begin(), mChildWindows.end(), win) != mChildWindows.end()) {
		return; //already a child
	}

	mChildWindows.push_back(win);
}

void Window::removeChildWindow(ref<Window> win) {
	for (auto it = mChildWindows.begin(); it != mChildWindows.end(); ++it){
		if (*it == win) {
			mChildWindows.erase(it);
			break;
		}
	}
}

void Window::dispose() {

	std::vector<ref<Window>> childWindows(mChildWindows.begin(), mChildWindows.end());

	for (auto w : childWindows) {
		w->dispose();
	}

	mChildWindows.clear();

    ref<Widget> widget = shared_from_this();
    while (widget->parent())
        widget = widget->parent();
    static_pointer_cast<Screen>(widget)->disposeWindow(static_pointer_cast<Window>(shared_from_this()));
}

void Window::center() {
    ref<Widget> widget = shared_from_this();
    while (widget->parent())
        widget = widget->parent();
	static_pointer_cast<Screen>(widget)->centerWindow(static_pointer_cast<Window>(shared_from_this()));
}

bool Window::mouseDragEvent(const Vector2i &, const Vector2i &rel,
                            int button, int /* modifiers */) {
    if (mDrag && (button & (1 << GLFW_MOUSE_BUTTON_1)) != 0) {
        mPos += rel;
        mPos = mPos.cwiseMax(Vector2i::Zero());
        mPos = mPos.cwiseMin(parent()->size() - mSize);
        return true;
    }
    return false;
}

bool Window::mouseButtonEvent(const Vector2i &p, int button, bool down, int modifiers) {
	if (mCloseable && closeButton->contains(p - mPos) && closeButton->mouseButtonEvent(p - mPos, button, down, modifiers)){
		return true;
	}

	if (mRollable && rollButton->contains(p - mPos) && rollButton->mouseButtonEvent(p - mPos, button, down, modifiers)) {
		return true;
	}

    if (Widget::mouseButtonEvent(p, button, down, modifiers))
        return true;

    if (button == GLFW_MOUSE_BUTTON_1) {
        mDrag = down && (p.y() - mPos.y()) < mTheme->mWindowHeaderHeight;
        return true;
    }
    return false;
}

bool Window::scrollEvent(const Vector2i &p, const Vector2f &rel) {
    Widget::scrollEvent(p, rel);
    return true;
}

void Window::refreshRelativePlacement() {
    /* Overridden in \ref Popup */
}

NAMESPACE_END(nanogui)
