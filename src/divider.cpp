//
// Created by ARJ on 11/9/15.
//

#include <nanogui/divider.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>

namespace nanogui {

Divider::Divider(ref<Widget> parent, Orientation const& orientation, int fillSize, float const& lineWidth) :
		Widget(parent),
		mFillSize(fillSize),
		mOrientation(orientation),
		mLineWidth(lineWidth)
{ }


void Divider::draw(NVGcontext *ctx) {
//	drawBounds(ctx);

	nvgSave(ctx);
	nvgTranslate(ctx, mPos.x, mPos.y);

	Vector2f lineStart = Vector2f(0), lineEnd(mSize.x, mSize.y);

	int freeAxis = mOrientation == Orientation::Horizontal ? 1 : 0;

	lineStart[freeAxis] = lineEnd[freeAxis] = (mSize[freeAxis] / 2.0f) - (mLineWidth / 2.0f);

	nvgBeginPath(ctx);
	nvgMoveTo(ctx, lineStart.x, lineStart.y);
	nvgLineTo(ctx, lineEnd.x, lineEnd.y);

	nvgStrokeColor(ctx, mTheme->mDividerColor);
	nvgStrokeWidth(ctx, mLineWidth);
	nvgStroke(ctx);

	nvgRestore(ctx);
	}

Vector2i Divider::preferredSize(NVGcontext *ctx) {
	int freeAxis = mOrientation == Orientation::Horizontal ? 1 : 0;

	Vector2i retval = mSize;
	mSize[freeAxis] = mFillSize;

	return retval;
}

}