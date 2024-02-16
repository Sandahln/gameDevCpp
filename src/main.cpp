#include <raylib.h>
#include <iostream>
#include <deque>
#include <raymath.h>

using namespace std;

Color green = {173, 204, 96, 255};
Color darkGreen = {43, 51, 24, 255};
int cellSize = 30;
int cellCount = 25;
int offset = 75;

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
        void Snake(enum left, enum up, enum right, enum down)
        {
            enum left_press = left;
            enum up_press = up;
            enum right_press = right;
            enum down_press = down;
        }

        void Draw()
        {
            for(unsigned int i = 0; i < body.size(); i++)
            {
                float x = body[i].x;
                float y = body[i].y;
                Rectangle segment = Rectangle{offset+x*cellSize, offset+y*cellSize, (float)cellSize, (float)cellSize};
                DrawRectangleRounded(segment, 0.5, 6, darkGreen);
            }
        }

        void UpdateDirection()
        {

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
            Image image = LoadImage("C:/gameDev/Graphics/food.PNG");
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
            DrawTexture(texture, offset+position.x*cellSize, offset+position.y*cellSize, WHITE);//DrawRectangle(position.x*cellSize, position.y*cellSize, cellSize, cellSize, darkGreen);
        }

        Vector2 GenerateRandomCell()
        {
            float x = GetRandomValue(0, cellCount-1);
            float y = GetRandomValue(0, cellCount-1);
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
        Snake snake = Snake();
        Food food = Food(snake.body);
        bool running = true;
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
        }

        void Update()
        {
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
            if(snake.body[0].x == cellCount || snake.body[0].x == -1)
            {
                GameOver();
            }
            if( snake.body[0].y == cellCount || snake.body[0].y == -1)
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
    InitWindow(2*offset +cellSize*cellCount, 2*offset + cellSize*cellCount, "SNAKE");
    SetTargetFPS(60);

    Game game = Game();
    bool keyPressedThisUpdate = false;
    while(WindowShouldClose() == false)
    {
        BeginDrawing();
        if(eventTriggered(0.2))
        {
            game.Update();
            keyPressedThisUpdate = false;       
        }
        
        if(IsKeyPressed(KEY_UP) && game.snake.direction.y != 1 && !keyPressedThisUpdate)
        {
            game.snake.direction = {0, -1};
            keyPressedThisUpdate = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1 && !keyPressedThisUpdate)
        {
            game.snake.direction = {0, 1};
            keyPressedThisUpdate = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1 && !keyPressedThisUpdate)
        {
            game.snake.direction = {-1, 0};
            keyPressedThisUpdate = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1 && !keyPressedThisUpdate)
        {
            game.snake.direction = {1, 0};
            keyPressedThisUpdate = true;
        }

        if(IsKeyPressed(KEY_SPACE))
        {
            keyPressedThisUpdate = false;
            game.startGame();
        }

        // DRAWING
        ClearBackground(green);
        DrawRectangleLinesEx(Rectangle{(float)offset-5, (float)offset-5, (float)cellSize*cellCount+10, (float)cellSize*cellCount+10}, 5, darkGreen);
        DrawText("snake", offset -5, 20, 40, darkGreen);
        DrawText(TextFormat("%i", game.score), offset -5, offset+cellSize*cellCount+3, 40, darkGreen);
        game.Draw();
        EndDrawing();

    }

    CloseWindow();
    return 0; 
}