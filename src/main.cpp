#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
const int WIDTH = 800, HEIGHT = 450;

typedef enum Screen
{
    LOGO = 0,
    LIST,
    RESTOCK,
    SELL
} Screen;
Screen currentScreen = LOGO;

struct item
{
    int id;
    std::string name;
    double price;
    int copies;
};
std::vector<item> stock, bill;
int framesCounter = 0;
bool dropdownedit, idedit, nameedit, priceedit, copiesedit;
int index, restockcnt, sellcnt;
double billsum = 0;
char idinput[101], nameinput[101], priceinput[101], copiesinput[101];

int main()
{
    InitWindow(WIDTH, HEIGHT, "Shop Managing System");

    SetTargetFPS(60);

    std::fstream save;
    save.open("..\\src\\savefile.dat", std::ios::in);
    if (save)
    {
        while (true)
        {
            item temp;
            save >> temp.id;
            if (temp.id)
                save >> temp.name >> temp.price >> temp.copies;
            else
                break;
            stock.push_back(temp);
        }
    }
    save.close();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        if (currentScreen != LOGO)
        {
            DrawRectangle(0, 0, 120, HEIGHT, LIGHTGRAY);
            DrawLine(120, 0, 120, HEIGHT, GRAY);
            if (GuiButton({10, 10, 100, 30}, "LIST"))
                currentScreen = LIST;
            if (GuiButton({10, 50, 100, 30}, "RESTOCK"))
                currentScreen = RESTOCK;
            if (GuiButton({10, 90, 100, 30}, "SELL"))
                currentScreen = SELL;
            if (GuiButton({10, 130, 100, 30}, "EXIT & SAVE"))
            {
                save.open("..\\src\\savefile.dat", std::ios::trunc | std::ios::out);
                for (item i : stock)
                    save << i.id << " " << i.name << " " << i.price << " " << i.copies << '\n';
                save << 0;
                save.close();
                CloseWindow();
            }
        }

        if (currentScreen == RESTOCK || currentScreen == SELL)
        {
            DrawRectangle(WIDTH / 2 + 60, 40, WIDTH - 120, 30, Fade(LIGHTGRAY, 0.3f));
            DrawText("ID", WIDTH / 2 + 70, 50, 10, GRAY);
            DrawText("NAME", WIDTH / 2 + 150, 50, 10, GRAY);
            DrawText("PRICE", WIDTH / 2 + 230, 50, 10, GRAY);
            DrawText("COPIES", WIDTH / 2 + 310, 50, 10, GRAY);
            for (int i = 0; i < stock.size(); i++)
            {
                if (i % 2 == 0)
                    DrawRectangle(WIDTH / 2 + 60, 70 + 30 * i, (WIDTH - 120) / 2, 30, Fade(LIGHTGRAY, 0.5f));
                else
                    DrawRectangle(WIDTH / 2 + 60, 70 + 30 * i, (WIDTH - 120) / 2, 30, Fade(LIGHTGRAY, 0.3f));

                DrawText(TextFormat("%i", stock[i].id), WIDTH / 2 + 70, 80 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%s", (stock[i].name).c_str()), WIDTH / 2 + 150, 80 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%lf", stock[i].price), WIDTH / 2 + 230, 80 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%i", stock[i].copies), WIDTH / 2 + 310, 80 + 30 * i, 10, GRAY);

                if (GuiButton({WIDTH - 20, 80 + 30 * i, 10, 10}, "x"))
                    stock.erase(stock.begin() + i);
            }
        }
        switch (currentScreen)
        {
        case LOGO:
        {
            framesCounter++;
            if (framesCounter > 120)
                currentScreen = LIST;
            DrawText("LOGO SCREEN", 20, 20, 40, LIGHTGRAY);
            DrawText("WAIT for 2 SECONDS...", 20, 70, 20, GRAY);
        }
        break;
        case LIST:
        {
            DrawRectangle(120, 0, WIDTH - 120, 40, WHITE);
            DrawText("LIST", 130, 10, 30, LIGHTGRAY);

            DrawRectangle(120, 40, WIDTH - 120, 30, Fade(LIGHTGRAY, 0.3f));
            DrawText("ID", 130, 50, 10, GRAY);
            DrawText("NAME", 330, 50, 10, GRAY);
            DrawText("PRICE", 530, 50, 10, GRAY);
            DrawText("COPIES", 730, 50, 10, GRAY);
            for (int i = 0; i < stock.size(); i++)
            {
                if (i % 2 == 0)
                    DrawRectangle(120, 70 + 30 * i, WIDTH - 120, 30, Fade(LIGHTGRAY, 0.5f));
                else
                    DrawRectangle(120, 70 + 30 * i, WIDTH - 120, 30, Fade(LIGHTGRAY, 0.3f));

                DrawText(TextFormat("%i", stock[i].id), 130, 80 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%s", (stock[i].name).c_str()), 330, 80 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%lf", stock[i].price), 530, 80 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%i", stock[i].copies), 730, 80 + 30 * i, 10, GRAY);

                if (GuiButton({WIDTH - 20, 80 + 30 * i, 10, 10}, "x"))
                    stock.erase(stock.begin() + i);
            }
        }
        break;
        case RESTOCK:
        {
            DrawRectangle(120, 0, WIDTH - 120, 40, WHITE);
            DrawText("RESTOCK", 130, 10, 30, LIGHTGRAY);
            DrawText("LIST", 10 + 60 + WIDTH / 2, 10, 30, LIGHTGRAY);
            DrawLine(60 + WIDTH / 2, 0, 60 + WIDTH / 2, HEIGHT, GRAY);

            if (index != stock.size())
            {
                GuiSpinner({WIDTH / 4 - 10, 90, 200, 30}, "Copies:", &restockcnt, 0, 9999, true);
                if (GuiButton({WIDTH / 4 - 10, 130, 200, 30}, "Restock"))
                    stock[index].copies += restockcnt;
            }
            else
            {
                DrawText("ID:", 130, 90, 10, GRAY);
                if (GuiTextBox({130, 110, (WIDTH - 120) / 2 - 20, 30}, idinput, 20, idedit))
                    idedit = !idedit;
                DrawText("Name:", 130, 150, 10, GRAY);
                if (GuiTextBox({130, 170, (WIDTH - 120) / 2 - 20, 30}, nameinput, 20, nameedit))
                    nameedit = !nameedit;
                DrawText("Price:", 130, 210, 10, GRAY);
                if (GuiTextBox({130, 230, (WIDTH - 120) / 2 - 20, 30}, priceinput, 20, priceedit))
                    priceedit = !priceedit;
                DrawText("Copies:", 130, 270, 10, GRAY);
                if (GuiTextBox({130, 290, (WIDTH - 120) / 2 - 20, 30}, copiesinput, 20, copiesedit))
                    copiesedit = !copiesedit;

                if (GuiButton({WIDTH / 4 - 10, 330, 200, 30}, "Stock"))
                {
                    item temp;
                    temp.id = std::stoi(idinput);
                    temp.name = nameinput;
                    temp.price = std::stod(priceinput);
                    temp.copies = std::stoi(copiesinput);
                    stock.push_back(temp);
                }
            }

            std::string list = "";
            for (int i = 0; i < stock.size(); i++)
            {
                std::stringstream ss;
                ss << stock[i].id;
                std::string num;
                ss >> num;
                list += num + " - " + stock[i].name + ";";
            }
            list += "CUSTOM";

            if (GuiDropdownBox({130, 50, (WIDTH - 120) / 2 - 20, 30}, TextFormat("%s", list.c_str()), &index, dropdownedit))
                dropdownedit = !dropdownedit;
        }
        break;
        case SELL:
        {
            DrawRectangle(120, 0, WIDTH - 120, 40, WHITE);
            DrawText("SELL", 130, 10, 30, LIGHTGRAY);
            DrawText("LIST", 10 + 60 + WIDTH / 2, 10, 30, LIGHTGRAY);
            DrawLine(60 + WIDTH / 2, 0, 60 + WIDTH / 2, HEIGHT, GRAY);

            GuiSpinner({WIDTH / 4 - 10, 90, 200, 30}, "Copies:", &sellcnt, 0, stock[index].copies, true);
            if (GuiButton({WIDTH / 4 - 10, 130, 200, 30}, "Add to bill"))
            {
                bool found = false;
                for (int i = 0; i < bill.size(); i++)
                    if (bill[i].id == stock[index].id)
                    {
                        bill[i].copies = sellcnt;
                        found = true;
                        break;
                    }
                if (!found)
                    bill.push_back({stock[index].id, stock[index].name, stock[index].price, sellcnt});
                billsum = 0;
                for (int i = 0; i < bill.size(); i++)
                    billsum += bill[i].price * bill[i].copies;
            }

            DrawRectangle(120, 170, (WIDTH - 120) / 2, 30, Fade(LIGHTGRAY, 0.3f));
            DrawText("ID", 130, 180, 10, GRAY);
            DrawText("NAME", 210, 180, 10, GRAY);
            DrawText("PRICE", 290, 180, 10, GRAY);
            DrawText("COPIES", 370, 180, 10, GRAY);
            for (int i = 0; i < bill.size(); i++)
            {
                if (i % 2 == 0)
                    DrawRectangle(120, 200 + 30 * i, (WIDTH - 120) / 2, 30, Fade(LIGHTGRAY, 0.5f));
                else
                    DrawRectangle(120, 200 + 30 * i, (WIDTH - 120) / 2, 30, Fade(LIGHTGRAY, 0.3f));

                DrawText(TextFormat("%i", bill[i].id), 130, 210 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%s", (bill[i].name).c_str()), 210, 210 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%lf", bill[i].price), 290, 210 + 30 * i, 10, GRAY);
                DrawText(TextFormat("%i", bill[i].copies), 370, 210 + 30 * i, 10, GRAY);

                if (GuiButton({(WIDTH / 2) + 40, 210 + 30 * i, 10, 10}, "x"))
                    bill.erase(bill.begin() + i);
            }
            DrawText(TextFormat("Total: %lf", billsum), 130, 210 + 30 * bill.size(), 10, GRAY);

            if (GuiButton({WIDTH / 4 - 10, 250 + 30 * bill.size(), 200, 30}, "Sell"))
            {
                billsum = 0;
                for (int i = 0; i < bill.size(); i++)
                    for (int j = 0; j < stock.size(); j++)
                        if (bill[i].id == stock[j].id)
                            stock[j].copies -= bill[i].copies;
                bill.clear();
            }

            std::string list = "";
            for (int i = 0; i < stock.size(); i++)
            {
                std::stringstream ss;
                ss << stock[i].id;
                std::string num;
                ss >> num;
                list += num + " - " + stock[i].name + ";";
            }
            list.pop_back();

            if (GuiDropdownBox({130, 50, (WIDTH - 120) / 2 - 20, 30}, TextFormat("%s", list.c_str()), &index, dropdownedit))
                dropdownedit = !dropdownedit;
        }
        break;
        default:
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}