#define _CRT_SECURE_NO_WARNINGS
#define _SHOW_MESSAGE


#include <iostream>
#include <string>
#include <bitset>
#include <chrono>
#include <filesystem>
#include <cstdlib>

using namespace std;

/* Define the maximum size of alphabet */
#define MAX_ALPHABET_SIZE 255

/* Define the abbreviated names */
typedef unsigned char UC;
typedef unsigned long long ULL;


enum DNA_Base {
	A = 0,  // 對應數字 0
	C = 1,  // 對應數字 1
	G = 2,  // 對應數字 2
	T = 3,  // 對應數字 3
	COUNT = 4
};
int base_lookup(char base) {
	switch (base) {
	case 'A': return DNA_Base::A;
	case 'T': return DNA_Base::T;
	case 'C': return DNA_Base::C;
	case 'G': return DNA_Base::G;
	default: throw invalid_argument("Invalid DNA base");
	}
}

int popcount(ULL i) {
	i = i - ((i >> 1) & 0x5555555555555555ULL);
	i = (i & 0x3333333333333333ULL) + ((i >> 2) & 0x3333333333333333ULL);
	return (int)((((i + (i >> 4)) & 0xF0F0F0F0F0F0F0FULL) * 0x101010101010101ULL) >> 56);
}

/**
 * ReadFile - Reads file content and stores it in dynamically allocated memory
 *
 * @param inputFile: The name of the file to be read
 * @param charText: A pointer to a pointer, which will be allocated memory to store file content
 * @return: The file size in bytes, or -1 if an error occurs
 *
 * Description:
 * This function opens the specified file, calculates its size, and reads the content into dynamically
 * allocated memory. It ensures that any previously allocated memory for `charText` is freed before
 * allocating new memory. After the file content is read, a null terminator is added to make it a valid string.
 */
int ReadFile(const char* filename, UC** fileText) {
	FILE* fp = NULL;  // Declare a FILE pointer to open the file

	// Open the file in binary read mode ("rb")
	errno_t ferr = fopen_s(&fp, filename, "rb");

	// Check if the file was successfully opened
	if (fp == NULL || ferr != 0) {
		// If the file fails to open, print an error message and exit the program
		printf("ERROR: Cannot read the text file (\"%s\").\n", filename);
		exit(1);
		return -1;
	}

	// Move the file pointer to the end of the file
	fseek(fp, 0, SEEK_END);

	// Get the current position of the file pointer, which represents the file size
	long fileSize = ftell(fp);

	// If *charText has been allocated before, free it to avoid memory leaks
	if (*fileText != NULL) {
		free(*fileText);  // Free previously allocated memory
		*fileText = NULL; // Set the pointer to NULL to avoid dangling pointers
	}

	// Allocate memory to store the file content, including space for a null terminator
	*fileText = (UC*)malloc(fileSize + 1);
	if (*fileText == NULL) {
		// If memory allocation fails, print an error message and close the file
		printf("ERROR: Memory allocation failed.\n");
		fclose(fp);
		exit(1);
		return -1;
	}

	// Reset the file pointer to the beginning of the file
	fseek(fp, 0, SEEK_SET);

	// Read the file content into the allocated memory
	fread(*fileText, 1, fileSize, fp);

	// Add a null terminator to ensure the content is a valid string
	(*fileText)[fileSize] = '\0';

	// Close the file
	fclose(fp);

	// Return the size of the file
	return fileSize;
}

void WriteFile(const char* filename, UC* data, int length) {
	FILE* fp = NULL;


	// Check whether the opening file for text is successful 
	errno_t ferr = fopen_s(&fp, filename, "w");
		
		// Check if the file was successfully opened
	if (fp == NULL || ferr != 0) {
		// If fail to open the file, free the used memory and stop.
		printf("ERROR: Cannot write the text file (\"%s\").\n", filename);
		exit(2);
		return;
	}

	for (int i = 0; i < length; i++) {
		fprintf(fp, "%d\n", data[i]);
	}

	// Close the file of text 
	fclose(fp);
}

void LookupTable_fgetc(int parameterL, const char* inputFile, bool isOutput, const char* outputFile);
void LookupTable(int parameterL, const char* inputFile, bool isOutput = false, const char* outputFile = "");
void SwitchCharacter(int parameterL, const char* inputFile, bool isOutput = false, const char* outputFile = "");
void NotEqual(int parameterL, const char* inputFile, bool isOutput = false, const char* outputFile = "");

int main(int argc, char* argv[])
{

	string algoName[] = {
		"NotEqual",
		"Switch",
		"Lookup",
		"Lookup_fgetc",
	};
	
	// Define an array of function pointers
	void (*func[])(int parameterL, const char* inputFile, bool isOutput, const char* outputFile) = {
		NotEqual,
		SwitchCharacter,
		LookupTable,
		LookupTable_fgetc,
	};

	int startL = 4;
	int endL = 21;
	int startAlgo = 0;
	int endAlgo = 3;
	//bool isOutput = true;
	bool isOutput = false;

	// input file name
	//const char* inputFile = ".\\input\\Paper_Example.txt";
	//const char* inputFile = ".\\input\\Drosophila_A04_1M.txt";
	//const char* inputFile = ".\\input\\RandomDNA_100M.txt";
	const char* inputFile = ".\\input\\RandomDNA_500M.txt";
	const char* outputFile = NULL;
	//string outputPrefixFile = ".\\output\\Paper_Example";
	//string outputPrefixFile = ".\\output\\Drosophila_A04_1M";
	//string outputPrefixFile = ".\\output\\RandomDNA_100M";
	string outputPrefixFile = ".\\output\\RandomDNA_500M";

	if (argc >= 3) {
		startAlgo = atoi(argv[1]) - 1;
		endAlgo = startAlgo;
		startL = atoi(argv[2]);
		endL = startL;
		inputFile = argv[3];
	}


	for (int i = startAlgo; i <= endAlgo; i++) {
		cout << "algorithm: " << algoName[i];
		if (argc < 3) {
			cout << endl;
		}
		for (int L = startL; L <= endL; L++)
		{

			if (isOutput) {
				
				string outputSuffixFile = outputPrefixFile + "_L=" + to_string(L) + "_" + algoName[i] + ".txt";
				outputFile = outputSuffixFile.c_str();

				func[i](L, inputFile, isOutput, outputFile);
			}
			else {
				cout << " L:" << setw(2) << right << L;

				auto start = chrono::high_resolution_clock::now();

				func[i](L, inputFile, false, "");

				auto end = chrono::high_resolution_clock::now();
				

				auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count(); 
				cout << " RT:" << duration / 1000.0 << endl;
			}


		}
	}
}



void LookupTable(int L, const char* inputFile, bool isOutput, const char* outputFile) {
	auto startTotalTime = chrono::high_resolution_clock::now();

	int i = 0;                            

	auto startReadFile = chrono::high_resolution_clock::now();
	UC* fileText = NULL;
	int fileSize = ReadFile(inputFile, &fileText); // The length of text

	if (fileSize < 0) {
		printf("ERROR: Cannot read the text file (\"%s\").\n", inputFile);
		exit(1);
		return;
	}

	auto endReadFile = chrono::high_resolution_clock::now();

	auto startCreatTable = chrono::high_resolution_clock::now();
	UC alphabet[MAX_ALPHABET_SIZE];        // The alphabet of strings
	// 先將字母表設成 0xff 表示未使用到
	memset(alphabet, 0xff, MAX_ALPHABET_SIZE); 

	UC inputChar = 0;                      // A temporary character for reading text string
	// 標記用到的字母
	for (i = 0; i < fileSize; i++) {
		inputChar = fileText[i];
		alphabet[inputChar] = 0;
	}
	UC sizeAlphabet = 0;                   // The size of alphabet 
	// 計算inputFile用到多少個字母表，並且根據ASCII順序編碼 0,1,2...
	for (i = 0; i != MAX_ALPHABET_SIZE; i++)
		if (!alphabet[i])
			alphabet[i] = sizeAlphabet++;  // Assign an unique number for this symbol

	UC numberBitForSymbol = 1;             // The number of bits used to record a symbol
	// 計算一個字母需要多少個bit
	for (i = 2; i < sizeAlphabet; i <<= 1)
		numberBitForSymbol++;              // Calculate the number of bits used to record a symbol 
	numberBitForSymbol++;                  // An extra leading witness bit is needed 

	auto endCreatTable = chrono::high_resolution_clock::now();

	auto startCharToBit = chrono::high_resolution_clock::now();
	UC ENV = sizeof(ULL) * 8;              // The size of a computer word
	// If the parameter l is too large that one computer word cannot deal with it, stop.
	if (L * numberBitForSymbol > ENV)
	{
		printf("ERROR: The parameter l is too large! (A unit (unsigned long long) cannot handle it.)\n");
		exit(1);
		return;
	}
	if (fileSize < L) {
		printf("ERROR: The parameter l is too small!\n");
		exit(1);
		return;
	}


	int numberInputString = fileSize - L + 1;             // The number of input strings 


#ifndef _SHOW_MESSAGE
	printf("text length = %d \n", fileSize);
	printf("the number of the bits = %d\n", numberBitForSymbol);
	printf("the number of input strings = %d\n", numberInputString);
	cout << "filterUselessBit = " << bitset<15>(filterUselessBit) << endl;
	cout << "clearWitnessBit  = " << bitset<15>(clearWitnessBit) << endl;
	cout << "keepWitnessBit   = " << bitset<15>(keepWitnessBit) << endl;
#endif // !_SHOW_MESSAGE


	// Request memory for bit string with respect to text string
	ULL* bitText = NULL;                   // The bit string with respect to text string
	bitText = (ULL*)calloc(numberInputString, sizeof(ULL));
	if (bitText == NULL) {
		// memory allocation fails
		printf("Memory allocation failed\n");
		exit(1);
		return;
	}


	// Read text string again and transfer each character into its corresponding number
	for (i = 0; i < L; i++) {
		bitText[0] = (bitText[0] << numberBitForSymbol) | alphabet[fileText[i]];
	}
	for (int i = 1; i < numberInputString; i++) {
		bitText[i] = (bitText[i - 1] << numberBitForSymbol) | alphabet[fileText[L + i - 1]];
	}

	auto endCharToBit = chrono::high_resolution_clock::now();

#ifndef _SHOW_MESSAGE
	for (i = 0; i < numberInputString; i++) {
		cout << "i=" << i << " bitText = " << bitset<15>(bitText[i]) << endl;
	}
#endif // !_SHOW_MESSAGE

	auto startCalDistance = chrono::high_resolution_clock::now();

	// If characters can exactly fill up in an ULL, no bit will be filted;
    // Otherwise, compute the mask for filtering the unused bits in the high positions.
	ULL filterUselessBit = 0x0ULL;         // The mask for filtering unused bits in a ULL 
	const ULL bitforSymbol = L * numberBitForSymbol;
	if (L * numberBitForSymbol == ENV)
		filterUselessBit = ~0x0ULL;
	else
		filterUselessBit = (0x1ULL << (L * numberBitForSymbol)) - 1;

	// Compute the masks for filtering witness bits and keeping witness bits
	ULL clearWitnessBit = 0x0ULL;          // The mask for filtering the witness bits
	ULL keepWitnessBit = 0x0ULL;           // The mask for filtering the bits whic are not witness bits
	keepWitnessBit = clearWitnessBit = 0x1ULL << (numberBitForSymbol - 1);
	for (i = 1; i != L; i++)
		keepWitnessBit = (keepWitnessBit << numberBitForSymbol) | clearWitnessBit;
	clearWitnessBit = (~keepWitnessBit) & filterUselessBit;

	// Calculate distance
	// Request memory for recording the distances between input string and reference string 
	UC* distance = NULL;                   // The Hamming distances 
	distance = (UC*)malloc(numberInputString);
	if (distance == NULL) {
		// memory allocation fails
		printf("Memory allocation failed\n");
		exit(1);
		return;
	}
	for (i = 0; i < numberInputString; i++) {
		ULL xorResult = bitText[0] ^ bitText[i];
		distance[i] = popcount((xorResult + clearWitnessBit) & keepWitnessBit);

#ifndef _SHOW_MESSAGE
		printf("%d,", distance[i]);
#endif // !_SHOW_MESSAGE
	}
#ifndef _SHOW_MESSAGE
	printf("\n");
#endif // !_SHOW_MESSAGE

	auto endCalDistance = chrono::high_resolution_clock::now();

	if (isOutput) {
		WriteFile(outputFile, distance, numberInputString);
	}

	auto startFreeMemory = chrono::high_resolution_clock::now();
	if (bitText != NULL) {
		free(bitText);
		bitText = NULL;
	}
	if (distance != NULL) {
		free(distance);
		distance = NULL;
	}
	auto endFreeMemory = chrono::high_resolution_clock::now();
	auto endTotalTime = chrono::high_resolution_clock::now();
	auto durReadFile = chrono::duration_cast<chrono::microseconds>(endReadFile - startReadFile).count();
	auto durCreatTable = chrono::duration_cast<chrono::microseconds>(endCreatTable - startCreatTable).count();
	auto durCharToBit = chrono::duration_cast<chrono::microseconds>(endCharToBit - startCharToBit).count();
	auto durCalDistance = chrono::duration_cast<chrono::microseconds>(endCalDistance - startCalDistance).count();
	auto durFreeMemory = chrono::duration_cast<chrono::microseconds>(endFreeMemory - startFreeMemory).count();
	auto durTotalTime = chrono::duration_cast<chrono::microseconds>(endTotalTime - startTotalTime).count();

	cout << " RF:" << fixed << setprecision(3) << durReadFile / 1000.0;
	cout << " CT:" << fixed << setprecision(3) << durCreatTable / 1000.0;
	cout << " CB:" << fixed << setprecision(3) << durCharToBit / 1000.0;
	cout << " CD:" << fixed << setprecision(3) << durCalDistance / 1000.0;
	cout << " FM:" << fixed << setprecision(3) << durFreeMemory / 1000.0;
	cout << " TT:" << fixed << setprecision(3) << durTotalTime / 1000.0;
}

void LookupTable_fgetc(int L, const char* inputFile, bool isOutput, const char* outputFile) {
	auto startTotalTime = chrono::high_resolution_clock::now();

	int i = 0;                             /* for loop index */
	auto startCreatTable = chrono::high_resolution_clock::now();
	FILE* fp = NULL;          /* The FILE pointer for opening the file of text */
	errno_t ferr = fopen_s(&fp, inputFile, "rb");

	/* Check whether the opening file for text is successful */
	if (fp == NULL || ferr != 0)
	{
		/* If fail to open the file, free the used memory and stop. */
		printf("ERROR: Cannot read the text file (\"%s\").\n", inputFile);
		exit(1);
		return;
	}

	UC alphabet[MAX_ALPHABET_SIZE];        /* The alphabet of strings */
	/* Read the text string and record the alphabet of text */
	memset(alphabet, 0xff, MAX_ALPHABET_SIZE); //先將字母表設成 0xff 表示未使用到

	UC inputChar = 0;                      /* A temporary character for reading text string */
	while ((char)(inputChar = fgetc(fp)) != EOF)
		alphabet[inputChar] = 0; // 將讀到的字母表設成 0 表示該字母有用到
	int fileSize = ftell(fp);  /* Obtain the length of text string */

	UC sizeAlphabet = 0;                   // The size of alphabet 
	// 計算inputFile用到多少個字母表，並且根據ASCII順序編碼 0,1,2...
	for (i = 0; i != MAX_ALPHABET_SIZE; i++)
		if (!alphabet[i])
			alphabet[i] = sizeAlphabet++; // Assign an unique number for this symbol

	UC numberBitForSymbol = 1;             // The number of bits used to record a symbol
	// 計算一個字母需要多少個bit
	for (i = 2; i < sizeAlphabet; i <<= 1)
		numberBitForSymbol++;             /* Calculate the number of bits used to record a symbol */
	numberBitForSymbol++;                 /* An extra leading witness bit is needed */
	auto endCreatTable = chrono::high_resolution_clock::now();

	auto startCharToBit = chrono::high_resolution_clock::now();
	UC ENV = sizeof(ULL) * 8;              // The size of a computer word
	// If the parameter l is too large that one computer word cannot deal with it, stop.
	if (L * numberBitForSymbol > ENV)
	{
		printf("ERROR: The parameter l is too large! (A unit (unsigned long long) cannot handle it.)\n");
		exit(1);
		return;
	}
	if (fileSize < L) {
		printf("ERROR: The parameter l is too small!\n");
		exit(1);
		return;
	}

	int numberInputString = fileSize - L + 1;             // The number of input strings 


#ifndef _SHOW_MESSAGE
	printf("text length = %d \n", fileSize);
	printf("the number of the bits = %d\n", numberBitForSymbol);
	printf("the number of input strings = %d\n", numberInputString);
	cout << "filterUselessBit = " << bitset<15>(filterUselessBit) << endl;
	cout << "clearWitnessBit  = " << bitset<15>(clearWitnessBit) << endl;
	cout << "keepWitnessBit   = " << bitset<15>(keepWitnessBit) << endl;
#endif // !_SHOW_MESSAGE

	// Request memory for bit string with respect to text string
	ULL* bitText = NULL;                   // The bit string with respect to text string
	bitText = (ULL*)calloc(numberInputString, sizeof(ULL));
	if (bitText == NULL) {
		// memory allocation fails
		printf("Memory allocation failed\n");
		exit(1);
		return;
	}

    // Read text string again and transfer each character into its corresponding number 
	fseek(fp, 0, SEEK_SET);
	for (i = 0; i < L; i++) {
		bitText[0] = (bitText[0] << numberBitForSymbol) | alphabet[fgetc(fp)];
	}
	for (int i = 1; i < numberInputString; i++) {
		bitText[i] = (bitText[i - 1] << numberBitForSymbol) | alphabet[fgetc(fp)];
	}
	// Close the file of text 
	fclose(fp);

	auto endCharToBit = chrono::high_resolution_clock::now();

#ifndef _SHOW_MESSAGE
	for (i = 0; i < numberInputString; i++) {
		cout << "i=" << i << " bitText = " << bitset<15>(bitText[i]) << endl;
	}
#endif // !_SHOW_MESSAGE

	auto startCalDistance = chrono::high_resolution_clock::now();

	// If characters can exactly fill up in an ULL, no bit will be filted;
	// Otherwise, compute the mask for filtering the unused bits in the high positions.
	ULL filterUselessBit = 0x0ULL;         // The mask for filtering unused bits in a ULL 
	const ULL bitforSymbol = L * numberBitForSymbol;
	if (L * numberBitForSymbol == ENV)
		filterUselessBit = ~0x0ULL;
	else
		filterUselessBit = (0x1ULL << (L * numberBitForSymbol)) - 1;

	// Compute the masks for filtering witness bits and keeping witness bits
	ULL clearWitnessBit = 0x0ULL;          // The mask for filtering the witness bits
	ULL keepWitnessBit = 0x0ULL;           // The mask for filtering the bits whic are not witness bits
	keepWitnessBit = clearWitnessBit = 0x1ULL << (numberBitForSymbol - 1);
	for (i = 1; i != L; i++)
		keepWitnessBit = (keepWitnessBit << numberBitForSymbol) | clearWitnessBit;
	clearWitnessBit = (~keepWitnessBit) & filterUselessBit;

	// Calculate distance
	// Request memory for recording the distances between input string and reference string 
	UC* distance = NULL;                   // The Hamming distances 
	distance = (UC*)malloc(numberInputString);
	if (distance == NULL) {
		// memory allocation fails
		printf("Memory allocation failed\n");
		exit(1);
		return;
	}

	// Calculate distance
	for (i = 0; i < numberInputString; i++) {
		ULL xorResult = bitText[0] ^ bitText[i];
		distance[i] = popcount((xorResult + clearWitnessBit) & keepWitnessBit);
#ifndef _SHOW_MESSAGE
		printf("%d,", distance[i]);
#endif // !_SHOW_MESSAGE
	}
#ifndef _SHOW_MESSAGE
	printf("\n");
#endif // !_SHOW_MESSAGE
	auto endCalDistance = chrono::high_resolution_clock::now();

	if (isOutput) {
		WriteFile(outputFile, distance, numberInputString);
	}

	auto startFreeMemory = chrono::high_resolution_clock::now();
	if (bitText != NULL) {
		free(bitText);
		bitText = NULL;
	}
	if (distance != NULL) {
		free(distance);
		distance = NULL;
	}
	auto endFreeMemory = chrono::high_resolution_clock::now();
	auto endTotalTime = chrono::high_resolution_clock::now();

	//auto durReadFile = chrono::duration_cast<chrono::microseconds>(endReadFile - startReadFile).count();
	auto durCreatTable = chrono::duration_cast<chrono::microseconds>(endCreatTable - startCreatTable).count();
	auto durCharToBit = chrono::duration_cast<chrono::microseconds>(endCharToBit - startCharToBit).count();
	auto durCalDistance = chrono::duration_cast<chrono::microseconds>(endCalDistance - startCalDistance).count();
	auto durFreeMemory = chrono::duration_cast<chrono::microseconds>(endFreeMemory - startFreeMemory).count();
	auto durTotalTime = chrono::duration_cast<chrono::microseconds>(endTotalTime - startTotalTime).count();

	cout << " RF:" << fixed << setprecision(3) << 0 / 1000.0;
	cout << " CT:" << fixed << setprecision(3) << durCreatTable / 1000.0;
	cout << " CB:" << fixed << setprecision(3) << durCharToBit / 1000.0;
	cout << " CD:" << fixed << setprecision(3) << durCalDistance / 1000.0;
	cout << " FM:" << fixed << setprecision(3) << durFreeMemory / 1000.0;
	cout << " TT:" << fixed << setprecision(3) << durTotalTime / 1000.0;
}

void SwitchCharacter(int L, const char* inputFile, bool isOutput, const char* outputFile) {
	auto startTotalTime = chrono::high_resolution_clock::now();
	int i = 0;                             

	auto startReadFile = chrono::high_resolution_clock::now();
	UC* fileText = NULL;
	int fileSize = ReadFile(inputFile, &fileText); // The length of text

	if (fileSize < 0) {
		printf("ERROR: Cannot read the text file (\"%s\").\n", inputFile);
		exit(1);
		return;
	}
	auto endReadFile = chrono::high_resolution_clock::now();
	auto startCharToBit = chrono::high_resolution_clock::now();
	UC ENV = sizeof(ULL) * 8;              // The size of a computer word
	UC numberBitForSymbol = 3;             // The number of bits used to record a symbol. 這裡只考慮 ACGT
	// If the parameter l is too large that one computer word cannot deal with it, stop.
	if (L * numberBitForSymbol > ENV)
	{
		printf("ERROR: The parameter l is too large! (A unit (unsigned long long) cannot handle it.)\n");
		exit(1);
		return;
	}
	if (fileSize < L) {
		printf("ERROR: The parameter l is too small!\n");
		exit(1);
		return;
	}

	// Compute the number of total input strings 
	int numberInputString = fileSize - L + 1;             // The number of input strings 

#ifndef _SHOW_MESSAGE
	printf("text length = %d \n", fileSize);
	printf("the number of the bits = %d\n", numberBitForSymbol);
	printf("the number of input strings = %d\n", numberInputString);
	cout << "filterUselessBit = " << bitset<15>(filterUselessBit) << endl;
	cout << "clearWitnessBit  = " << bitset<15>(clearWitnessBit) << endl;
	cout << "keepWitnessBit   = " << bitset<15>(keepWitnessBit) << endl;
#endif // !_SHOW_MESSAGE

	// Request memory for bit string with respect to text string 
	ULL* bitText = NULL;                   // The bit string with respect to text string 
	bitText = (ULL*)calloc(numberInputString, sizeof(ULL));
	if (bitText == NULL) {
		// 記憶體分配失敗的處理邏輯
		printf("Memory allocation failed\n");
		exit(1);
		return;
	}

	// Read text string again and transfer each character into its corresponding number 
	for (i = 0; i < L; i++) {
		bitText[0] = (bitText[0] << numberBitForSymbol) | base_lookup(fileText[i]);
	}
	for (int i = 1; i < numberInputString; i++) {
		bitText[i] = (bitText[i - 1] << numberBitForSymbol) | base_lookup(fileText[L + i - 1]);
	}
	auto endCharToBit = chrono::high_resolution_clock::now();

#ifndef _SHOW_MESSAGE
	for (i = 0; i < numberInputString; i++) {
		cout << "i=" << i << " bitText = " << bitset<15>(bitText[i]) << endl;
	}
#endif // !_SHOW_MESSAGE
	auto startCalDistance = chrono::high_resolution_clock::now();

	// If characters can exactly fill up in an ULL, no bit will be filted;
	// Otherwise, compute the mask for filtering the unused bits in the high positions.
	ULL filterUselessBit = 0x0ULL;         // The mask for filtering unused bits in a ULL
	const ULL bitforSymbol = L * numberBitForSymbol;
	if (L * numberBitForSymbol == ENV)
		filterUselessBit = ~0x0ULL;
	else
		filterUselessBit = (0x1ULL << (L * numberBitForSymbol)) - 1;

	ULL clearWitnessBit = 0x0ULL;          // The mask for filtering the witness bits
	ULL keepWitnessBit = 0x0ULL;           // The mask for filtering the bits whic are not witness bits
	// Compute the masks for filtering witness bits and keeping witness bits
	keepWitnessBit = clearWitnessBit = 0x1ULL << (numberBitForSymbol - 1);
	for (i = 1; i != L; i++)
		keepWitnessBit = (keepWitnessBit << numberBitForSymbol) | clearWitnessBit;
	clearWitnessBit = (~keepWitnessBit) & filterUselessBit;

	// Calculate distance
	// Request memory for recording the distances between input string and reference string 
	UC* distance = NULL;                   // The Hamming distances 
	distance = (UC*)malloc(numberInputString);
	if (distance == NULL) {
		// memory allocation fails
		printf("Memory allocation failed\n");
		exit(1);
		return;
}
	for (i = 0; i < numberInputString; i++) {
		ULL xorResult = bitText[0] ^ bitText[i];
		distance[i] = popcount((xorResult + clearWitnessBit) & keepWitnessBit);

#ifndef _SHOW_MESSAGE
		printf("%d,", distance[i]);
#endif // !_SHOW_MESSAGE
	}
#ifndef _SHOW_MESSAGE
	printf("\n");
#endif // !_SHOW_MESSAGE

	auto endCalDistance = chrono::high_resolution_clock::now();

	if (isOutput) {
		WriteFile(outputFile, distance, numberInputString);
	}

	auto startFreeMemory = chrono::high_resolution_clock::now();
	if (bitText != NULL) {
		free(bitText);
		bitText = NULL;
	}
	if (distance != NULL) {
		free(distance);
		distance = NULL;
	}
	auto endFreeMemory = chrono::high_resolution_clock::now();
	auto endTotalTime = chrono::high_resolution_clock::now();

	auto durReadFile = chrono::duration_cast<chrono::microseconds>(endReadFile - startReadFile).count();
	//auto durCreatTable = chrono::duration_cast<chrono::microseconds>(endCreatTable - startCreatTable).count();
	auto durCharToBit = chrono::duration_cast<chrono::microseconds>(endCharToBit - startCharToBit).count();
	auto durCalDistance = chrono::duration_cast<chrono::microseconds>(endCalDistance - startCalDistance).count();
	auto durFreeMemory = chrono::duration_cast<chrono::microseconds>(endFreeMemory - startFreeMemory).count();
	auto durTotalTime = chrono::duration_cast<chrono::microseconds>(endTotalTime - startTotalTime).count();

	cout << " RF:" << fixed << setprecision(3) << durReadFile / 1000.0;
	cout << " CT:" << fixed << setprecision(3) << 0 / 1000.0;
	cout << " CB:" << fixed << setprecision(3) << durCharToBit / 1000.0;
	cout << " CD:" << fixed << setprecision(3) << durCalDistance / 1000.0;
	cout << " FM:" << fixed << setprecision(3) << durFreeMemory / 1000.0;
	cout << " TT:" << fixed << setprecision(3) << durTotalTime / 1000.0;
}

void NotEqual(int L, const char* inputFile, bool isOutput, const char* outputFile) {
	auto startTotalTime = chrono::high_resolution_clock::now();
	
	int i = 0;                             
	auto startReadFile = chrono::high_resolution_clock::now();
	UC* fileText = NULL;
	int fileSize = ReadFile(inputFile, &fileText); // The length of text

	if (fileSize < 0) {
		printf("ERROR: Cannot read the text file (\"%s\").\n", inputFile);
		exit(1);
		return;
	}
	auto endReadFile = chrono::high_resolution_clock::now();

	auto startCalDistance = chrono::high_resolution_clock::now();
	// Compute the number of total input strings 
	int numberInputString = fileSize - L + 1;             // The number of input strings 

	UC* distance = NULL;                   // The Hamming distances 
	distance = (UC*)malloc(numberInputString);
	if (distance == NULL) {
		// memory allocation fails
		printf("Memory allocation failed\n");
		exit(1);
		return;
	}
	for (i = 0; i < numberInputString; i++) {
		int diffcount = 0;
		for (int l = 0; l < L; l++) {
			if (fileText[l] != fileText[i + l]) {
				diffcount++;
			}
		}
		distance[i] = diffcount;

#ifndef _SHOW_MESSAGE
		printf("%d,", distance[i]);
#endif // !_SHOW_MESSAGE
	}
#ifndef _SHOW_MESSAGE
	printf("\n");
#endif // !_SHOW_MESSAGE		

	auto endCalDistance = chrono::high_resolution_clock::now();

	if (isOutput) {
		WriteFile(outputFile, distance, numberInputString);
	}

	auto startFreeMemory = chrono::high_resolution_clock::now();
	if (distance != NULL) {
		free(distance);
		distance = NULL;
	}
	auto endFreeMemory = chrono::high_resolution_clock::now();
	auto endTotalTime = chrono::high_resolution_clock::now();

	auto durReadFile = chrono::duration_cast<chrono::microseconds>(endReadFile - startReadFile).count();
	//auto durCreatTable = chrono::duration_cast<chrono::microseconds>(endCreatTable - startCreatTable).count();
	//auto durCharToBit = chrono::duration_cast<chrono::microseconds>(endCharToBit - startCharToBit).count();
	auto durCalDistance = chrono::duration_cast<chrono::microseconds>(endCalDistance - startCalDistance).count();
	auto durFreeMemory = chrono::duration_cast<chrono::microseconds>(endFreeMemory - startFreeMemory).count();
	auto durTotalTime = chrono::duration_cast<chrono::microseconds>(endTotalTime - startTotalTime).count();

	cout << " RF:" << fixed << setprecision(3) << durReadFile / 1000.0;
	cout << " CT:" << fixed << setprecision(3) << 0 / 1000.0;
	cout << " CB:" << fixed << setprecision(3) << 0 / 1000.0;
	cout << " CD:" << fixed << setprecision(3) << durCalDistance / 1000.0;
	cout << " FM:" << fixed << setprecision(3) << durFreeMemory / 1000.0;
	cout << " TT:" << fixed << setprecision(3) << durTotalTime / 1000.0;

}
