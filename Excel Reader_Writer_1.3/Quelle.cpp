#include <istream>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ostream>
#include <algorithm>
#include <iterator>
#include <sstream>

// Is necessary to differentiate the fields in case they are seperated by ; or "" 
enum class CSVState { 
    UnquotedField,
    QuotedField,
    QuotedQuote
};

// Goeas through each char to determine where each field starts and ends
std::vector<std::string> readCSVRow(const std::string& row) { 
    CSVState state = CSVState::UnquotedField;
    std::vector<std::string> fields{ "" };
    size_t i = 0; // index of the current field
    for (char c : row) {
        switch (state) {
        case CSVState::UnquotedField:
            switch (c) {
            case ';': // end of field                          Active
                fields.push_back(""); i++;
                break;
            case '"': state = CSVState::QuotedField;
                break;
                
            default:  fields[i].push_back(c);               // Active
                break;
            }
            break;
        case CSVState::QuotedField:
            switch (c) {
            case '"': state = CSVState::QuotedQuote;
                break;
            default:  fields[i].push_back(c);              // Active
                break;
            }
            break;
        case CSVState::QuotedQuote:
            switch (c) {
            case ';': // , after closing quote
                fields.push_back(""); i++;
                state = CSVState::UnquotedField;
                
                break;
            case '"': // "" -> "
                fields[i].push_back('"');
                state = CSVState::QuotedField;
                
                break;
            default:  // end of quote
                state = CSVState::UnquotedField;
                
                break;
            }
            break;
        }
        
    }
    
    
    return fields;
}

/// Read CSV file, Excel dialect. Accept "quoted fields ""with quotes""" and fields seperated by ;
std::vector<std::vector<std::string>> readCSV(std::istream& in) {
    std::vector<std::vector<std::string>> table;
    std::string row;
    while (!in.eof()) {
        std::getline(in, row);
        
        if (in.bad() || in.fail()) {
            std::cout << "Couldn't read the file properly.\n";
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);     
    }
     return table;
}

int main() /*Version 1.3: 
           Program to compare two CSV-files. One with Assets according to DIN276 and one with the Assets of the analyzed building. 
           Important is that the assetlist has to be saved in CSV and that they are structured in a certain way (see the for loop below).
           Next steps are to create a file with the results and to let the foor loop start at catch words like "Anlage" and "Anzahl"
           After that the DIN276 list should be extended to hold more Assets referring to one asset of DIN276 so more words can be found to be assigned to
           an asset and the associated pricing*/
{
    std::ifstream din276("../../DIN276.csv");
    std::vector<std::vector<std::string>> din276_file = readCSV(din276);

    std::ifstream file("../../Fuerth.csv");
    std::vector<std::vector<std::string>> table = readCSV(file);

    std::vector<std::string>din;
    for (int i = 0; i < din276_file.size(); i++) {
        for (int j = 0; j < din276_file[i].size(); j++) {
            din.push_back(din276_file[i][j]);
        }
    }
    std::vector<std::string>anlage;
   for (int k = 0; k < table.size(); k++) {
        for (int l = 2; l < table[k].size() - 1; l++) { //Just chose the 3. column (later search algorithm for keywords included)
            anlage.push_back(table[k][l]);
        }
   }
   for (int i = 0; i < din.size(); ++i)
       for (int k = 0; k < anlage.size(); k++)
           if (din[i] == anlage[k])
               std::cout << din[i] << "\t" << table[k][3] << "\n";


}