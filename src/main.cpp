#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>
#include "SETTINGS.h"
#include "buttons.cpp"


using namespace std;



double lastUpdateTime = 0;

bool ElementInDeque(Vector2 element, deque<Vector2> deque)
{
    for(unsigned int i = 0; i < deque.size(); i++)
    {
        if(Vector2Equals(deque[i], element))
        {
            return true;
        }
    }
    return false;
}

bool eventTriggered(double interval)
{
    double currentTime = GetTime();
    if(currentTime -lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

class Snake
{
    public:
        deque<Vector2> body = {Vector2{6, 9}, Vector2{5, 9}, Vector2{4, 9}};
        Vector2 direction = {1, 0};
        bool addSegment = false;
        bool keyPressedThisUpdate = false;
        int left_press;
        int up_press;
        int right_press;
        int down_press;

        Snake(int left, int up, int right, int down)
        {
            left_press = left;
            up_press = up;
            right_press = right;
            down_press = down;
        }

        void Draw()
        {
            for(unsigned int i = 0; i < body.size(); i++)
            {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{OFFSET+x*CELL_SIZE, OFFSET+y*CELL_SIZE, (float)CELL_SIZE, (float)CELL_SIZE};
                DrawRectangleRounded(segment, 0.5, 6, DARK_GREEN);
            }
        }

        void UpdateDirection()
        {
            if(IsKeyPressed(up_press) && direction.y != 1 && !keyPressedThisUpdate)
            {
                direction = {0, -1};
                keyPressedThisUpdate = true;
            }
            if(IsKeyPressed(down_press) && direction.y != -1 && !keyPressedThisUpdate)
            {
                direction = {0, 1};
                keyPressedThisUpdate = true;
            }
            if(IsKeyPressed(left_press) && direction.x != 1 && !keyPressedThisUpdate)
            {
                direction = {-1, 0};
                keyPressedThisUpdate = true;
            }
            if(IsKeyPressed(right_press) && direction.x != -1 && !keyPressedThisUpdate)
            {
                direction = {1, 0};
                keyPressedThisUpdate = true;
            }
        }

        void Update()
        {
            body.push_front(Vector2Add(body[0], direction));
            if(addSegment == true)
            {
                
                addSegment = false;
            }
            else
            {
                body.pop_back();
            }

        }

        void Reset()
        {
            body = {Vector2{6, 9}, Vector2{5,9}, Vector2{4,9}};
            direction = {1, 0};
        }

};

class Food
{

    public:
        Texture2D texture;
        Vector2 position;
        Food(deque<Vector2> snakeBody)
        {
            Image image = LoadImage(FOOD_IMAGE_PATH);
            texture = LoadTextureFromImage(image);
            UnloadImage(image);
            position = GenerateRandomPos(snakeBody);
        }

        ~Food()
        {
            UnloadTexture(texture);
        }
        void Draw()
        {
            DrawTexture(texture, OFFSET+position.x*CELL_SIZE, OFFSET+position.y*CELL_SIZE, WHITE);//DrawRectangle(position.x*cellSize, position.y*cellSize, cellSize, cellSize, DARK_GREEN);
        }

        Vector2 GenerateRandomCell()
        {
            float x = GetRandomValue(0, CELL_COUNT-1);
            float y = GetRandomValue(0, CELL_COUNT-1);
            return Vector2{x, y};
        }

        Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
        {

            Vector2 position = GenerateRandomCell();
            while(ElementInDeque(position, snakeBody))
            {
                position = GenerateRandomCell();
            }
            return position;
            
        }
};

class Game 
{
    public:
        Snake snake = Snake(KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN);
        Food food = Food(snake.body);
        Push_Btn btn = Push_Btn(WINDOW_WIDTH/4, WINDOW_HIGHT/4, 200, 80);
        bool running = false;
        int score = 0;
        Sound eatSound;
        Sound gameOverSound;

        Game()
        {
            InitAudioDevice();
            eatSound = LoadSound("Sounds/eat.mp3");
            gameOverSound = LoadSound("Sounds/wall.mp3");
        }

        ~Game()
        {
            UnloadSound(eatSound);
            UnloadSound(gameOverSound);
            CloseAudioDevice();
        }
        void Draw()
        {
            food.Draw();
            snake.Draw();
            if(!running)
            {
                btn.Draw_Btn();
                DrawText("PRESS SPACE TO START", (WINDOW_WIDTH/2-350), WINDOW_HIGHT/2, 40, DARK_GREEN);
            }
            
        }

        void Update()
        {
            btn.Btn_pressed();
            if(running)
            {
                
                snake.Update();
                CheckCollisionWithFood();
                CheckCollisionWithEdges();
                CheckCollisionWithTail();
            }

        }

        void CheckCollisionWithFood()
        {
           if(Vector2Equals(snake.body[0], food.position))
           {
                food.position = food.GenerateRandomPos(snake.body);
                snake.addSegment = true;
                score ++;
                PlaySound(eatSound);
           } 
        }

        void CheckCollisionWithEdges()
        {
            if(snake.body[0].x == CELL_COUNT || snake.body[0].x == -1)
            {
                GameOver();
            }
            if( snake.body[0].y == CELL_COUNT || snake.body[0].y == -1)
            {
                GameOver();
                
            }
        }

        
        void CheckCollisionWithTail()
        {
            deque<Vector2> headlessBody = snake.body;
            headlessBody.pop_front();
            if(ElementInDeque(snake.body[0], headlessBody))
            {
                GameOver();
            }
        }
        void startGame()
        {
            running = true;
            snake.Reset();
            food.position = food.GenerateRandomPos(snake.body);
            score = 0;
        }

        void GameOver()
        {

            running = false;
            PlaySound(gameOverSound);
        }

};



int main()
{
    cout << "print shit" << endl;
    InitWindow(WINDOW_WIDTH, WINDOW_HIGHT, "SNAKE");
    SetTargetFPS(60);

    Game game = Game();
    
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        if(eventTriggered(0.2))
        {
            game.Update();
            game.snake.keyPressedThisUpdate = false;       
        }
        game.snake.UpdateDirection();


        if(IsKeyPressed(KEY_SPACE))
        {
            std::cout << "SPACE IS PRESSED" << std::endl;
            game.snake.keyPressedThisUpdate = false;
            game.startGame();
        }

        // DRAWING
        ClearBackground(GREEN_LIGHT);
        DrawRectangleLinesEx(Rectangle{(float)OFFSET-5, (float)OFFSET-5, (float)CELL_SIZE*CELL_COUNT+10, (float)CELL_SIZE*CELL_COUNT+10}, 5, DARK_GREEN);
        DrawText("snake", OFFSET -5, 20, 40, DARK_GREEN);
        DrawText(TextFormat("%i", game.score), OFFSET -5, OFFSET+CELL_SIZE*CELL_COUNT+3, 40, DARK_GREEN);
        game.Draw();
        EndDrawing();

    }

    CloseWindow();
    return 0; 
}