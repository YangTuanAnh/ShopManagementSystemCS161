#include "shop.h"
#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int index, cnt;
char idinput[101], nameinput[101], priceinput[101], copiesinput[101];
bool dropdownedit, idedit, nameedit, priceedit, copiesedit;

int main()
{
    InitWindow(WIDTH, HEIGHT, "Shop Managing System");
    SetTargetFPS(60);

    Program currentProgram;

    std::fstream save;
    currentProgram.inputsave(save);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawRectangle(0, 0, 120, HEIGHT, LIGHTGRAY);
        DrawRectangle(120, 0, WIDTH - 120, 40, WHITE);
        DrawLine(120, 0, 120, HEIGHT, GRAY);
        if (GuiButton({10, 10, 100, 30}, "LIST"))
            currentProgram.currentScreen = LIST;
        if (GuiButton({10, 50, 100, 30}, "RESTOCK"))
            currentProgram.currentScreen = RESTOCK;
        if (GuiButton({10, 90, 100, 30}, "SELL"))
            currentProgram.currentScreen = SELL;
        if (GuiButton({10, 130, 100, 30}, "EXIT & SAVE"))
            currentProgram.exitsave(save);

        int listwidth = (currentProgram.currentScreen == LIST ? WIDTH - 120 : WIDTH / 2 - 60);
        DrawLine(WIDTH - listwidth, 0, WIDTH - listwidth, HEIGHT, GRAY);
        DrawText("LIST", WIDTH - listwidth + 10, 10, 30, LIGHTGRAY);

        currentProgram.printlist(WIDTH - listwidth, 40, listwidth, currentProgram.stock);

        switch (currentProgram.currentScreen)
        {
        case RESTOCK:
        {
            DrawText("RESTOCK", 130, 10, 30, LIGHTGRAY);
            if (index != currentProgram.stock.size())
            {
                GuiSpinner({WIDTH / 4 - 10, 90, 200, 30}, "Copies:", &cnt, 0, 9999, true);
                if (GuiButton({WIDTH / 4 - 10, 130, 200, 30}, "Restock"))
                    currentProgram.stock[index].copies += cnt;
            }
            else
            {
                DrawText("ID:", 130, 90, 10, GRAY);
                if (GuiTextBox({130, 110, (WIDTH - 120) / 2 - 20, 30}, idinput, 20, idedit))
                    idedit ^= 1;
                DrawText("Name:", 130, 150, 10, GRAY);
                if (GuiTextBox({130, 170, (WIDTH - 120) / 2 - 20, 30}, nameinput, 20, nameedit))
                    nameedit ^= 1;
                DrawText("Price:", 130, 210, 10, GRAY);
                if (GuiTextBox({130, 230, (WIDTH - 120) / 2 - 20, 30}, priceinput, 20, priceedit))
                    priceedit ^= 1;
                DrawText("Copies:", 130, 270, 10, GRAY);
                if (GuiTextBox({130, 290, (WIDTH - 120) / 2 - 20, 30}, copiesinput, 20, copiesedit))
                    copiesedit ^= 1;

                if (GuiButton({WIDTH / 4 - 10, 330, 200, 30}, "Stock"))
                    currentProgram.stock.push_back({std::stoi(idinput), nameinput, std::stod(priceinput), std::stoi(copiesinput)});
            }
        }
        break;
        case SELL:
        {
            DrawText("SELL", 130, 10, 30, LIGHTGRAY);
            GuiSpinner({WIDTH / 4 - 10, 90, 200, 30}, "Copies:", &cnt, 0, currentProgram.stock[index].copies, true);
            if (GuiButton({WIDTH / 4 - 10, 130, 200, 30}, "Add to bill"))
                currentProgram.updatebill(index, cnt);

            currentProgram.printlist(120, 170, listwidth, currentProgram.bill);

            DrawText(TextFormat("Total: %lf", currentProgram.billsum), 130, 210 + 30 * currentProgram.bill.size(), 10, GRAY);

            if (GuiButton({WIDTH / 4 - 10, 240 + 30 * currentProgram.bill.size(), 200, 30}, "Sell"))
                currentProgram.updatestock();
        }
        break;
        }

        if (currentProgram.currentScreen != LIST)
            if (GuiDropdownBox({130, 50, (WIDTH - 120) / 2 - 20, 30}, TextFormat("%s", currentProgram.dropdownlist().c_str()), &index, dropdownedit))
                dropdownedit ^= 1;

        EndDrawing();
    }
    CloseWindow();
    return 0;
}