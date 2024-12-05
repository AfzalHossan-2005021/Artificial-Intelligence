#include <bits/stdc++.h>

#define DATA_PATH "car evaluation dataset/car.data"
#define TRAIN_RATIO 0.8

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

// Define a pair for dataset sample: {features, label}
using Sample = pair<Car, Label>;

vector<Sample> readDataset()
{
    vector<Sample> dataset;
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

void stratifiedSplit(const vector<Sample> &dataset, double trainRatio,
                     vector<Sample> &trainSet, vector<Sample> &testSet)
{
    unordered_map<Label, vector<Sample *>> classGroups;

    // Group samples by their class labels
    for (const auto &sample : dataset)
    {
        classGroups[sample.second].push_back(const_cast<Sample *>(&sample));
    }

    random_device rd;
    mt19937 gen(rd());

    // Split each class group into train and test sets
    for (auto &[label, samples] : classGroups)
    {
        shuffle(samples.begin(), samples.end(), gen);

        int splitIndex = samples.size() * trainRatio;
        for (int i = 0; i < samples.size(); i++)
        {
            if (i < splitIndex)
                trainSet.push_back(*samples[i]);
            else
                testSet.push_back(*samples[i]);
        }
    }
}

int main()
{
    vector<Sample> dataset = readDataset();

    vector<Sample> trainingSet, testSet;
    stratifiedSplit(dataset, TRAIN_RATIO, trainingSet, testSet);

    cout << "Training set size: " << trainingSet.size() << endl;
    cout << "Test set size: " << testSet.size() << endl;

    return 0;
}