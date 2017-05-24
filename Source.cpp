#include "Huffman.h"
#include "compareFiles.h"
#include <vector>
#include <iostream>
#include <stdint.h>
#include <fstream>
#include <bitset>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
using namespace cv;
using namespace std;
int colll = 2125;
int cols = 0;
//Function that takes image name and a threshold then return a vector of vectors or (2d array)
vector<vector<int>> convertImageToBinaryData(string imgName, int threshold)
{
	Mat image = imread(imgName, CV_LOAD_IMAGE_GRAYSCALE);
	ofstream orig("original.txt");
	int rows = image.rows;
	cols = image.cols;
	vector <vector<int>> arr;
	for (int r = 0; r < rows; r++)
	{
		vector <int> row;
		for (int c = 0; c < cols; c++)
		{
			if (image.at<uint8_t>(r, c)>threshold)
			{
				row.push_back(1);
				orig << 1;
				image.at<uint8_t>(r, c) = 255;
			}
			else
			{
				orig << 0;
				row.push_back(0);
				image.at<uint8_t>(r, c) = 0;
			}
		}
		arr.push_back(row);
		orig << '\n';
	}
	orig.close();
	return arr;
}
////////////////////
void encodingRLE(vector<vector<int>>& bin,string filename)
{
	vector< pair< int, int>> row;
	int block_char, c, countc = 0, counta = 0;
	int block_len;
	block_len = 0;
	block_char = -1;
	String encodin = "";
	ofstream file;
	file.open(filename);
	colll = bin[0].size();
	for (int i = 0; i < bin.size(); i++)
	{

		block_char = bin[i][0];
		for (int j = 0; j < bin[i].size(); j++)
		{
			c = bin[i][j];
			if (c == block_char)
			{
				/* same block */
				block_len++;
			}
			else
			{
				//std::cout << "blockkkk" << block_char << endl;
				if (block_len)
					
				{   /* write block */

					if (block_char == 1)
					{
						if (counta>0 || countc>0)
							row.push_back(make_pair(block_len, block_char));
						else
						{
							file << block_len << 'w';
						}

					}

					else
					{
						if (counta>0 || countc>0)
							row.push_back(make_pair(block_len, block_char));
						else
						{
							file << block_len << 'b';
						}
					}

					block_len = 0;
				}
				/* start new block */

				block_char = c;
				block_len = 1;
			}

		}

		row.push_back(make_pair(block_len, block_char));

		if (block_len == 2125)
		{
			if (block_char == 0)
			{
				countc++;
			}
			else
			{
				counta++;
			}

		}
		else
		{
			if (counta > 0)
			{
				file << counta << 'a';
				counta = 0;
				for (int f = 0; f < row.size(); f++)
				{
					file << row[f].first;

					if (row[f].second == 1)
						file << 'w';
					else
						file << 'b';
				}
			}
			else if (countc > 0)
			{

				file << countc << 'c';
				countc = 0;
				for (int f = 0; f < row.size(); f++)
				{
					file << row[f].first;

					if (row[f].second == 1)
						file << 'w';
					else
						file << 'b';

				}
			}

			else {
				file << block_len;
				if (block_char == 1)
					file << 'w';
				else
					file << 'b';
			}
			file << '/';

		}


		block_len = 0;
		row.clear();
	}

	if (counta) {
		file << counta << 'a';
	}
	if (countc)
	{
		file << countc << 'c';
	}
	file.close();


	cout << "step 1: RLE encoding passed well " << endl;
}

vector <vector <int>> decodingRLE(string filename)
{
	ifstream file;
	file.open(filename);
	vector <vector <int>> d;
	vector<int> row;
	int c;
	int len;
	char a; String b = "";

	while (!file.eof())
	{
		file >> a;
		if (a == 'a')
		{
			len = stoi(b);
			b = "";
			for (int m = 0; m < colll; m++)
			{
				row.push_back(1);
			}
			for (int j = 0; j < len; j++)
			{
				d.push_back(row);

			}
			row.clear();
			a = 'h';
		}
		else if (a == 'c')
		{
			len = stoi(b);
			b = "";
			for (int m = 0; m < colll; m++)
			{
				row.push_back(0);
			}
			for (int j = 0; j < len; j++)
			{
				d.push_back(row);
			}
			row.clear();
			a = 'h';
		}
		else if (a == 'w')
		{
			len = stoi(b);
			b = "";
			for (int j = 0; j < len; j++)
			{
				row.push_back(1);
			}
			a = 'h';
		}
		else if (a == 'b')
		{
			len = stoi(b);
			b = "";
			for (int j = 0; j < len; j++)
			{
				row.push_back(0);
			}
			a = 'h';
		}
		else if (a == '/')
		{
			if (!row.empty())
			{
				d.push_back(row);
			}
			row.clear();
			b = "";
			a = 'h';
		}
		else
		{
			b += a;
		}
		if (d.size() == 2585)
		{
			char f;
			f = 'd';
		}
	}

	cout << "step 3 decoding: RLE decoding passed well" << endl;

	return d;
}
//8 bits encoding and decoding starts here
void BinaryToAsciiEncoding(string BinaryFileName, string AsciiFileName)
{
	
	ifstream in(BinaryFileName);
	
	string row,row2;
	getline(in, row2);
	
	in >> row;
	in.close();
	ofstream out(AsciiFileName);
	out << row2 << '\n';
	out << row.size()%8<<'\n';
	istringstream inn(row);
	bitset<8> bs;
	while (inn >> bs)
	{
		out << char(bs.to_ulong());
	}
	
	out.close();

	cout << "step 4: 8-bit encoding passed well " << endl;
}
////////////////////

void AsciiToBinaryDecoding(string AsciiFileName, string DecodedFileName)
{

	int ColSize;
	string row;
	ifstream  f(AsciiFileName, ios::in | ios::binary);
	ofstream out(DecodedFileName);
	getline(f, row);
	f >> ColSize;
	string temp = "";
	int x;
	string binaryString1, binaryString2;
	out << row << '\n';
	vector<char> c(istreambuf_iterator<char>(f), (istreambuf_iterator<char>()));

	for (int i = 2; i < c.size()-1; i++)
	{
				if (c[i + 1] != '\n')
				{
					binaryString1 = "";
					binaryString2 = "";
					temp = "";
					unsigned char c2 = c[i];
					x = int(c2);
					while (x)
					{
						if (x % 2 == 0)
							binaryString1 += '0';
						else
							binaryString1 += '1';
						x /= 2;
					}
					if (binaryString1.size() != 8)
						for (int i = 0; i < 8 - binaryString1.size(); i++)
							temp += '0';

					for (int j = binaryString1.size() - 1; j > -1; j--)
					{
						temp += binaryString1[j];
					}
					out << temp;
				}

	}
	binaryString1 = "";
	binaryString2 = "";
	temp = "";
	unsigned char c2 = c[c.size()-1];
	x = int(c2);
	while (x)
	{
		if (x % 2 == 0)
			binaryString1 += '0';
		else
			binaryString1 += '1';
		x /= 2;
	}
	if (binaryString1.size() != 8)
		for (int i = 0; i < 8 - binaryString1.size(); i++)
			temp += '0';

	for (int j = binaryString1.size() - 1; j > -1; j--)
	{
		temp += binaryString1[j];
	}
	///Last bit check
	if (ColSize % 8 != 0)
	{
		string temm = temp;
		temp = "";
		for (int i = 8 - (ColSize % 8); i < temm.size(); i++)
		{
			temp += temm[i];
		}
	}
	out << temp;


	cout << "step 1 decoding: 8 bit decoding passed well" << endl;
}

//8 bits encosing and decoding ends here
int main(int argc, char** argv)
{
	String folderpath = "DataSet";
	vector<String> filename;
	glob(folderpath, filename);
	for (int imageIndex = 0; imageIndex < filename.size(); imageIndex++)
	//int imageIndex = 3;
	{
		//cout << filename[imageIndex] << endl;
//	}
		string image = filename[imageIndex];////////////////
		//make the two text files:
		string encoded = "", decoded = "";///////////
		for (int i = folderpath.size() + 1; i < image.size() - 4; i++)//////////
		{
			encoded += image[i];
			decoded += image[i];
		}
		string str1 = to_string(imageIndex);
		encoded = encoded + "Encodod" + str1 + ".txt";//////////
		decoded = decoded + "Decodod" + str1 + "release"+".txt";/////////////
		cout << encoded << endl << decoded << endl;
		//Encoding
		vector <vector <int>>arr = convertImageToBinaryData(image, 180);///////////////////
		encodingRLE(arr, encoded);
		HuffmanEncodemain(encoded);
		BinaryToAsciiEncoding(encoded, encoded);

		//Decoding
		AsciiToBinaryDecoding(encoded, decoded);
		HuffmanDecodingmain(decoded);
		vector <vector <int>>decode = decodingRLE(decoded);

		//make two output files one for the original text file and the other for the decompressed file
		ofstream fin(decoded);
		string originalfile = "original" + str1 + ".txt";
		ofstream org(originalfile);

		//Checking losslessness
		if (arr.size() != decode.size())
			cout << "error 3" << imageIndex << endl;
		else
		{
			for (int i = 0; i < arr.size(); i++)
			{
				if (arr[i].size() != decode[i].size())
				{
					cout << "error 1" << imageIndex << endl;
					//return 0;
					break;
				}
				for (int j = 0; j < arr[i].size(); j++)
				{
					if (arr[i][j] != decode[i][j])
					{
						cout << "error 2" << imageIndex << endl;
						//return 0;
						break;
					}
					org << arr[i][j];
					fin << decode[i][j];
				}
			}
		}
		//compare the original file of the image with the decompresssed file(last decoded file)
		//if (compareFiles(originalfile, decoded))
			//cout << "done  done doneeeee"<<endl;
		org.close();
		fin.close();
	}
	waitKey();
	return 0;
}

