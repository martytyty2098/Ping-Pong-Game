#include <windows.h>
#include <cmath>
#include "platform_common.cpp"

typedef unsigned int ui32;

#define process_button(b, vk)\
case vk:\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
break;

#define pressed(b) input.buttons[b].is_down and input.buttons[b].changed
#define released(b) (!input.buttons[b].is_down) and input.buttons[b].changed
#define is_down(b) input.buttons[b].is_down

#define global static
#define and &&
#define or ||

struct RENDER_STATE {
	int buffer_height, buffer_width;
	void* buffer_memory = nullptr;
	BITMAPINFO buffer_bitmap_info;
}global render_state;