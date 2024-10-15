#define _CRT_SECURE_NO_WARNINGS // 在 Microsoft Visual Studio 中 fopen 為不安全函式

#include <iostream>
#include <random>
#include <cstdio> // for fopen, fprintf, fclose



using namespace std;

int main() {
    random_device rd;  // 用於生成隨機數的隨機設備
    mt19937 generator(rd()); // 使用梅森旋轉算法作為隨機數生成器
    uniform_int_distribution<int> distribution(0, 3); // 均勻分布，範圍 0 到 3

    const long long length = 500 * 1024 * 1024; // DNA序列長度設置為100MB (104857600字元)
    const char* filename = "RandomDNA_500M.txt";
    // 使用 fopen 寫入檔案
    FILE* output_file = fopen(filename, "w"); // 開啟檔案進行寫入
    if (output_file != nullptr) {
        const char bases[] = { 'A', 'T', 'C', 'G' }; // DNA 的四種鹼基

        for (long long i = 0; i < length; ++i) {
            int random_index = distribution(generator); // 生成隨機索引
            fprintf(output_file, "%c", bases[random_index]); // 將鹼基寫入檔案
        }

        fclose(output_file); // 關閉檔案
        cout << "隨機生成的DNA序列已寫入 "<< filename << endl;
    }
    else {
        cerr << "無法打開檔案進行寫入" << endl;
    }

    cout << "程式執行完畢，請按任意鍵以結束..." << endl;

    cin.get(); // 等待使用者輸入
    return 0;
}