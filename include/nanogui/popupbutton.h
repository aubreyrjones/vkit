/*
    nanogui/popupbutton.h -- Button which launches a popup widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

#include <nanogui/button.h>
#include <nanogui/popup.h>
#include <nanogui/entypo.h>
#include <nanogui/font_awesome.h>

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT PopupButton : public Button {
public:
    PopupButton(ref<Widget> parent, const std::string &caption = "Untitled",
                int buttonIcon = 0,
                int chevronIcon = FA_CHEVRON_RIGHT);

    void setChevronIcon(int icon) { mChevronIcon = icon; }
    int chevronIcon() const { return mChevronIcon; }

    ref<Popup> popup() { return mPopup; }
    const ref<Popup> popup() const { return mPopup; }

    virtual void draw(NVGcontext* ctx);
    virtual Vector2i preferredSize(NVGcontext *ctx);

	virtual void associate() override;

protected:
    ref<Popup> mPopup;
    int mChevronIcon;
};

NAMESPACE_END(nanogui)
