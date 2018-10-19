#include "File.hpp"
#include <iostream>
#include <fstream>


void WriteTrainingData(training_data &Data, const char *FileName)
{
	using namespace std;
	ofstream File;
	File.open(FileName, ios::out | ios::binary | ios::trunc);
	if(!File)
	{
		cout << "File Open Error" << endl;
	}
	File.write((char*)&Data, sizeof(training_data));
	File.close();
}
