/*
    src/theme.cpp -- Storage class for basic theme-related properties

    The text box widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/theme.h>
#include <nanogui/opengl.h>
#include <resources.h>

NAMESPACE_BEGIN(nanogui)

Theme::Theme(NVGcontext *ctx) {
    mStandardFontSize                 = 16;
    mButtonFontSize                   = 20;
    mTextBoxFontSize                  = 20;
    mWindowCornerRadius               = 2;
    mWindowHeaderHeight               = 30;
    mWindowDropShadowSize             = 10;
    mButtonCornerRadius               = 2;

    mDropShadow                       = Color(0, 128);
    mTransparent                      = Color(0, 0);
    mBorderDark                       = Color(29, 255);
    mBorderLight                      = Color(92, 255);
    mBorderMedium                     = Color(35, 255);
    mTextColor                        = Color(255, 160);
    mDisabledTextColor                = Color(255, 80);
    mTextColorShadow                  = Color(0, 160);
    mIconColor                        = mTextColor;
	mDividerColor                     = mBorderMedium;

    mButtonGradientTopFocused         = Color(64, 255);
    mButtonGradientBotFocused         = Color(48, 255);
    mButtonGradientTopUnfocused       = Color(74, 255);
    mButtonGradientBotUnfocused       = Color(58, 255);
    mButtonGradientTopPushed          = Color(32, 255);
    mButtonGradientBotPushed          = Color(16, 255);

    /* Window-related */
    mWindowFillUnfocused              = Color(43, 250);
    mWindowFillFocused                = Color(45, 250);
    mWindowTitleUnfocused             = Color(220, 255);
    mWindowTitleFocused               = Color(255, 255);

    mWindowHeaderGradientTop          = mButtonGradientTopUnfocused;
    mWindowHeaderGradientBot          = mButtonGradientBotUnfocused;
    mWindowHeaderSepTop               = mBorderLight;
    mWindowHeaderSepBot               = mBorderDark;

    mWindowPopup                      = Color(50, 255);
    mWindowPopupTransparent           = Color(50, 0);

	_r::buffer robotoRegular = _r::r("assets/Roboto-Regular.ttf");
	_r::buffer robotoBold = _r::r("assets/Roboto-Bold.ttf");
	_r::buffer iconFont = _r::r("assets/entypo.ttf");
	_r::buffer faFont = _r::r("assets/fontawesome.ttf");

    mFontNormal = nvgCreateFontMem(ctx, "sans", const_cast<uint8_t *>(robotoRegular.data),
                                   robotoRegular.length, 0);
    mFontBold = nvgCreateFontMem(ctx, "sans-bold", const_cast<uint8_t *>(robotoBold.data),
                                 robotoBold.length, 0);
    mFontIcons = nvgCreateFontMem(ctx, "icons", const_cast<uint8_t *>(iconFont.data),
                                  iconFont.length, 0);
	mFontMoreIcons = nvgCreateFontMem(ctx, "fa", const_cast<uint8_t *>(faFont.data),
								  faFont.length, 0);

    if (mFontNormal == -1 || mFontBold == -1 || mFontIcons == -1)
        throw std::runtime_error("Could not load fonts!");
}

NAMESPACE_END(nanogui)
