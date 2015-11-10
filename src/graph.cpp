/*
    src/graph.cpp -- Simple graph widget for showing a function plot

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/graph.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

NAMESPACE_BEGIN(nanogui)

Graph::Graph(ref<Widget> parent, const std::string &caption)
    : Widget(parent), mCaption(caption) {
    mBackgroundColor = Color(20, 128);
    mForegroundColor = Color(255, 192, 0, 128);
    mTextColor = Color(240, 192);
}

Vector2i Graph::preferredSize(NVGcontext *) const {
    return Vector2i(180, 45);
}

void Graph::draw(NVGcontext *ctx) {
    Widget::draw(ctx);

    nvgBeginPath(ctx);
    nvgRect(ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y());
    nvgFillColor(ctx, mBackgroundColor);
    nvgFill(ctx);

    if (mValues.size() < 2)
        return;

    nvgBeginPath(ctx);
    nvgMoveTo(ctx, mPos.x(), mPos.y()+mSize.y());
    for (size_t i = 0; i < (size_t) mValues.size(); i++) {
        float value = mValues[i];
        float vx = mPos.x() + i * mSize.x() / (float) (mValues.size() - 1);
        float vy = mPos.y() + (1-value) * mSize.y();
        nvgLineTo(ctx, vx, vy);
    }

    nvgLineTo(ctx, mPos.x() + mSize.x(), mPos.y() + mSize.y());
    nvgStrokeColor(ctx, Color(100, 255));
    nvgStroke(ctx);
    nvgFillColor(ctx, mForegroundColor);
    nvgFill(ctx);

    nvgFontFace(ctx, "sans");

    if (!mCaption.empty()) {
        nvgFontSize(ctx, 14.0f);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
        nvgFillColor(ctx, mTextColor);
        nvgText(ctx, mPos.x() + 3, mPos.y() + 1, mCaption.c_str(), NULL);
    }

    if (!mHeader.empty()) {
        nvgFontSize(ctx, 18.0f);
        nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_TOP);
        nvgFillColor(ctx, mTextColor);
        nvgText(ctx, mPos.x() + mSize.x() - 3, mPos.y() + 1, mHeader.c_str(), NULL);
    }

    if (!mFooter.empty()) {
        nvgFontSize(ctx, 15.0f);
        nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_BOTTOM);
        nvgFillColor(ctx, mTextColor);
        nvgText(ctx, mPos.x() + mSize.x() - 3, mPos.y() + mSize.y() - 1, mFooter.c_str(), NULL);
    }

    nvgBeginPath(ctx);
    nvgRect(ctx, mPos.x(), mPos.y(), mSize.x(), mSize.y());
    nvgStrokeColor(ctx, Color(100, 255));
    nvgStroke(ctx);
}

NAMESPACE_END(nanogui)
