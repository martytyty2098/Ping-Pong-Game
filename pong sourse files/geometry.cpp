#include "utils.cpp"

class GeometricShape {
public:
	virtual void draw() = 0;
	bool colliderect(GeometricShape& shape);
	GeometricShape(float spawn_x, float spawn_y, float _width, float _height, ui32 _color);
	float x, y;
	float width, height;
	ui32 color;
	inline int int_x();
	inline int int_y();
	inline int int_height();
	inline int int_width();
};

inline int GeometricShape::int_height() {
	return (int)floor(render_state.buffer_height * height);
}

inline int GeometricShape::int_width() {
	return (int)floor(render_state.buffer_width * width);
}

inline int GeometricShape::int_x() {
	return (int)floor(render_state.buffer_width * x);
}

inline int GeometricShape::int_y() {
	return (int)floor(render_state.buffer_height * y);
}

bool GeometricShape::colliderect(GeometricShape& shape)
{
	int other_x = shape.int_x();
	int other_y = shape.int_y();
	int other_width = shape.int_width();
	int other_height = shape.int_height();
	for (int temp_y = int_y(); temp_y < int_y() + int_height(); ++temp_y) {
		for (int temp_x = int_x(); temp_x < int_x() + int_width(); ++temp_x) {
			if (temp_x >= other_x and temp_x <= other_x + other_width and
				temp_y >= other_y and temp_y <= other_y + other_height) return true;
		}
	}
	return false;
}

GeometricShape::GeometricShape(float spawn_x, float spawn_y, float _width, float _height, ui32 _color)
	:x(spawn_x), y(spawn_y),
	width(_width), height(_height),
	color(_color)
{}

class Rect : public GeometricShape {
public:
	Rect(float spawn_x, float spawn_y, float _width, float _height, ui32 _color)
		: GeometricShape(spawn_x, spawn_y, _width, _height, _color) {};
	virtual void draw();
private:
	Rect();
};
