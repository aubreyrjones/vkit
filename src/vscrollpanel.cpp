/*
    src/vscrollpanel.cpp -- Adds a vertical scrollbar around a widget
    that is too big to fit into a certain area

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/vscrollpanel.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <iostream>

NAMESPACE_BEGIN(nanogui)


VScrollPanel::VScrollPanel(ref<Widget> parent)
		: Widget(parent), mChildPreferredHeight(0), mScroll(0.0f) { }

void VScrollPanel::setMaxHeight(int maxHeight) {
	this->maxHeight = maxHeight;
}

int VScrollPanel::getMaxHeight() const {
	return maxHeight;
}

void VScrollPanel::performLayout(NVGcontext *ctx) {
	if (mChildren.empty())
		return;

	ref<Widget> child = mChildren[0];
	mChildPreferredHeight = child->preferredSize(ctx).y();
	child->setPosition(Vector2i(0, 0));

	child->setSize(child->preferredSize(ctx));
	child->performLayout(ctx);
}


bool VScrollPanel::mouseDragEvent(const Vector2i &, const Vector2i &rel, int, int) {
	if (mChildren.empty())
		return false;

	float scrollh = height() *
					std::min(1.0f, height() / (float) mChildPreferredHeight);

	mScroll = std::max((float) 0.0f, std::min((float) 1.0f,
											  mScroll + rel.y() / (float) (mSize.y() - 8 - scrollh)));
	if (scrollCallback) {
		scrollCallback();
	}

	return true;
}

bool VScrollPanel::scrollEvent(const Vector2i &/* p */, const Vector2f &rel) {
	float scrollAmount = rel.y() * (mSize.y() / 20.0f);
	float scrollh = height() * std::min(1.0f, height() / (float) mChildPreferredHeight);

	mScroll = std::max((float) 0.0f, std::min((float) 1.0f,
											  mScroll - scrollAmount / (float) (mSize.y() - 8 - scrollh)));

	if (scrollCallback) {
		scrollCallback();
	}

	return true;
}

static constexpr int scrollThumbWidth = 12;
static constexpr int leftScrollMargin = 4;
static constexpr int rightScrollMargin = 4;
static constexpr int totalScrollMarginH = leftScrollMargin + rightScrollMargin;
static constexpr int totalScrollWidth = scrollThumbWidth + totalScrollMarginH;
static constexpr int scrollTopMargin = 4;
static constexpr int scrollButtomMargin = 4;
static constexpr int totalScrollMarginV = scrollTopMargin + scrollButtomMargin;


Vector2i VScrollPanel::preferredSize(NVGcontext *ctx) {
	if (mChildren.empty())
		return Vector2i::Zero();

	ref<Widget>  child = mChildren[0];

	Vector2i preferredSize = child->preferredSize(ctx) + Vector2i(totalScrollWidth, 0);

	if (preferredSize.y() > maxHeight) {
		preferredSize.y() = maxHeight;
	}

	return preferredSize;
}

void VScrollPanel::draw(NVGcontext *ctx) {
	if (mChildren.empty())
		return;
	ref<Widget> child = mChildren[0];
	mChildPreferredHeight = child->preferredSize(ctx).y();
	float scrollh = height() *
					std::min(1.0f, height() / (float) mChildPreferredHeight);
	child->setPosition(Vector2i(0, -mScroll * (mChildPreferredHeight - mSize.y())));

	nvgSave(ctx);
	nvgTranslate(ctx, mPos.x(), mPos.y());

	nvgSave(ctx);
	nvgScissor(ctx, 0, 0, mSize.x() - scrollThumbWidth, mSize.y());
	if (child->visible())
		child->draw(ctx);
	nvgRestore(ctx);

	// draw the scroll tab
	float scrollXPos = mSize.x() - totalScrollWidth + leftScrollMargin;

	NVGpaint paint = nvgBoxGradient(ctx,
									scrollXPos + 1,
									scrollTopMargin + 1,
									scrollThumbWidth - rightScrollMargin,
									mSize.y() - totalScrollMarginV,
									3,
									4,
									Color(0, 32),
									Color(0, 92));
	nvgBeginPath(ctx);
	nvgRoundedRect(ctx,
				   scrollXPos,
				   4,
				   scrollThumbWidth,
				   mSize.y() - 8,
				   3);
	nvgFillPaint(ctx, paint);
	nvgFill(ctx);

	// these casts right here are why Eigen makes a bad small vector library
	float thumbStart = std::max(0.0f + scrollTopMargin, (float) (4 + 1 + (mSize.y() - 8 - scrollh) * mScroll));
	float thumbHeight = std::min(scrollh - 2, (float) mSize.y() - totalScrollMarginV);

	paint = nvgBoxGradient(ctx,
						   scrollXPos - 1,
						   thumbStart - 2,
						   scrollThumbWidth - rightScrollMargin - 1,
						   thumbHeight + 2,
						   3,
						   4,
						   Color(220, 100),
						   Color(128, 100));

	nvgBeginPath(ctx);
	nvgRoundedRect(ctx,
				   scrollXPos + 1,
				   thumbStart,
				   scrollThumbWidth - 1,
				   thumbHeight,
				   2);
	nvgFillPaint(ctx, paint);
	nvgFill(ctx);

	nvgRestore(ctx);
}

NAMESPACE_END(nanogui)
