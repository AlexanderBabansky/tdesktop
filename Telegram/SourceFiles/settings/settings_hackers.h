#pragma once

#include "settings/settings_common.h"

namespace Settings {

	class Hackers : public Section {
	public:
		Hackers(
			QWidget* parent,
			not_null<Window::SessionController*> controller);

	private:
		void setupContent(not_null<Window::SessionController*> controller);

	};

} // namespace Settings
