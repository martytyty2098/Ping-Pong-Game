#include "renderer.cpp"
#include <ctime>

enum Direction { up_right, down_right, down_left, up_left };
static void move(Rect& shape, int dir, float speed);
static void collision(Rect& shape, Rect& player, Rect& enemy, int& dir);

void simulate_game(input& input, float MSPF, bool resizing)
{
	srand((unsigned int)time(0));
	static bool first_frame = true;
	static bool second_frame = false;
	static Rect player(0.01f, 0.5f, 0.02f, 0.25f, 0xFFFFFF);
	static Rect enemy(0.97f, 0.5f, 0.02f, 0.25f, 0XFFFFFF);
	static Rect ball(0.5f, 0.5f, 0.03f, 0.03f, 0xFFFFFF);
	static Rect line(0.5f, 0.0f, 0.001f, 1.0f, 0xFFFFFF);
	static int ball_dir = rand() % 2;
	float speed_per_second = 1.0f;
	float player_speed = speed_per_second * MSPF;

	if(second_frame and (is_down(BUTTON_UP) or is_down(BUTTON_DOWN))) second_frame = false;
	if (second_frame and !resizing) return;

	fill_screen(0);
	if (is_down(BUTTON_UP) and player.y + player_speed + player.height <= 1.0f) player.y += player_speed;
	if (is_down(BUTTON_DOWN) and player.y - player_speed >= 0) player.y -= player_speed;
	collision(ball, player, enemy, ball_dir);
	move(ball, ball_dir, 1.0f * MSPF);

	if (enemy.y + enemy.height / 2.0f > ball.y + ball.height / 2.0f and enemy.y - player_speed >= 0.0f) enemy.y -= player_speed;
	if (enemy.y + enemy.height / 2.0f < ball.y + ball.height / 2.0f and enemy.y + player_speed + enemy.height < 1.0f) enemy.y += player_speed;

	player.draw();
	enemy.draw();
	ball.draw();
	line.draw();

	if (first_frame) {
		first_frame = false;
		second_frame = true;
	}

	if (ball.x <= 0.0f or ball.x + ball.width >= 1.0f) game_is_running = false;
}

static void move(Rect& shape, int dir, float speed)
{
	switch (dir) {
	case up_right:
		shape.x += speed;
		shape.y += speed;
		break;
	case up_left:
		shape.y += speed;
		shape.x -= speed;
		break;
	case down_right:
		shape.x += speed;
		shape.y -= speed;
		break;
	case down_left:
		shape.y -= speed;
		shape.x -= speed;
		break;
	}
}

static void collision(Rect& shape, Rect& player, Rect& enemy, int& dir)
{
	if (shape.x <= 0.0f or shape.colliderect(player)) {
		if (dir == down_left) dir = down_right;
		if (dir == up_left) dir = up_right;
	}
	if (shape.y + shape.height >= 1.0f) {
		if (dir == up_left) dir = down_left;
		if (dir == up_right) dir = down_right;
	}
	if (shape.x + shape.width >= 1.0f or shape.colliderect(enemy)) {
		if (dir == down_right) dir = down_left;
		if (dir == up_right) dir = up_left;
	}
	if (shape.y <= 0.0f) {
		if (dir == down_left) dir = up_left;
		if (dir == down_right) dir = up_right;
	}
}