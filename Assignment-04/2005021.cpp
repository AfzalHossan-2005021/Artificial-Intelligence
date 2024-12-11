#include <bits/stdc++.h>

#define DATA_PATH "car evaluation dataset/car.data"
#define TRAIN_RATIO 0.8
#define EPOCHS 20
#define RANDOM_RANGE 3

using namespace std;

enum class HEURISTIC
{
    ENTROPY,
    GINI,
    COUNT // Number of heuristics
};

enum class SPLIT
{
    BEST,
    RANDOM,
    COUNT // Number of splits
};

enum class Attribute
{
    buying,
    maint,
    doors,
    persons,
    lugBoot,
    safety,
    COUNT // Number of attributes
};

enum class Buying
{
    vhigh,
    high,
    med,
    low,
    COUNT // Number of buying options
};

enum class Maint
{
    vhigh,
    high,
    med,
    low,
    COUNT // Number of maint options
};

enum class Doors
{
    two,
    three,
    four,
    more,
    COUNT // Number of doors options
};

enum class Persons
{
    two,
    four,
    more,
    COUNT // Number of persons options
};

enum class LugBoot
{
    small,
    med,
    big,
    COUNT // Number of lug boot options
};

enum class Safety
{
    low,
    med,
    high,
    COUNT // Number of safety options
};

enum class Label
{
    unacc,
    acc,
    good,
    vgood,
    COUNT // Number of labels
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

// Decision tree node
class Node
{
public:
    bool isLeaf;

    Attribute attribute;
    Label label;

    map<int, Node *> children;

    Node(Attribute attribute) : isLeaf(false), attribute(attribute) {}
    Node(Label label) : isLeaf(true), label(label) {}

    void addChild(int value, Node *child)
    {
        children[value] = child;
    }
};

Label getPluralityValue(const vector<Sample> &examples)
{
    unordered_map<Label, int> labelCounts;
    for (const auto &example : examples)
    {
        labelCounts[example.second]++;
    }

    int maxCount = 0;
    Label pluralityValue;
    for (const auto &[label, count] : labelCounts)
    {
        if (count > maxCount)
        {
            maxCount = count;
            pluralityValue = label;
        }
    }

    return pluralityValue;
}

Label getClassification(const vector<Sample> &examples)
{
    return examples[0].second;
}

bool allExamplesHaveSameLabel(const vector<Sample> &examples)
{
    Label label = examples[0].second;
    for (const auto &example : examples)
    {
        if (example.second != label)
            return false;
    }
    return true;
}

vector<Sample> getExamplesWithAttributeValue(const vector<Sample> &examples, Attribute attribute, int value)
{
    vector<Sample> examples_v;
    for (const auto &example : examples)
    {
        switch (attribute)
        {
        case Attribute::buying:
            if (example.first.buying == static_cast<Buying>(value))
                examples_v.push_back(example);
            break;
        case Attribute::maint:
            if (example.first.maint == static_cast<Maint>(value))
                examples_v.push_back(example);
            break;
        case Attribute::doors:
            if (example.first.doors == static_cast<Doors>(value))
                examples_v.push_back(example);
            break;
        case Attribute::persons:
            if (example.first.persons == static_cast<Persons>(value))
                examples_v.push_back(example);
            break;
        case Attribute::lugBoot:
            if (example.first.lugBoot == static_cast<LugBoot>(value))
                examples_v.push_back(example);
            break;
        case Attribute::safety:
            if (example.first.safety == static_cast<Safety>(value))
                examples_v.push_back(example);
            break;
        }
    }
    return examples_v;
}

vector<int> getAttributeValues(Attribute attribute)
{
    vector<int> values;
    switch (attribute)
    {
    case Attribute::buying:
        for (int i = 0; i < static_cast<int>(Buying::COUNT); i++)
            values.push_back(i);
        break;
    case Attribute::maint:
        for (int i = 0; i < static_cast<int>(Maint::COUNT); i++)
            values.push_back(i);
        break;
    case Attribute::doors:
        for (int i = 0; i < static_cast<int>(Doors::COUNT); i++)
            values.push_back(i);
        break;
    case Attribute::persons:
        for (int i = 0; i < static_cast<int>(Persons::COUNT); i++)
            values.push_back(i);
        break;
    case Attribute::lugBoot:
        for (int i = 0; i < static_cast<int>(LugBoot::COUNT); i++)
            values.push_back(i);
        break;
    case Attribute::safety:
        for (int i = 0; i < static_cast<int>(Safety::COUNT); i++)
            values.push_back(i);
        break;
    }
    return values;
}

double getEntropy(const vector<Sample> &examples)
{
    double entropy = 0;
    unordered_map<Label, int> labelCounts;
    for (const auto &example : examples)
    {
        labelCounts[example.second]++;
    }
    for (const auto &[label, count] : labelCounts)
    {
        double p = static_cast<double>(count) / examples.size();
        entropy -= p * log2(p);
    }
    return entropy;
}

double getGiniImpurity(const vector<Sample> &examples)
{
    double impurity = 1;
    unordered_map<Label, int> labelCounts;
    for (const auto &example : examples)
    {
        labelCounts[example.second]++;
    }
    for (const auto &[label, count] : labelCounts)
    {
        double p = static_cast<double>(count) / examples.size();
        impurity -= p * p;
    }
    return impurity;
}

double getInformationGain(const vector<Sample> &examples, Attribute attribute, double parentEntropy)
{
    double gain = 0;
    for (const auto &value : getAttributeValues(attribute))
    {
        vector<Sample> examples_v = getExamplesWithAttributeValue(examples, attribute, value);
        double p = static_cast<double>(examples_v.size()) / examples.size();
        gain += p * getEntropy(examples_v);
    }
    return parentEntropy - gain;
}

double getGiniGain(const vector<Sample> &examples, Attribute attribute, double parentImpurity)
{
    double gain = 0;
    for (const auto &value : getAttributeValues(attribute))
    {
        vector<Sample> examples_v = getExamplesWithAttributeValue(examples, attribute, value);
        double p = static_cast<double>(examples_v.size()) / examples.size();
        gain += p * getGiniImpurity(examples_v);
    }
    return parentImpurity - gain;
}

Attribute chooseBestAttribute(const vector<Sample> &examples, const set<Attribute> &attributes, SPLIT split, HEURISTIC heuristic)
{
    double parentEntropy = getEntropy(examples);
    double parentImpurity = getGiniImpurity(examples);

    vector<pair<double, Attribute>> gains;
    for (const auto &attribute : attributes)
    {
        double gain;
        if (heuristic == HEURISTIC::ENTROPY)
            gain = getInformationGain(examples, attribute, parentEntropy);
        else if (heuristic == HEURISTIC::GINI)
            gain = getGiniGain(examples, attribute, parentImpurity);
        gains.push_back({gain, attribute});
    }
    // sort by gain in descending order
    sort(gains.begin(), gains.end(), greater<pair<double, Attribute>>());

    if (split == SPLIT::BEST)
        return gains[0].second;
    else // if (split == SPLIT::RANDOM)
    {
        int randomIndex = rand() % RANDOM_RANGE;
        if (randomIndex >= gains.size())
            return gains[0].second;
        else
            return gains[randomIndex].second;
    }
}

Node *decisionTreeLearn(const vector<Sample> &examples, const set<Attribute> &attributes, const vector<Sample> &parent_eramples, SPLIT split, HEURISTIC heuristic)
{
    if (examples.empty())
    {
        Label Label = getPluralityValue(parent_eramples);
        return new Node(Label);
    }
    else if (allExamplesHaveSameLabel(examples))
    {
        Label label = getClassification(examples);
        return new Node(label);
    }
    else if (attributes.empty())
    {
        Label label = getPluralityValue(examples);
        return new Node(label);
    }
    else
    {
        Attribute bestAttribute = chooseBestAttribute(examples, attributes, split, heuristic);
        Node *node = new Node(bestAttribute);
        set<Attribute> remainingAttributes = attributes;
        remainingAttributes.erase(bestAttribute);
        for (const auto &value : getAttributeValues(bestAttribute))
        {
            vector<Sample> examples_v = getExamplesWithAttributeValue(examples, bestAttribute, value);
            Node *child = decisionTreeLearn(examples_v, remainingAttributes, examples, split, heuristic);
            node->addChild(value, child);
        }
        return node;
    }
}

Label classify(Node *node, const Car &car)
{
    if (node->isLeaf)
        return node->label;

    int value;
    switch (node->attribute)
    {
    case Attribute::buying:
        value = static_cast<int>(car.buying);
        break;
    case Attribute::maint:
        value = static_cast<int>(car.maint);
        break;
    case Attribute::doors:
        value = static_cast<int>(car.doors);
        break;
    case Attribute::persons:
        value = static_cast<int>(car.persons);
        break;
    case Attribute::lugBoot:
        value = static_cast<int>(car.lugBoot);
        break;
    case Attribute::safety:
        value = static_cast<int>(car.safety);
        break;
    }

    return classify(node->children[value], car);
}

double testDecisionTree(Node *node, const vector<Sample> &testSet)
{
    vector<int> sample_label_count(4, 0), predicted_label_count(4, 0);
    for (const auto &sample : testSet)
    {
        sample_label_count[static_cast<int>(sample.second)]++;
    }

    int correct = 0;
    for (const auto &sample : testSet)
    {
        Label predicted = classify(node, sample.first);
        if (predicted == sample.second)
            correct++;
        predicted_label_count[static_cast<int>(predicted)]++;
    }
    return static_cast<double>(correct) / testSet.size();
}

void printMinus(int n, int newLine)
{
    for (int i = 0; i < n; i++)
        cout << "-";
    if (newLine)
        cout << endl;
}

void printSpaces(int n, int newLine)
{
    for (int i = 0; i < n; i++)
        cout << " ";
    if (newLine)
        cout << endl;
}

void printReportTable(const vector<vector<double>> &accuracy)
{
    cout << setprecision(2) << fixed << endl;
    cout << "+";
    printMinus(52, 0);
    cout << "+";
    printMinus(52, 0);
    cout << "+" << endl << "|";
    printSpaces(52, 0);
    cout << "|";
    printSpaces(11, 0);
    cout << "Average accuracy over 20 runs";
    printSpaces(12, 0);
    cout << "|" << endl << "|";
    printSpaces(12, 0);
    cout << "Attribute selection strategy";
    printSpaces(12, 0);
    cout << "+";
    printMinus(27, 0);
    cout << "+";
    printMinus(24, 0);
    cout << "+" << endl << "|";
    printSpaces(52, 0);
    cout << "|";
    printSpaces(6, 0);
    cout << "Information gain";
    printSpaces(5, 0);
    cout << "|";
    printSpaces(6, 0);
    cout << "Gini impurity";
    printSpaces(5, 0);
    cout << "|" << endl;
    cout << "+";
    printMinus(52, 0);
    cout << "+";
    printMinus(27, 0);
    cout << "+";
    printMinus(24, 0);
    cout << "+" << endl << "|";
    printSpaces(10, 0);
    cout <<"Always select the best attribute";
    printSpaces(10, 0);
    cout << "|";
    printSpaces(10, 0);
    cout << " " << accuracy[0][0] << " %";
    printSpaces(9, 0);
    cout << "|";
    printSpaces(8, 0);
    cout << " " << accuracy[0][1] << " %";
    printSpaces(8, 0);
    cout << "|" << endl;
    cout << "+";
    printMinus(52, 0);
    cout << "+";
    printMinus(27, 0);
    cout << "+";
    printMinus(24, 0);
    cout << "+" << endl;
    cout << "| Select one randomly from the top three attributes  |";
    printSpaces(10, 0);
    cout << " " << accuracy[1][0] << " %";
    printSpaces(9, 0);
    cout << "|";
    printSpaces(8, 0);
    cout << " " << accuracy[1][1] << " %";
    printSpaces(8, 0);
    cout << "|" << endl;
    cout << "+";
    printMinus(52, 0);
    cout << "+";
    printMinus(27, 0);
    cout << "+";
    printMinus(24, 0);
    cout << "+" << endl << endl;
}

int main()
{
    vector<Sample> dataset = readDataset();
    set<Attribute> attributes;
    for (int i = 0; i < static_cast<int>(Attribute::COUNT); i++)
    {
        attributes.insert(static_cast<Attribute>(i));
    }

    vector<vector<double>> accuracy(2, vector<double>(2, 0));

    for (int i = 0; i < EPOCHS; i++)
    {
        vector<Sample> trainSet, testSet;
        stratifiedSplit(dataset, TRAIN_RATIO, trainSet, testSet);

        for (int j = 0; j < static_cast<int>(SPLIT::COUNT); j++)
        {
            for (int k = 0; k < static_cast<int>(HEURISTIC::COUNT); k++)
            {
                Node *decisionTree = decisionTreeLearn(trainSet, attributes, trainSet, static_cast<SPLIT>(j), static_cast<HEURISTIC>(k));
                accuracy[j][k] += testDecisionTree(decisionTree, testSet);
                delete decisionTree;
            }
        }
    }

    for (int j = 0; j < static_cast<int>(SPLIT::COUNT); j++)
    {
        for (int k = 0; k < static_cast<int>(HEURISTIC::COUNT); k++)
        {
            accuracy[j][k] = (accuracy[j][k] * 100) / EPOCHS;
        }
    }

    printReportTable(accuracy);

    return 0;
}