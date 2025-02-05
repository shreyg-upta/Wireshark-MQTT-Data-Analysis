#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <string>
#include <bits/stdc++.h>
using namespace std;

bool strcmpp(string a, string b)
{
    if (a.size() != b.size())
    {
        // cout << a << " " << b << endl;
        return false;
    }

    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            // cout << a[i] << " " << b[i] << endl;
            return false;
        }
    }
    return true;
}
string conv(string str)
{
    return string(str.begin() + 1, str.end() - 1);
}

// Function to read CSV file and store data in a vector of vectors
vector<vector<string> > readCSV(const string &filename)
{
    vector<vector<string> > data;
    ifstream file(filename);
    string line, word;

    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return data;
    }

    while (getline(file, line))
    {
        vector<string> row;
        stringstream ss(line);
        while (getline(ss, word, '|'))
        {
            row.push_back(word);
        }
        data.push_back(row);
    }

    file.close();
    return data;
}

// Function to write data to a CSV file
void writeCSV(const string &filename, const vector<vector<string> > &data)
{
    ofstream file(filename);

    if (!file.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing" << endl;
        return;
    }

    for (const auto &row : data)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            file << row[i];
            if (i < row.size() - 1)
            {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
    cout << "Data successfully written to " << filename << endl;
}

int main()
{
    string inputFile = "Data_after_conversion.csv";
    string outputFile = "output2.csv";

    std::ifstream file("selected_pcapng.txt");
    std::string filename;

    if (file)
    {
        std::getline(file, filename);
        file.close();
    }
    else
    {
        std::cerr << "Error: Could not open selected_pcapng.txt!" << std::endl;
        return 1;
    }

    // Extract "nXms" (where X is a digit 1-9) using regex
    std::regex ms_regex(R"(([1-9])0ms)");
    std::smatch match;
    int ms_value = -1; // Default: not found

    if (std::regex_search(filename, match, ms_regex))
    {
        ms_value = std::stoi(match[1]); // Convert extracted number to int
    }

    // Check if filename contains "laptop" (case insensitive)
    bool is_laptop = filename.find("laptop") != std::string::npos ||
                     filename.find("LAPTOP") != std::string::npos;

    // Print results
    std::cout << "File: " << filename << std::endl;
    std::cout << "Milliseconds: " << (ms_value != -1 ? std::to_string(ms_value) : "Not Found") << std::endl;
    std::cout << "Is Laptop: " << (is_laptop ? "True" : "False") << std::endl;

    vector<vector<string> > data = readCSV(inputFile);
    vector<vector<string> > data2;

    string publisher;
    string broker;
    string subscriber;
    if (is_laptop)
    {
        publisher = "192.168.146.187";
        broker = "192.168.146.97";
        subscriber = "192.168.146.112";
    }
    else
    {
        publisher = "192.168.146.187";
        broker = "192.168.146.208";
        subscriber = "192.168.146.97";
    }
    for (int i = 1; i < data.size(); i++)
    {
        // cout << data[i][1] << " " << data[i][5] << endl;
        // cout << i << " \n"  ;
        if (strcmpp(data[i][1], publisher) && strcmpp(data[i][2], broker) || strcmpp(data[i][1], broker) && strcmpp(data[i][2], subscriber)){
            vector<string> temp;
            temp.push_back(data[i][0]);
            temp.push_back(to_string(data[i][8].size()/76));
            data2.push_back(temp);
        }
    }
    if (!data2.empty())
    {
        writeCSV(outputFile, data2);
    }

    return 0;
}
