//
// Created by ARJ on 11/9/15.
//


#include "widget.h"
#include "layout.h"


#ifndef OMNIBUILD_DIVIDER_H
#define OMNIBUILD_DIVIDER_H

NAMESPACE_BEGIN(nanogui)

class NANOGUI_EXPORT Divider : public Widget {
public:
	Divider(ref<Widget> parent, Orientation const& orientation = Orientation::Horizontal, int fillSize = 4, float const& lineWidth = 2.0f);

	virtual void draw(NVGcontext *ctx) override;

	virtual Vector2i preferredSize(NVGcontext *ctx) override;
protected:
	int mFillSize;
	Orientation mOrientation;
	float mLineWidth;
};

NAMESPACE_END(nanogui)

#endif //OMNIBUILD_DIVIDER_H
