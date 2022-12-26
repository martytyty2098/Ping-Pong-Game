#include "game.cpp"

global bool resizing = false;

LRESULT CALLBACK window_callback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		game_is_running = false;
		break;
	case WM_SIZE: 
	{
		resizing = true;
		RECT rect;
		GetClientRect(hWnd, &rect);
		render_state.buffer_width = rect.right - rect.left;
		render_state.buffer_height = rect.bottom - rect.top;
		int buffer_size = render_state.buffer_width * render_state.buffer_height * sizeof(ui32);
		if (render_state.buffer_memory) VirtualFree(render_state.buffer_memory, 0, MEM_RELEASE);
		render_state.buffer_memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		render_state.buffer_bitmap_info.bmiHeader.biSize = sizeof(render_state.buffer_bitmap_info.bmiHeader);
		render_state.buffer_bitmap_info.bmiHeader.biWidth = render_state.buffer_width;
		render_state.buffer_bitmap_info.bmiHeader.biHeight = render_state.buffer_height;
		render_state.buffer_bitmap_info.bmiHeader.biPlanes = 1;
		render_state.buffer_bitmap_info.bmiHeader.biBitCount = 32;
		render_state.buffer_bitmap_info.bmiHeader.biCompression = BI_RGB;
	} break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Create Window Class
	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = L"Game Window";
	wc.lpfnWndProc = window_callback;

	// Register Class
	RegisterClass(&wc);

	// Create Window
	HWND window = CreateWindow(wc.lpszClassName, L"Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1280, 720, 0, 0, hInstance, 0); // CW_USEDEFAULT
	HDC hdc = GetDC(window);
	input input = {};

	// FPS Optimization
	float delta_time = 0.016666f;
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);
	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)perf.QuadPart;
	}

	while (game_is_running)
	{
		// Input
		MSG message;
		for (int i = 0; i < BUTTON_COUNT; ++i) input.buttons[i].changed = false;

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) 
		{
			ui32 vk_code;
			bool is_down;

			switch (message.message) {
			case WM_KEYUP:
			case WM_KEYDOWN:
				vk_code = (ui32)message.wParam;
				is_down = ((message.lParam & (1 << 31)) == 0);

				switch (vk_code) {
					process_button(BUTTON_UP, VK_UP)
					process_button(BUTTON_DOWN, VK_DOWN)
					process_button(BUTTON_LEFT, VK_LEFT)
					process_button(BUTTON_RIGHT, VK_RIGHT)
				}

				break;
			default:
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}

		// Simulate
		simulate_game(input, delta_time, resizing);

		// Render
		StretchDIBits(hdc, 0, 0, render_state.buffer_width, render_state.buffer_height, 0, 0, render_state.buffer_width, render_state.buffer_height, render_state.buffer_memory, &render_state.buffer_bitmap_info, DIB_RGB_COLORS, SRCCOPY);

		// FPS Optimization
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = float((frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency);
		if (delta_time > 0.01f) delta_time = 0.01f;
		frame_begin_time = frame_end_time;
		resizing = false;
	}
	return 0;
}