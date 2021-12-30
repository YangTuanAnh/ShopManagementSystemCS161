#pragma once

#include <raylib.h>
#include "raygui.h"
#include <fstream>
#include <vector>
#include <cstring>

const int WIDTH = 800, HEIGHT = 450;

struct item
{
    int id;
    std::string name;
    double price;
    int copies;
};

typedef enum Screen
{
    LIST = 0,
    RESTOCK,
    SELL
} Screen;

struct Program
{
    Screen currentScreen;
    std::vector<item> stock, bill;
    double billsum;
    Program() : currentScreen(LIST), billsum(0) {}

    void printlist(const int posX, const int posY, const int length, std::vector<item> &list);
    void inputsave(std::fstream &save);
    void exitsave(std::fstream &save);
    std::string dropdownlist();
    void updatebill(int index, int sellcnt);
    void updatestock();
};
