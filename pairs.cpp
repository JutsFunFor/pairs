#include <iostream>
#include <vector>
#include <utility>
#include <cstdio>
#include <cstring>
#include <filesystem>

class DataProcessor {
public:
    using Data = std::vector<std::pair<long, float>>;

    DataProcessor() = default;
    ~DataProcessor() = default;

    std::vector<Data> readData(std::vector<char*>& paths, const char* target) {
        std::vector<Data> allData;

        for (const char* path : paths) {
            Data data;
            long t;
            float p;
            bool reverse = compareTargetCurr(path, target);
            if (FILE* fp = fopen(path, "r")) {
                while (fscanf(fp, "%ld;%f", &t, &p) == 2) {
                    data.push_back(std::make_pair(t, (reverse ? 1.0f / p : p)));
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
        FILE* file;
        if (file = fopen(filename, "r")) {
            fclose(file);
            return true;
        }
        return false;
    }

    void writeData(const char* out_path, const Data& data) {
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
                fprintf(fp, "%ld;%f\n", pair.first, pair.second);
            }
            fclose(fp);
            std::cout << "Data written to 'out.csv' successfully." << std::endl;
        } else {
            perror("Error opening file");
        }
    }
    Data multiplyData(int k, const std::vector<Data>& dataVec) {
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
            float p = 1.0f;
            for (const Data& data : dataVec) {
                p *= k * data[i].second;
            }
            data_k.push_back(std::make_pair(dataVec[0][i].first, p));
        }

        return data_k;
    }

    std::vector<char*> parseCsv(const char* path) {
        std::vector<char*> csvFiles;
        namespace fs = std::filesystem;
        try {
            
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && entry.path().extension() == ".csv") {
                    char* fileName = new char[entry.path().filename().string().size() + 1];
                    std::strcpy(fileName, entry.path().filename().string().c_str());
                    csvFiles.push_back(fileName);
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error accessing directory: " << e.what() << std::endl;
            // Clear allocated memory if an error occurs
            for (char* fileName : csvFiles) {
                delete[] fileName;
            }
            csvFiles.clear();
        }

        return csvFiles;
    }

    char* concatenateStrings(const char* str1, const char* str2) {
        size_t len1 = strlen(str1);
        size_t len2 = strlen(str2);

        char* concatenated = new char[len1 + len2 + 1];

        strcpy(concatenated, str1);
        strcat(concatenated, str2);
        return concatenated;
    }

    bool compareTargetCurr(const char* path, const char* target) {
        const char* ext_s = strrchr(path, '.');
        if (ext_s == nullptr) {
            return false; // No extension found
        }

        size_t fname_l = ext_s - path;
        size_t target_l = strlen(target);

        if (target_l != fname_l) {
            return false; // Length mismatch
    }

    return strncmp(path, target, target_l) == 0;
}
    void processDir(const char* dir_path, const char* target_curr, int volume) {
        std::vector<char*> csvFiles = parseCsv(dir_path);
        std::vector<Data> allData = readData(csvFiles, target_curr);
        Data data_k = multiplyData(volume, allData);
        writeData("./out.csv", data_k);

        // Clear allocated memory for filenames
        for (char* fileName : csvFiles) {
            delete[] fileName;
        }
    }
};

int main(int argc, char* argv[]) {
    const char* dir_path = "./";
    const char* target_curr = "EURCHF";
    int volume = 1;

    DataProcessor proc;
    proc.processDir(dir_path, target_curr, volume);

    return 0;
}
