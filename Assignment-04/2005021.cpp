#include <bits/stdc++.h>

#define DATA_PATH "car evaluation dataset/car.data"

using namespace std;

enum class Attribute
{
    buying,
    maint,
    doors,
    persons,
    lugBoot,
    safety
};

enum class Buying
{
    vhigh,
    high,
    med,
    low
};

enum class Maint
{
    vhigh,
    high,
    med,
    low
};

enum class Doors
{
    two,
    three,
    four,
    more
};

enum class Persons
{
    two,
    four,
    more
};

enum class LugBoot
{
    small,
    med,
    big
};

enum class Safety
{
    low,
    med,
    high
};

enum class Label
{
    unacc,
    acc,
    good,
    vgood
};

struct Car
{
    Buying buying;
    Maint maint;
    Doors doors;
    Persons persons;
    LugBoot lugBoot;
    Safety safety;
};

vector<pair<Car, Label>> readDataset()
{
    vector<pair<Car, Label>> dataset;
    ifstream file(DATA_PATH);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return dataset;
    }
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string token;
        Car car;
        Label label;

        int i = 0;
        while (getline(ss, token, ','))
        {
            switch (i)
            {
            case 0:
                if (token == "vhigh")
                    car.buying = Buying::vhigh;
                else if (token == "high")
                    car.buying = Buying::high;
                else if (token == "med")
                    car.buying = Buying::med;
                else if (token == "low")
                    car.buying = Buying::low;
                break;
            case 1:
                if (token == "vhigh")
                    car.maint = Maint::vhigh;
                else if (token == "high")
                    car.maint = Maint::high;
                else if (token == "med")
                    car.maint = Maint::med;
                else if (token == "low")
                    car.maint = Maint::low;
                break;
            case 2:
                if (token == "2")
                    car.doors = Doors::two;
                else if (token == "3")
                    car.doors = Doors::three;
                else if (token == "4")
                    car.doors = Doors::four;
                else if (token == "5more")
                    car.doors = Doors::more;
                break;
            case 3:
                if (token == "2")
                    car.persons = Persons::two;
                else if (token == "4")
                    car.persons = Persons::four;
                else if (token == "more")
                    car.persons = Persons::more;
                break;
            case 4:
                if (token == "small")
                    car.lugBoot = LugBoot::small;
                else if (token == "med")
                    car.lugBoot = LugBoot::med;
                else if (token == "big")
                    car.lugBoot = LugBoot::big;
                break;
            case 5:
                if (token == "low")
                    car.safety = Safety::low;
                else if (token == "med")
                    car.safety = Safety::med;
                else if (token == "high")
                    car.safety = Safety::high;
                break;
            case 6:
                if (token == "unacc")
                    label = Label::unacc;
                else if (token == "acc")
                    label = Label::acc;
                else if (token == "good")
                    label = Label::good;
                else if (token == "vgood")
                    label = Label::vgood;
                break;
            }
            i++;
        }
        dataset.push_back({car, label});
    }
    return dataset;
}

int main()
{
    vector<pair<Car, Label>> dataset = readDataset();

    for(auto data : dataset)
    {
        cout << "Buying: " << (int)data.first.buying << endl;
        cout << "Maint: " << (int)data.first.maint << endl;
        cout << "Doors: " << (int)data.first.doors << endl;
        cout << "Persons: " << (int)data.first.persons << endl;
        cout << "LugBoot: " << (int)data.first.lugBoot << endl;
        cout << "Safety: " << (int)data.first.safety << endl;
        cout << "Label: " << (int)data.second << endl;
        cout << endl;
    }
    cout << "Dataset size: " << dataset.size() << endl;

    return 0;
}