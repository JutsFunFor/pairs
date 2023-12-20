#include <bits/stdc++.h>
using namespace std;


class DataProcessor{
public:
    using Data = vector<pair<long, float>>;
    DataProcessor() = default;
    ~DataProcessor() = default;

    Data readData(const char* path, bool reverse=false){
    long t; //timestamp
    float p; //price
    Data data{};
    if (FILE *fp = fopen(path, "r")){
        while (fscanf(fp,"%d;%f", &t, &p)==2){
            data.push_back(make_pair(t, (reverse ? 1.0f/p:p)));
        }
        fclose(fp);
    } else{
        perror("Error opening file\n");
    }
    return data;
}

    void writeData(const char* out_path, const Data& data){
    if (FILE *fp = fopen(out_path, "w+")){
        for (int i=0; i <data.size(); i++)
            fprintf(fp, "%ld;%f\n", data[i].first, data[i].second);
        fclose(fp);
    }
    else{
        perror("Error opening file");
    }
}

    Data multiplyData(int k, const Data& data1, const Data& data2, const Data& data3){
    Data data_k;
    data_k.reserve(data1.size());

    if ((data1.size() == data2.size()) && (data1.size() == data3.size())){
        for (int i=0; i<data1.size(); i++){
            float p = k * data1[i].second * data2[i].second*data3[i].second;
            data_k.push_back(make_pair(data1[i].first, p));
        }
    }
    else{
        printf("Data has unequal sizes\n");
    }
    return data_k;
}

char** parseCsv(const char* path){
    
    namespace fs = std::filesystem;
    try{
        unsigned short count = 0;
        for (const auto& entry: fs::directory_iterator(path)){
            if (entry.is_regular_file() && entry.path().extension() == ".csv"){
                count++;
            }
        }
        if (count == 0){
            return nullptr;
        }

        char** csvFiles = new char*[count+1];
        unsigned short idx = 0;

        for(const auto& entry : fs::directory_iterator(path)){
            if(entry.is_regular_file() && entry.path().extension() == ".csv"){
                csvFiles[idx] = new char[entry.path().filename().string().size() + 1];
                std::strcpy(csvFiles[idx], entry.path().filename().string().c_str());
                idx++;
            }
        }

        csvFiles[count] = nullptr;
        return csvFiles;
    } catch (const fs::filesystem_error& e){
        printf("Error acessing directory: %s\n", e.what());
        return nullptr;
    }
}

char* concatenateStrings(const char* str1, const char* str2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);

    char* concatenated = new char[len1 + len2 + 1]; 

    strcpy(concatenated, str1);
    strcat(concatenated, str2);
    return concatenated;
}

void processDir(const char* dir_path, const char* target_curr){
    // find all csv files
    char** csvFiles = parseCsv(dir_path);
    // read data and reverse target_curr 

    // multiply data and save as out.csv
}



};

int main(int argc, char* argv[] ){
    const char* dir_path = "./";

    const char* path_eur_cad ="./EURCAD.csv";
    const char* path_cad_chf ="./CADCHF.csv";
    const char* path_eur_chf ="./EURCHF.csv";
    const char* out_k_path ="./out.csv";
    int volume = 1;
    
    DataProcessor proc;

    char** csvFiles = proc.parseCsv(dir_path);


    if(csvFiles != nullptr){
        printf("CSV files:\n");
        for (unsigned short i=0; csvFiles[i] != nullptr; i++){
            printf("--%s\n", csvFiles[i]);
            delete[] csvFiles[i];
        }
        delete[] csvFiles;
    } else{
        printf("No csv files found\n");
    }

    DataProcessor::Data data_eur_cad = proc.readData(path_eur_cad);
    DataProcessor::Data data_cad_chf = proc.readData(path_cad_chf);
    DataProcessor::Data data_eur_chf = proc.readData(path_eur_chf, true);
     
    DataProcessor::Data data_k = proc.multiplyData(volume, data_cad_chf, data_eur_cad, data_eur_chf);
    proc.writeData(out_k_path, data_k);

    return 0;
}

