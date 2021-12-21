#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct item
{
    int id;
    string name;
    int price;
    int copies = 0;
};

void init(vector<item> &stock);
void restock(vector<item> &stock);
void sell(vector<item> &stock);
void print(vector<item> &stock);
int main()
{
    vector<item> stock;
    freopen("Input.txt", "r", stdin);
    freopen("Output.txt", "w", stdout);
    init(stock);
    print(stock);
    string s;
    while (cin >> s && s != "end")
    {
        if (s == "restock")
            restock(stock);
        else if (s == "sell")
            sell(stock);
        else if (s == "stock" || s == "list")
            print(stock);
        else
            cout << "Please check your input\n";
    }
    return 0;
}
void init(vector<item> &stock)
{
    item English, Computer, Math;

    English.id = 123;
    English.name = "English";
    English.price = 12.5;
    English.copies = 2;

    Computer.id = 345;
    Computer.name = "Computer";
    Computer.price = 20.9;
    Computer.copies = 9;

    Math.id = 789;
    Math.name = "Math";
    Math.price = 8;
    Math.copies = 3;

    stock.push_back(English);
    stock.push_back(Computer);
    stock.push_back(Math);
}
void restock(vector<item> &stock)
{
    cout << "How many to re-stock?:";

    int n, cnt = 1;
    cin >> n;

    while (n--)
    {
        item temp;
        cout << "Item " << cnt << "\n";
        cout << "ID: ";
        cin >> temp.id;
        cout << "Name: ";
        cin >> temp.name;
        cout << "Price: ";
        cin >> temp.price;
        cout << "Copies: ";
        cin >> temp.copies;
        cnt++;
        for (item i : stock)
            if (temp.id == i.id && temp.name == i.name && temp.price == i.price)
                i.copies += temp.copies;
            else
                stock.push_back(temp);
    }
}
void print(vector<item> &stock)
{
    for (item i : stock)
        cout << "- " << i.id << ", " << i.name << ", $" << i.price << ", " << i.copies << "\n";
}
void sell(vector<item> &stock)
{
    cout << "How many to sell?: ";

    int n, cnt = 1;
    vector<item> bill;
    cin >> n;

    while (n--)
    {
        item temp;
        cout << "Item " << cnt << "\n";
        cout << "ID: ";
        cin >> temp.id;
        cout << "Copies: ";
        cin >> temp.copies;
        cnt++;
        bool found = false;
        for (item &i : stock)
            if (temp.id == i.id && temp.copies <= i.copies)
            {
                i.copies -= temp.copies;
                temp.name = i.name;
                temp.price = i.price;
                bill.push_back(temp);
                found = true;
            }
        if (!found)
            cout << "That item is unstocked\n";
    }

    print(bill);
    int sum = 0;
    for (item i : bill)
        sum += i.price * i.copies;
    cout << "Total bill: $" << sum << "\n";

    for (item i : stock)
        if (i.copies == 0)
            cout << i.id << ", " << i.name << " is out of stock!\n";
}