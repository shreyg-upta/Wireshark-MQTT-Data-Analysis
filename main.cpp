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
    string outputFile = "output.csv";


    std::ifstream file("selected_pcapng.txt");
    std::string filename;
    
    if (file) {
        std::getline(file, filename);
        file.close();
    } else {
        std::cerr << "Error: Could not open selected_pcapng.txt!" << std::endl;
        return 1;
    }

    // Extract "nXms" (where X is a digit 1-9) using regex
    std::regex ms_regex(R"(([1-9])0ms)");
    std::smatch match;
    int ms_value = -1;  // Default: not found

    if (std::regex_search(filename, match, ms_regex)) {
        ms_value = std::stoi(match[1]);  // Convert extracted number to int
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
    map<int, int> no_of_retransmissions;
    map<int, int> no_of_transmissions2;
    map<int, double> first_occurence;
    map<int, vector<int> > ack_no;
    map<int, int> ack_no_expecting;
    string publisher ;
    string broker ;
    string subscriber ;
    if(is_laptop){
publisher = "192.168.146.187";
     broker = "192.168.146.97";
     subscriber = "192.168.146.112";
    }
    else{
         publisher = "192.168.146.187";
     broker = "192.168.146.208";
     subscriber = "192.168.146.97";
    }
    vector<int> temp(2);
    set<int> buffer;
    // cout << data[5][5];
    int itest1 = 0;
    for (int i = 1; i < data.size(); i++)
    {
        // cout << data[i][1] << " " << data[i][5] << endl;
        // cout << i << " \n"  ;
        if (strcmpp(data[i][1], publisher) && strcmpp(data[i][2], broker))
        {
            itest1++;

            // cout << "Publisher to Broker" <<itest1<< endl;
            if (data[i][8].size() > 12)
            {

                for (int j = 0; j <= data[i][8].size() - 12; j++)
                {
                    // if(itest1==10)
                    // cout << string(data[i][9].begin()+j,data[i][9].begin()+j+12) << endl;
                    if (j + 12 <= data[i][8].size() && strcmpp(string(data[i][8].begin() + j, data[i][8].begin() + j + 12), "\"\"sig no\"\": "))
                    {
                        string signal_no = "";
                        for (int k = j + 12; k < data[i][8].size(); k++)
                        {
                            if ('0' <= data[i][8][k] && data[i][8][k] <= '9')
                            {
                                signal_no += data[i][8][k];
                            }
                            else
                            {
                                break;
                            }
                        }
                        if(!strcmpp(signal_no,""))
                        {
                            
                        int signal_no_int = stoi(signal_no);
                        if (itest1 < 100)
                            cout << signal_no_int << " included by first column no. " << i << "\n";
                        if (first_occurence[signal_no_int] == 0)
                        {
                            first_occurence[signal_no_int] = stod(data[i][0]);
                            buffer.insert(signal_no_int);
                        }
                        else
                        {
                            // if(signal_no_int==11)
                            // {
                            //     cout << "signal no 0 included by first column no. " << i << "\n";
                            // }
                            no_of_retransmissions[signal_no_int]++;
                        }
                        }
                    }
                }
            }
            else{
            for (int j = 0; j < data[i][10].size(); j++)
            {
                if (strcmpp(string(data[i][10].begin() + j, data[i][10].begin() + j + 12), "\"\"sig no\"\": "))
                {
                    string signal_no = "";
                    for (int k = j + 12; k < data[i][10].size(); k++)
                    {
                        if ('0' <= data[i][10][k] && data[i][10][k] <= '9')
                        {
                            signal_no += data[i][10][k];
                        }
                        else
                        {
                            break;
                        }
                    }
                                            if(!strcmpp(signal_no,""))
{
                    int signal_no_int = stoi(signal_no);
                    // if (itest1 < 100)
                    //     cout << signal_no_int << " included by first1 column no. " << i << "\n";
                    if (first_occurence[signal_no_int] == 0)
                    {
                        first_occurence[signal_no_int] = stod(data[i][0]);
                        buffer.insert(signal_no_int);
                    }
                    else
                    {
                        // if(signal_no_int==11)
                        //     {
                        //         cout << "signal no 0 included by first column no. " << i << "\n";
                        //     }
                        no_of_retransmissions[signal_no_int]++;
                    }
}
                }
            }
            }
        }
        else if (strcmpp(data[i][1], broker) && strcmpp(data[i][2], subscriber))
        {
            // cout << "Broker to Subscriber" << endl;
            if (data[i][8].size() > 12)
            {
                for (int j = 0; j < data[i][8].size() - 12; j++)
                {
                    // cout << string(data[i][9].begin()+j,data[i][9].begin()+j+12) << endl;
                    if (strcmpp(string(data[i][8].begin() + j, data[i][8].begin() + j + 12), "\"\"sig no\"\": "))
                    {
                        string signal_no = "";
                        for (int k = j + 12; k < data[i][8].size(); k++)
                        {
                            if ('0' <= data[i][8][k] && data[i][8][k] <= '9')
                            {
                                signal_no += data[i][8][k];
                            }
                            else
                            {
                                break;
                            }
                        }
                                                if(!strcmpp(signal_no,""))
{
                        int signal_no_int = stoi(signal_no);
                        ack_no[stoi(data[i][6])].push_back(signal_no_int);
                        if(!ack_no_expecting[signal_no_int])
                        {
                            ack_no_expecting[signal_no_int]=stoi(data[i][6]);
                        }
                        if (itest1 < 100)
                            cout << signal_no_int << " included by second\n";
                        if (first_occurence[signal_no_int] == 0)
                        {
                            cout << signal_no_int << " gadbad ho gayi" << endl;
                            return 0;
                        }
                        else
                        {
                            // no_of_retransmissions[signal_no_int]++;
                            // cout << data[i][6] << endl;
                            temp[0] = stoi(data[i][6]);
                            temp[1] = signal_no_int;
                            // cout << temp[0] << " " << temp[1] << endl;
                            if (no_of_transmissions2[temp[1]] == 0)
                            {
                                no_of_transmissions2[temp[1]] = 1;
                            }
                            else
                            {
                                no_of_transmissions2[temp[1]]++;
                            }
                        }
}
                    }
                }
            }
            else{
            for (int j = 0; j < data[i][10].size(); j++)
            {
                if (strcmpp(string(data[i][10].begin() + j, data[i][10].begin() + j + 12), "\"\"sig no\"\": "))
                {
                    string signal_no = "";
                    for (int k = j + 12; k < data[i][10].size(); k++)
                    {
                        if ('0' <= data[i][10][k] && data[i][10][k] <= '9')
                        {
                            signal_no += data[i][10][k];
                        }
                        else
                        {
                            break;
                        }
                    }
                                            if(!strcmpp(signal_no,""))
{
                    int signal_no_int = stoi(signal_no);
                        ack_no[stoi(data[i][6])].push_back(signal_no_int);
                        if(!ack_no_expecting[signal_no_int])
                        {
                            ack_no_expecting[signal_no_int]=stoi(data[i][6]);
                        }
                        
                    if (itest1 < 100)
                        cout << signal_no_int << " included by second1\n";
                    if (first_occurence[signal_no_int] == 0)
                    {
                        cout << signal_no_int << "gadbad ho gayi" << endl;
                    }
                    else
                    {

                        no_of_transmissions2[signal_no_int]++;
                    }
}
                }
            }}
        }
        else if (strcmpp(data[i][1], broker) && strcmpp(data[i][2], publisher))
        {
            // cout << "Broker to Publisher" << endl;
            // eat 5 star, do nothing
        }
        else if (strcmpp(data[i][1], subscriber) && strcmpp(data[i][2], broker))
        {
            // cout << "Broker to Subscriber" << endl;
            int buffer_size = buffer.size();
            vector<int> elements_to_erase;
            for(int buffer_element:buffer)
            {
                cout << buffer_element << " ";
                int signal = buffer_element; 
                if(first_occurence[buffer_element] && ack_no_expecting[buffer_element]<=stoi(data[i][5]) && ack_no_expecting[buffer_element])
                {
                    // cout << "Hi";
                    if(first_occurence[signal]==-1){
                    //lite
                }
                else
                {
                   // output.push_back({to_string(first_occurence[signal]),to_string(stod(data[i][0])-first_occurence[signal]),to_string(signal)});
                    vector<string> temp;
                    temp.push_back(to_string(first_occurence[signal]));
                    // temp.push_back(to_string(signal));

                    temp.push_back(to_string(stod(data[i][0]) - first_occurence[signal]));
                    temp.push_back(to_string(no_of_retransmissions[signal]+no_of_transmissions2[signal]-1));//-1 aayega ki nahi
                    temp.push_back(to_string(buffer_size));
                    temp.push_back(to_string(signal));
                    temp.push_back(to_string(ack_no_expecting[signal]));
                    temp.push_back(data[i][5]);
                    data2.push_back(temp);
                    first_occurence[signal] = -1;
                    elements_to_erase.push_back(buffer_element);
                }
                }
                
            }
            for(int element : elements_to_erase)
            {
                buffer.erase(element);
            }

            cout << endl;




            // for(int signal:ack_no[stoi(data[i][5])])
            // {
            //     if (first_occurence[signal] == 0)
            //     {
            //         cout << signal << " gadbad ho gayi" << endl;
            //         return 0;
            //     }
            //     else if(first_occurence[signal]==-1){
            //         //lite
            //     }
            //     else
            //     {
            //        // output.push_back({to_string(first_occurence[signal]),to_string(stod(data[i][0])-first_occurence[signal]),to_string(signal)});
            //         vector<string> temp;
            //         temp.push_back(to_string(first_occurence[signal]));
            //         // temp.push_back(to_string(signal));

            //         temp.push_back(to_string(stod(data[i][0]) - first_occurence[signal]));
            //         temp.push_back(to_string(no_of_retransmissions[signal]+no_of_transmissions2[signal]-1));
            //         temp.push_back(to_string(buffer_size));
            //         data2.push_back(temp);
            //         first_occurence[signal] = -1;
            //         buffer.erase(signal);
            //     }
            // }
        }
        else
        {
            // cout << "redundant packet from" <<  data[i][1] << " to " << data[i][2] << endl;
        }
    }
    sort(data2.begin(), data2.end(), [](const vector<string> &a, const vector<string> &b) {
        return stod(a[0]) < stod(b[0]);
    });
    if (!data2.empty())
    {
        writeCSV(outputFile, data2);
    }

    return 0;
}
