#include <iostream>
#include <chrono>


using namespace std;

int main() {
	const unsigned long long iterations = 1ULL << 20;
	const int times = 10;
	for (int j = 0; j < times; j++)
	{
		//volatile unsigned long long result1;
		//volatile unsigned long long result2;
		// 測試 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL * 1024ULL
		auto start1 = chrono::high_resolution_clock::now();
		for (unsigned long long i = 0; i < iterations; ++i) {
			//volatile unsigned long long result1 = 2ULL * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2; // 避免編譯器優化
			volatile unsigned long long result1 = 1;
			for (int k = 0; k < 60; k++)
			{
				result1 *= 2;
			}
		}
		auto end1 = chrono::high_resolution_clock::now();

		// 測試 1ULL << 60
		auto start2 = chrono::high_resolution_clock::now();
		for (unsigned long long i = 0; i < iterations; ++i) {
			volatile unsigned long long result2 = 1ULL << 60; // 避免編譯器優化
		}
		auto end2 = chrono::high_resolution_clock::now();
		//if (result1 != result2) cout << "result error" << endl;
		// 計算時間差
		auto duration1 = chrono::duration_cast<chrono::microseconds>(end1 - start1).count(); // 轉換為微秒
		auto duration2 = chrono::duration_cast<chrono::microseconds>(end2 - start2).count(); // 轉換為微秒
		cout << "第 " << (j + 1) << " 次" << endl;
		cout << "2^60  耗時: " << duration1 << " 微秒" << endl;
		cout << "1<<60 耗時: " << duration2 << " 微秒" << endl << endl;
	}


	cout << "程式執行完畢，請按任意鍵以結束..." << endl;

	cin.get(); // 等待使用者輸入
	return 0;
}
