static bool game_is_running = true;

struct ButtonState {
	bool is_down = false;
	bool changed = false;
};

enum { BUTTON_UP, BUTTON_DOWN, BUTTON_LEFT, BUTTON_RIGHT, BUTTON_COUNT };

struct input {
	ButtonState buttons[BUTTON_COUNT];
};