
/*
    src/popupbutton.cpp -- Button which launches a popup widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/popupbutton.h>
#include <nanogui/entypo.h>
#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <iostream>

NAMESPACE_BEGIN(nanogui)

PopupButton::PopupButton(ref<Widget> parent, const std::string &caption,
                         int buttonIcon, int chevronIcon)
    : Button(parent, caption, buttonIcon), mChevronIcon(chevronIcon) {

    setFlags(Flags::ToggleButton | Flags::PopupButton);
}

void PopupButton::associate() {
	Widget::associate();

	ref<Window> parentWindow = window();
	mPopup = makewidget<Popup>(parentWindow->parent(), window());
	mPopup->setSize(Vector2i(320, 250));
	mPopup->setVisible(false);

	setChangeCallback([this](bool pushed) { mPopup->setVisible(pushed);});
}

Vector2i PopupButton::preferredSize(NVGcontext *ctx){
	nvgFontSize(ctx, mFontSize < 0 ? mTheme->mButtonFontSize : mFontSize);
	nvgFontFace(ctx, "fa");

	auto icon = utf8(mChevronIcon);

	float chevronWidth = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);

    return Button::preferredSize(ctx) + Vector2i(chevronWidth, 0);
}

void PopupButton::draw(NVGcontext* ctx) {
    if (!mEnabled && mPushed)
        mPushed = false;

	ref<const Window> parentWindow = window();
	Vector2i anchor = Vector2i(parentWindow->width() + 15, absolutePosition().y() - parentWindow->position().y() + mSize.y() / 2);
	mPopup->setAnchorPos(anchor);

    Button::draw(ctx);

    if (mChevronIcon) {
        auto icon = utf8(mChevronIcon);
        NVGcolor textColor =
            mTextColor.w() == 0 ? mTheme->mTextColor : mTextColor;

        nvgFontSize(ctx, (mFontSize < 0 ? mTheme->mButtonFontSize : mFontSize));
        nvgFontFace(ctx, "fa");
        nvgFillColor(ctx, mEnabled ? textColor : mTheme->mDisabledTextColor);
        nvgTextAlign(ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);

        float iw = nvgTextBounds(ctx, 0, 0, icon.data(), nullptr, nullptr);
        Vector2f iconPos(mPos.x() + mSize.x() - iw - 8,
                         mPos.y() + mSize.y() * 0.5f);

        nvgText(ctx, iconPos.x(), iconPos.y(), icon.data(), nullptr);
    }
}

NAMESPACE_END(nanogui)
