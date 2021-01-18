#pragma once
namespace gui {
	// an enum class for event types
	enum class Event {
		MouseHover,
		MouseMove,

		LeftMousePress,
		LeftMouseRelease,

		RightMousePress,
		RightMouseRelease,

		MouseWheelPress,
		MouseWheelUp,
		MouseWheelDown
	};
}