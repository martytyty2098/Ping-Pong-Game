#include "geometry.cpp"

void Rect::draw()
{
	int temp_x = int_x();
	int temp_y = int_y();
	ui32* anchor_point = (ui32*)render_state.buffer_memory + render_state.buffer_width * temp_y + temp_x;
	for (int y0 = 0; y0 < int_height(); ++y0) {
		if (temp_y + y0 >= render_state.buffer_height or temp_y + y0 < 0) continue;
		for (int x0 = 0; x0 < int_width(); ++x0) {
			if (temp_x + x0 >= render_state.buffer_width or temp_x + x0 < 0) continue;
			*(anchor_point + render_state.buffer_width * y0 + x0) = color;
		}
	}
}

static void fill_screen(ui32 color)
{
	ui32* pixel = (ui32*)render_state.buffer_memory;
	for (int y = 0; y < render_state.buffer_height; ++y) {
		for (int x = 0; x < render_state.buffer_width; ++x) {
			*pixel++ = color;
		}
	}
}

static void clear_screen() {
	fill_screen(0);
}
