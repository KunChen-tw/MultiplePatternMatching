// CompareFiles.cpp : 此檔案包含 'main' 函式。程式會於該處開始執行及結束執行。
//

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib> // 用於 malloc 和 free

using namespace std;


bool compareFiles(const char* file1, const char* file2) {
    FILE* fp1 = nullptr;
    FILE* fp2 = nullptr;

    // Use fopen_s to open the files
    if (fopen_s(&fp1, file1, "rb") != 0 || fopen_s(&fp2, file2, "rb") != 0) {
        cerr << "Unable to open file!" << endl;
        if (fp1) fclose(fp1);
        if (fp2) fclose(fp2);
        return false;
    }

    // Move to the end of the files to get their sizes
    fseek(fp1, 0, SEEK_END);
    fseek(fp2, 0, SEEK_END);
    long size1 = ftell(fp1);
    long size2 = ftell(fp2);

    // Check if the file sizes are the same
    if (size1 != size2) {
        fclose(fp1);
        fclose(fp2);
        return false;
    }

    // Reset file pointers back to the beginning
    rewind(fp1);
    rewind(fp2);

    // Allocate memory to store file contents
    char* buffer1 = (char*)malloc(size1);
    char* buffer2 = (char*)malloc(size2);

    if (buffer1 == nullptr || buffer2 == nullptr) {
        cerr << "Memory allocation failed!" << endl;
        fclose(fp1);
        fclose(fp2);
        free(buffer1);
        free(buffer2);
        return false;
    }

    // Use fread to read file contents into memory
    fread(buffer1, 1, size1, fp1);
    fread(buffer2, 1, size2, fp2);

    // Compare the file contents in memory
    bool areFilesSame = (memcmp(buffer1, buffer2, size1) == 0);

    // Free resources
    fclose(fp1);
    fclose(fp2);
    free(buffer1);
    free(buffer2);

    return areFilesSame;
}


int main() {
    int startL = 4;
    int endL = 16;

    string algoName[] = { "Lookup","Lookup_fgetc","Switch","NoEqual" };
    
    string prefixFileName = ".\\output\\Drosophila_A04_1M";
    //string prefixFileName = ".\\output\\Paper_Example";
    
    for (int i = 1; i <= 3; i++) {

        for (int L = startL; L <= endL; L++) {
            string file1 = prefixFileName + "_L=" + to_string(L) + "_" + algoName[0] + ".txt";
            string file2 = prefixFileName + "_L=" + to_string(L) + "_" + algoName[i] + ".txt";

            if (compareFiles(file1.c_str(), file2.c_str())) {
                cout << file1 << " and " << file2 << " SAME" << endl;
            }
            else {
                cout << file1 << " and " << file2 << " DIFFERENCT" << endl;
            }
        }
    }
    return 0;
}
