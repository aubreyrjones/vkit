/*
    src/messagedialog.cpp -- Simple "OK" or "Yes/No"-style modal dialogs

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#include <nanogui/messagedialog.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>
#include <nanogui/entypo.h>
#include <nanogui/label.h>

NAMESPACE_BEGIN(nanogui)

MessageDialog::MessageDialog(ref<Widget> parent, Type type, const std::string &title,
							 const std::string &message,
							 const std::string &buttonText,
							 const std::string &altButtonText, bool altButton) : Window(parent, title) {
	setLayout(makeref<BoxLayout>(Orientation::Vertical,
							Alignment::Middle, 10, 10));
	setModal(true);

	ref<Widget> panel1 = makeref<Widget>(shared_from_this());
	panel1->setLayout(makeref<BoxLayout>(Orientation::Horizontal,
										 Alignment::Middle, 10, 15));
	int icon = 0;
	switch (type) {
	case Type::Information:
		icon = ENTYPO_ICON_CIRCLED_INFO;
		break;
	case Type::Question:
		icon = ENTYPO_ICON_CIRCLED_HELP;
		break;
	case Type::Warning:
		icon = ENTYPO_ICON_WARNING;
		break;
	}
	ref<Label> iconLabel = makeref<Label>(panel1, std::string(utf8(icon).data()), "icons");
	iconLabel->setFontSize(50);
	ref<Label> msgLabel = makeref<Label>(panel1, message);
	msgLabel->setFixedWidth(200);
	ref<Widget> panel2 = makeref<Widget>(shared_from_this());
	panel2->setLayout(makeref<BoxLayout>(Orientation::Horizontal,
									Alignment::Middle, 0, 15));

	if (altButton) {
		ref<Button> button = makeref<Button>(panel2, altButtonText, ENTYPO_ICON_CIRCLED_CROSS);
		button->setCallback([&] {
			if (mCallback) mCallback(1);
			dispose();
		});
	}

	ref<Button> button = makeref<Button>(panel2, buttonText, ENTYPO_ICON_CHECK);
	button->setCallback([&] {
		if (mCallback) mCallback(0);
		dispose();
	});
	center();
	requestFocus();
}

NAMESPACE_END(nanogui)
