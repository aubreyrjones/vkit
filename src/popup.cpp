/*
    src/popup.cpp -- Simple popup widget which is attached to another given
    window (can be nested)

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/popup.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)
    
Popup::Popup(ref<Widget> parent, ref<Window> parentWindow)
    : Window(parent, ""), mParentWindow(parentWindow),
      mAnchorPos(Vector2i(0)), mAnchorHeight(30) {
}

void Popup::associate() {
	Window::associate();

	ref<Window> parent = mParentWindow.lock();
	if (parent) {
		parent->addChildWindow(static_pointer_cast<Window>(shared_from_this()));
	}
}

void Popup::performLayout(NVGcontext *ctx) {
    if (mLayout || mChildren.size() != 1) {
        Widget::performLayout(ctx);
    } else {
        mChildren[0]->setPosition(Vector2i(0));
        mChildren[0]->setSize(mSize);
        mChildren[0]->performLayout(ctx);
    }
}

void Popup::dispose() {

	ref<Window> pWin = mParentWindow.lock();
	if (pWin) {
		pWin->removeChildWindow(static_pointer_cast<Window>(shared_from_this()));
	}

	ref<Widget> parent = mParent.lock();
	if (parent) {
		parent->removeChild(shared_from_this());
	}

	Window::dispose();
}

void Popup::refreshRelativePlacement() {
	ref<Window> parent = mParentWindow.lock();

    parent->refreshRelativePlacement();
    mVisible &= parent->visibleRecursive();
    mPos = parent->position() + mAnchorPos - Vector2i(0, mAnchorHeight);
}

void Popup::draw(NVGcontext* ctx) {
    refreshRelativePlacement();

    if (!mVisible)
        return;

    int ds = mTheme->mWindowDropShadowSize, cr = mTheme->mWindowCornerRadius;

    /* Draw a drop shadow */
    NVGpaint shadowPaint = nvgBoxGradient(
        ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr*2, ds*2,
        mTheme->mDropShadow, mTheme->mTransparent);

    nvgBeginPath(ctx);
    nvgRect(ctx, mPos.x-ds,mPos.y-ds, mSize.x+2*ds, mSize.y+2*ds);
    nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);
    nvgPathWinding(ctx, NVG_HOLE);
    nvgFillPaint(ctx, shadowPaint);
    nvgFill(ctx);

    /* Draw window */
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);

    nvgMoveTo(ctx, mPos.x-15,mPos.y+mAnchorHeight);
    nvgLineTo(ctx, mPos.x+1,mPos.y+mAnchorHeight-15);
    nvgLineTo(ctx, mPos.x+1,mPos.y+mAnchorHeight+15);

    nvgFillColor(ctx, mTheme->mWindowPopup);
    nvgFill(ctx);

    Widget::draw(ctx);
}

NAMESPACE_END(nanogui)
