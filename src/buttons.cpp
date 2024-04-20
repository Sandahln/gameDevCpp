#include "raylib.h"
#include <iostream>

class Push_Btn
{
    float btn_x;
    float btn_y;
    float btn_width;
    float btn_height;

    Color buttonColor = GRAY;
    Color textColor = WHITE;

    Rectangle button_bounds;
    public:
        Push_Btn(int set_btn_x, int set_btn_y, int set_btn_width, int set_btn_height)
        {
            btn_x = set_btn_x;
            btn_y = set_btn_y;
            btn_width = set_btn_width;
            btn_height = set_btn_height;

            button_bounds = { btn_x, btn_y, btn_width, btn_height};
        }

        void Draw_Btn()
        {
            // Render the button
            DrawRectangleRec(button_bounds, buttonColor);
            DrawText("Click me!", static_cast<int>(button_bounds.x) + 35, static_cast<int>(button_bounds.y) + 30, 20, textColor);
        }


        bool IsMouseInsideButton() {
            Vector2 mouse = GetMousePosition();
            return (mouse.x >= button_bounds.x && mouse.x <= (button_bounds.x + button_bounds.width) &&
                    mouse.y >= button_bounds.y && mouse.y <= (button_bounds.y + button_bounds.height));
        }
        
        bool Btn_pressed()
        {
            // Detect mouse input
            if (IsMouseInsideButton()) 
            {
                buttonColor = LIGHTGRAY;
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Action to perform when the button is clicked
                    // For now, let's just print a message
                    std::cout << "button pressed" << std::endl;
                    return true;
                }
            } 
            else 
            {
                buttonColor = GRAY;
                return false;
            }
            return false;
        }
        
};