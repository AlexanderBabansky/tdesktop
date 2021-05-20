#include "settings/settings_hackers.h"

#include "settings/settings_common.h"
#include "ui/effects/animations.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/wrap/slide_wrap.h"
#include "ui/widgets/checkbox.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/discrete_sliders.h"
#include "lang/lang_keys.h"
#include "window/notifications_manager.h"
#include "window/window_session_controller.h"
#include "platform/platform_specific.h"
#include "platform/platform_notifications_manager.h"
#include "base/platform/base_platform_info.h"
#include "mainwindow.h"
#include "core/application.h"
#include "main/main_session.h"
#include "main/main_account.h"
#include "main/main_domain.h"
#include "apiwrap.h"
#include "facades.h"
#include "app.h"
#include "styles/style_settings.h"
#include "styles/style_boxes.h"
#include "styles/style_window.h"
#include "styles/style_dialogs.h"

#include <QTimer>

namespace Settings {
	Hackers::Hackers(
		QWidget* parent,
		not_null<Window::SessionController*> controller)
		: Section(parent) {
		setupContent(controller);
	}

	void SetupHackersContent(
		not_null<Window::SessionController*> controller,
		not_null<Ui::VerticalLayout*> container) {
		const auto session = &controller->session();
		const auto checkbox = [&](const QString& label, bool checked) {
			return object_ptr<Ui::Checkbox>(
				container,
				label,
				checked,
				st::settingsCheckbox);
		};
		const auto addCheckbox = [&](const QString& label, bool checked) {
			return container->add(
				checkbox(label, checked),
				st::settingsCheckboxPadding);
		};
		const auto& settings = Core::App().settings();

		const auto cbGhMode = addCheckbox(
			"Ghost Mode",
			settings.ghostMode());
		const auto cbADeleter = addCheckbox(
			"Anti Deleter",
			settings.antiDeleter());

		cbGhMode->checkedChanges() | rpl::filter([](bool checked) {
			return (checked != Core::App().settings().ghostMode());
			}) | rpl::start_with_next([=](bool checked) {
				Core::App().settings().setGhostMode(checked);
				Core::App().saveSettingsDelayed();
				}, cbGhMode->lifetime());

		cbADeleter->checkedChanges() | rpl::filter([](bool checked) {
			return (checked != Core::App().settings().antiDeleter());
			}) | rpl::start_with_next([=](bool checked) {
				Core::App().settings().setAntiDeleter(checked);
				Core::App().saveSettingsDelayed();
				}, cbADeleter->lifetime());
	}

	void SetupHackers(
		not_null<Window::SessionController*> controller,
		not_null<Ui::VerticalLayout*> container) {
		AddSkip(container, st::settingsCheckboxesSkip);
		auto wrap = object_ptr<Ui::VerticalLayout>(container);
		SetupHackersContent(controller, wrap.data());
		container->add(object_ptr<Ui::OverrideMargins>(
			container,
			std::move(wrap)));
		AddSkip(container, st::settingsCheckboxesSkip);
	}

	void Hackers::setupContent(
		not_null<Window::SessionController*> controller) {
		const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);

		SetupHackers(controller, content);

		Ui::ResizeFitChild(this, content);
	}
}
