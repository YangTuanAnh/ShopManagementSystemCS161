#include "shop.h"

void Program::printlist(const int posX, const int posY, const int length, std::vector<item> &list)
{
    DrawRectangle(posX, posY, length, 30, Fade(LIGHTGRAY, 0.3f));
    DrawText("ID", posX + 10, posY + 10, 10, GRAY);
    DrawText("NAME", posX + length / 4 + 10, posY + 10, 10, GRAY);
    DrawText("PRICE", posX + length / 2 + 10, posY + 10, 10, GRAY);
    DrawText("COPIES", posX + length / 4 * 3 + 10, posY + 10, 10, GRAY);
    for (int i = 0; i < list.size(); i++)
    {
        if (i % 2 == 0)
            DrawRectangle(posX, posY + 30 * (i + 1), length, 30, Fade(LIGHTGRAY, 0.5f));
        else
            DrawRectangle(posX, posY + 30 * (i + 1), length, 30, Fade(LIGHTGRAY, 0.3f));

        DrawText(TextFormat("%i", list[i].id), posX + 10, posY + 10 + 30 * (i + 1), 10, GRAY);
        DrawText(TextFormat("%s", (list[i].name).c_str()), posX + length / 4 + 10, posY + 10 + 30 * (i + 1), 10, GRAY);
        DrawText(TextFormat("%lf", list[i].price), posX + length / 2 + 10, posY + 10 + 30 * (i + 1), 10, GRAY);
        DrawText(TextFormat("%i", list[i].copies), posX + length / 4 * 3 + 10, posY + 10 + 30 * (i + 1), 10, GRAY);

        if (GuiButton({posX + length - 20, posY + 10 + 30 * (i + 1), 10, 10}, "x"))
            list.erase(list.begin() + i);
    }
}

void Program::inputsave(std::fstream &save)
{
    save.open("..\\src\\savefile.dat", std::ios::in);
    if (save)
        while (!save.eof())
        {
            item temp;
            save >> temp.id >> temp.name >> temp.price >> temp.copies;
            stock.push_back(temp);
        }
    stock.pop_back();
    save.close();
}

void Program::exitsave(std::fstream &save)
{
    save.open("..\\src\\savefile.dat", std::ios::trunc | std::ios::out);
    for (item i : stock)
        save << i.id << " " << i.name << " " << i.price << " " << i.copies << '\n';
    save.close();
    CloseWindow();
}

std::string Program::dropdownlist()
{
    std::string list = "";
    for (item i : stock)
    {
        std::string num = "";
        for (int temp = i.id; temp; temp /= 10)
            num.insert(num.begin(), temp % 10 + '0');
        list += num + " - " + i.name + ";";
    }

    if (currentScreen == RESTOCK)
        list += "CUSTOM;";
    list.pop_back();
    return list;
}

void Program::updatebill(int index, int sellcnt)
{
    bool found = false;
    for (int i = 0; i < bill.size() && !found; i++)
        if (bill[i].id == stock[index].id)
        {
            bill[i].copies = sellcnt;
            found = true;
        }
    if (!found)
        bill.push_back({stock[index].id, stock[index].name, stock[index].price, sellcnt});

    billsum = 0;
    for (int i = 0; i < bill.size(); i++)
        billsum += bill[i].price * bill[i].copies;
}

void Program::updatestock()
{
    billsum = 0;
    for (int i = 0; i < bill.size(); i++)
        for (int j = 0; j < stock.size(); j++)
            if (bill[i].id == stock[j].id)
                stock[j].copies -= bill[i].copies;
    bill.clear();
}