#include <raylib.h>
#include <utility>

int player_score = 0;
int cpu_score = 0;

Color Green{38, 185, 154, 255};
Color Dark_Green{20, 160, 133, 255};
Color Light_Green{129, 204, 184, 255};
Color Yellow{243, 213, 91, 255};
                            
class Ball {
    public:
  Ball(float x, float y, int radius, int speed_x, int speed_y)
         : x(x), y(y), radius(radius), speed_x(speed_x), speed_y(speed_y) {}
    void Draw() {
        DrawCircle(x, y, radius, Yellow);
    }
    void Update() {
        x+=speed_x;
        y+=speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y*=-1; 
        }
        if (x + radius >= GetScreenWidth()) {
          cpu_score++; 
		  ResetBall();
		}
		if(x - radius <= 0) {
			player_score++;
            ResetBall();
		}

    }
    void UpdateSpeed(int x, int y) { speed_x =x, speed_y = y;}

	void ResetBall() {
	
		x = GetScreenHeight() / 2;
        y = GetScreenWidth() / 2;

		int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
		speed_y *= speed_choices[GetRandomValue(0, 1)];

	}

	std::pair<int, int> GetCoordinates() { return {x, y}; }

	int GetRadius() { return radius; }
        std::pair<int, int> GetSpeed() { return {speed_x,speed_y}; }

    private:
        float x,y;
        int speed_x, speed_y;
        int radius;

};

class Paddle {
    public: 
        Paddle() {}  
        Paddle(float x, float y, float width, float height, int speed)
            : x(x), y(y), width(width), height(height), speed(speed) {}  
        void Draw() {
          DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
        }   

        void Update() {
          if (IsKeyDown(KEY_UP)) {
            y = y - speed;           
}
          if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;       
		  }   
			Movement();
        }

        virtual float GetWidth() { return width; }
        virtual void SetWidth(float width) { this->width = width; }
        float GetHeight() {return height;}
        void SetHeight(float height) { this->height= height; }
        void UpdateSpeed(int speed) {this->speed = speed;}
        void SetCoordinates(int x, int y) {this->x = x; this->y = y;}
        std::pair<int, int> GetCoordinates() {
          return {x,y};
        }
        int GetSpeed() { return speed; }

	protected:
       void Movement() {
           if (y <= 0) {
             y = 0;
           }
           if (y + height > GetScreenHeight()) {
             y = GetScreenHeight() - height;
           }
         }

	private:
        float x, y;
        float width, height;
        int speed;

};

class CPUPaddle : public Paddle {
    public:
  void Update(int ball_y) {
    std::pair<int, int> coordinates = GetCoordinates();
    if (coordinates.second + GetHeight() / 2 > ball_y) {
      SetCoordinates(coordinates.first, coordinates.second - GetSpeed());
	}
    if (coordinates.second + GetHeight() / 2 < ball_y) {
          SetCoordinates(coordinates.first, coordinates.second + GetSpeed());
	}
    Movement();
  }

};

void CheckCollision(std::pair<int,int> ball_coordinates, std::pair<int,int> paddle_coordinates, Ball &ball, Paddle &paddle) {
  if (CheckCollisionCircleRec(
          Vector2{(float)ball_coordinates.first,
                  (float)ball_coordinates.second},  // Circle center (x, y)
          ball.GetRadius(),                         // Circle radius
          Rectangle{
              float(paddle_coordinates.first),  // rectangle x
              float(paddle_coordinates.second),  // rectangle y
              paddle.GetWidth(),                 // rectangle width
              paddle.GetHeight()                 // rectangle height
          })) {
    std::pair<int, int> speed = ball.GetSpeed();
    ball.UpdateSpeed(speed.first * -1, speed.second);
  }
}

int main() {

	const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 800;
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pong");
    SetTargetFPS(60);

    Ball ball(SCREEN_WIDTH/ 2, SCREEN_HEIGHT/ 2, 20,7,7);

    Paddle paddle;
    paddle.SetWidth(25);
    paddle.SetHeight(120);
    paddle.SetCoordinates(SCREEN_WIDTH - paddle.GetWidth() -10, SCREEN_HEIGHT/2 - paddle.GetHeight()/2);
    paddle.UpdateSpeed(6);

    CPUPaddle cpu_paddle;
    cpu_paddle.SetWidth(25);
    cpu_paddle.SetHeight(120);
    cpu_paddle.SetCoordinates(10, SCREEN_HEIGHT / 2 - cpu_paddle.GetHeight() / 2);
    cpu_paddle.UpdateSpeed(6);


	while (WindowShouldClose() == false) {
	   BeginDrawing();

       ball.Update();
       paddle.Update();
       std::pair<int, int> ball_coordinates = ball.GetCoordinates();
       std::pair<int, int> paddle_coordinates = paddle.GetCoordinates();
       std::pair<int, int> cpu_coordinates = cpu_paddle.GetCoordinates();
       cpu_paddle.Update(ball_coordinates.second);

	   CheckCollision(ball_coordinates, paddle_coordinates, ball, paddle);
       CheckCollision(ball_coordinates, cpu_coordinates, ball, cpu_paddle);

       ClearBackground(Dark_Green);
       DrawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT,
                     Green);
       DrawCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 150, Light_Green);

       DrawLine(SCREEN_WIDTH/2,0,SCREEN_WIDTH/2,SCREEN_HEIGHT,WHITE);
       ball.Draw();
       paddle.Draw();
       cpu_paddle.Draw();
       DrawText(TextFormat("%i", cpu_score), SCREEN_WIDTH / 4 - 20, 20, 80,
                WHITE);
       DrawText(TextFormat("%i", player_score), 3 * SCREEN_WIDTH / 4 - 20, 20, 80,
                WHITE);
	   EndDrawing();
	}

	CloseWindow();
	return 0;
}

