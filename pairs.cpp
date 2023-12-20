#include <iostream>
#include <vector>
#include <utility>
#include <cstdio>
#include <cstring>
#include <filesystem>

class DataProcessor {
public:
    DataProcessor() = default;
    ~DataProcessor() = default;

    // creation of processDir function that handle all processing steps
    void processDir(const char* dir_path, const char* target_curr, long long volume) {
        
        // search for csv files in directory
        std::vector<char*> csvFiles = parseCsv(dir_path);
        // read data from csv files 
        std::vector<Data> allData = readData(csvFiles, target_curr);
        // multiply data taking into account volume parameter 
        Data data_k = multiplyData(volume, allData);
        // save data inside ./out.csv file
        writeData("./out.csv", data_k);

        // clear space 
        for (char* fileName : csvFiles) {
            delete[] fileName;
        }
    }
private:
    // structure for storing data points long for timestamp and double for price values
    using Data = std::vector<std::pair<long, double>>;
    
    std::vector<Data> readData(std::vector<char*>& paths, const char* target) {
        /*
        readData:
            input  |- paths  :  vector containing paths to csv files in specified previously folder
                   |- target :  currency pair wich price should be reversed (ex. EUR/CHF => CHF/EUR)
            ----------------------------------------------------------------------------------------
            output |- allData:  vector<Data> that contains data from each csv file
        */  
        std::vector<Data> allData;

        for (const char* path : paths) {
            Data data;
            long t;
            long double p;
            bool reverse = compareTargetCurr(path, target);
            if (FILE* fp = fopen(path, "r")) {
                while (fscanf(fp, "%ld;%llf", &t, &p) == 2) {
                    data.push_back(std::make_pair(t, (reverse ? 1 / p : p)));
                }
                fclose(fp);
            } else {
                perror("Error opening file\n");
            }
            allData.push_back(data);
        }
        return allData;
    }

    bool fileExists(const char* filename) {
        /*
        helper function to check weither file with filename exisits.
        */
        FILE* file;
        if (file = fopen(filename, "r")) {
            fclose(file);
            return true;
        }
        return false;
    }

    void writeData(const char* out_path, const Data& data) {
        /*
        writeData:
            input |- out_path :  name of saved file (./out.csv)
                  |- data     :  data that should be saved
        */
        if (fileExists(out_path)) {
            std::cout << "File 'out.csv' already exists. Do you want to overwrite? (Y/N): ";
            char response;
            std::cin >> response;

            if (response != 'Y' && response != 'y') {
                std::cout << "Operation cancelled. File not overwritten." << std::endl;
                return;
            }
        }

        if (FILE* fp = fopen(out_path, "w+")) {
            for (const auto& pair : data) {
                fprintf(fp, "%ld;%llf\n", pair.first, pair.second);
            }
            fclose(fp);
            std::cout << "Data written to 'out.csv' successfully." << std::endl;
        } else {
            perror("Error opening file");
        }
    }
    Data multiplyData(long long volume, const std::vector<Data>& dataVec) {
        /*
        multiplyData:
            input   |- volume  :  volume to be multiplied on prices values
                    |- dataVec :  currency pair wich price should be reversed (ex. EUR/CHF => CHF/EUR)
            --------------------------------------------------------------------------------------------------
            output  |- data_k  :  Data data_k that contains prices multiplication between all files and volume
        */
        if (dataVec.empty()) {
            std::cerr << "No data\n";
            return Data();
        }

        size_t dataSize = dataVec[0].size();
        for (const Data& data : dataVec) {
            if (data.size() != dataSize) {
                std::cerr << "Data has unequal size\n";
                return Data();
            }
        }

        Data data_k;
        data_k.reserve(dataSize);
        for (size_t i = 0; i < dataSize; i++) {
            long double p = 1.0;
            for (const Data& data : dataVec) {
                p *= data[i].second;
            }
            data_k.push_back(std::make_pair(dataVec[0][i].first, volume*p));
        }

        return data_k;
    }

    std::vector<char*> parseCsv(const char* path) {

        /*
        parseCsv:
            input   |- path      :  path to directory with csv files
            --------------------------------------------------------
            output  |- csvFiles  :  vector with csv filenames
        */

        std::vector<char*> csvFiles;
        namespace fs = std::filesystem;
        try {
            
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                    if (entry.path().filename() == "out.csv")
                        continue;
                    char* fileName = new char[entry.path().filename().string().size() + 1];
                    std::strcpy(fileName, entry.path().filename().string().c_str());
                    csvFiles.push_back(fileName);
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error accessing directory: " << e.what() << "\n";
        
            for (char* fileName : csvFiles) {
                delete[] fileName;
            }
            csvFiles.clear();
        }

        return csvFiles;
    }

    bool compareTargetCurr(const char* path, const char* target) {

        /*
        parseCsv:
            input   |- path      :  path to directory with csv files
                    |- target    :  target currency pairs to be founded in raw .csv filename for further reversing
            ------------------------------------------------------------------------------------------------------
            output  |- bool      :  true if raw .csv filename contains target currency pair, false - otherwise
        */

        const char* ext_s = strrchr(path, '.');
        if (ext_s == nullptr) {
            return false;
        }

        size_t fname_l = ext_s - path;
        size_t target_l = strlen(target);

        if (target_l != fname_l) {
            return false;
    }

    return strncmp(path, target, target_l) == 0;
}

};

int main(int argc, char* argv[]) {

    // specify usage
    if (argc < 4){
        std::cerr<<"\nUsage: \n"<<argv[0]<< " <directory_path> <target_currency_pair> <volume>\n\n";
        return 1;
    }

    // get params from command line
    const char* dir_path = argv[1];
    const char* target_curr = argv[2];
    long long volume = std::atoll(argv[3]);

    // init
    DataProcessor proc;
    // process
    proc.processDir(dir_path, target_curr, volume);

    return 0;
}
