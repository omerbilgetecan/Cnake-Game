#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include <time.h>

#define WIDTH 600
#define HEIGHT 600

Color snakeColor = { 196, 77, 255 ,255 };
int snakeLenght = 1;
float fadePower = 0.5;
float speed = 4;

int maxFood = 2;
Vector2* foodPos;
Color foodCol = { 174, 255, 0 ,255 };

typedef struct snake {
	Vector2 pos;
	Color color;
}Snake;


void DrawGrid() {
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++) {
			DrawRectangleLines(j * 30, i * 30, 30, 30, BLACK);

		}
	}
	for (int i = 0; i < maxFood; i++)
	{
		DrawRectangle(foodPos[i].x, foodPos[i].y, 29, 29, foodCol);
	}

}



void Food() {

	srand(time(0));
	for (int i = 0; i < maxFood; i++)
	{

		int posX = (rand() % 20) * 30;
		int posY = (rand() % 20) * 30;
		foodPos[i].x = posX;
		foodPos[i].y = posY;

		DrawRectangle(posX, posY, 29, 29, foodCol);

	}

}

int CheckFail(Snake* snakeCell, int len) {

	for (int i = 1; i < len; i++)
	{
		if (snakeCell[0].pos.x == snakeCell[i].pos.x && snakeCell[0].pos.y == snakeCell[i].pos.y) {
			printf("Fail\n");

			snakeCell[0].pos.x = 300;
			snakeCell[0].pos.y = 300;

			return len = 1;
		}
	}
}

void Movement(Snake* snakeCell, Vector2 direction, int len) {


	Vector2* temp = malloc(sizeof(Vector2) * len);

	for (int i = 0; i < len; i++)
	{
		temp[i] = snakeCell[i].pos;

	}
	for (int i = 1; i < len; i++)
	{
		snakeCell[i].pos = temp[i - 1];
	}
	snakeCell[0].pos.x += direction.x * 30;
	snakeCell[0].pos.y += direction.y * 30;
	static Vector2 target;

	len = CheckFail(snakeCell, len);
	NewCell(len, snakeCell, direction);

}

void DrawSnake(int len, Vector2 direction, Snake* snakeCell) {


	static float time;
	for (int i = 0; i < len; i++)
	{
		Rectangle rec = { snakeCell[i].pos.x, snakeCell[i].pos.y , 29, 29 };
		DrawRectangleRec(rec, snakeCell[i].color);
	}


	if (time > 1) {
		Movement(snakeCell, direction, len);
		time = 0;
	}

	time += speed * GetFrameTime();

}

int NewCell(int len, Snake* snakeCell, Vector2 dir) {

	for (int i = 0; i < maxFood; i++)
	{
		if (snakeCell[0].pos.x == foodPos[i].x && snakeCell[0].pos.y == foodPos[i].y) {
			len++;
			snakeCell[len - 1].pos.x = snakeCell[len - 2].pos.x - dir.x * 30;
			snakeCell[len - 1].pos.y = snakeCell[len - 2].pos.y - dir.y * 30;
			snakeCell[len - 1].color = snakeCell[len - 2].color;
			snakeCell[len - 1].color.a = snakeCell[len - 2].color.a - fadePower;
			Food();

		}
	}

	snakeLenght = len;

}

int main() {
	InitWindow(WIDTH, HEIGHT, "Snake");
	SetTargetFPS(144);

	bool start = false;


	Vector2 direction = { 1,0 };

	foodPos = malloc(sizeof(Vector2*) * maxFood);

	Snake* snakeCell = malloc(sizeof(Snake) * HEIGHT * WIDTH);
	snakeCell[0].color = snakeColor;
	snakeCell[0].pos.x = 300;
	snakeCell[0].pos.y = 300;

	// Bg Color
	Color color = { 230,230,230,255 };



	//Game Loop
	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(color);
		DrawGrid();


		switch (GetKeyPressed())
		{
		case(KEY_DOWN):
			if (direction.y != -1) {
				direction.x = 0; direction.y = 1;
				Movement(snakeCell, direction, snakeLenght);
			}
			break;

		case(KEY_UP):
			if (direction.y != 1) {
				direction.x = 0; direction.y = -1;
				Movement(snakeCell, direction, snakeLenght);
			}
			break;

		case(KEY_RIGHT):
			if (direction.x != -1) {
				direction.x = 1; direction.y = 0;
				Movement(snakeCell, direction, snakeLenght);
			}
			break;
		case(KEY_LEFT):
			if (direction.x != 1) {
				direction.x = -1; direction.y = 0;
				Movement(snakeCell, direction, snakeLenght);
			}
			break;
		default:
			break;
		}


		if (IsKeyPressed(KEY_SPACE)) {
			snakeLenght = 1;
			snakeCell[0].pos.x = 300;
			snakeCell[0].pos.y = 300;
			Food();
			start = true;
		}

		if (start)
			DrawSnake(snakeLenght, direction, snakeCell);

		EndDrawing();
	}

	CloseWindow();
	return 0;
}