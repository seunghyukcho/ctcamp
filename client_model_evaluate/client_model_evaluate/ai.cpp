#include"ai.h"

#include <iostream>
#include <io.h>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <stdio.h>
#include <conio.h>

using namespace std;
using namespace cv;

areaInfo area[MAX_SIZE_AREA];

void aiInit(void)
{
	for (int i = 0; i < MAX_SIZE_AREA; i++)
	{
		area[i].isWall = FALSE;
		area[i].pictureLen = 0;
		area[i].picture_png = (char*)calloc(MAX_SIZE_PICTURE, sizeof(char));
	}
}

void sendResult(SOCKET sock, int playerNextPosition, int applePosition, int bombPosition)
{
	char sendString[12];

	*((int*)sendString) = playerNextPosition;
	*((int*)sendString + 1) = applePosition;
	*((int*)sendString + 2) = bombPosition;

	send(sock, sendString, 12, 0);
}

// 0 is wall, 1 is fine

int recvPicture(SOCKET sock, int flags, int idx)
{
	int recvLen, tmpLen, isFine;
	int iter = 0;
	char recvTmpChar[4];

	recv(sock, recvTmpChar, 4, flags);

	isFine = *((int*)recvTmpChar);

	recv(sock, recvTmpChar, 4, flags);

	recvLen = *((int*)recvTmpChar);

	area[idx].isWall = !isFine;
	area[idx].pictureLen = recvLen;

	if (!isFine)
		return -1;

	while (1)
	{
		if (iter >= recvLen)
			break;

		tmpLen = recv(sock, (area[idx].picture_png) + iter, recvLen - iter, flags);

		if (tmpLen == -1)
			continue;

		iter = iter + tmpLen;
	}

	(area[idx].picture_png)[recvLen] = 0;

	vector<uchar> trans;

	trans.assign(area[idx].picture_png, area[idx].picture_png + area[idx].pictureLen);
	area[idx].picture_rgba = imdecode(InputArray(trans), CV_8SC4);

	trans.clear();

	return recvLen;
}

void recvResult(SOCKET sock)
{
	for (int i = 0; i < MAX_SIZE_AREA; i++)
		recvPicture(sock, 0, i);
}

void AI(SOCKET sock)
{
	int playerNextPosition;
	int bombPosition;
	int applePosition;

	aiInit();

	while (1)
	{
		recvResult(sock);

		aiCode(playerNextPosition, applePosition, bombPosition);

		sendResult(sock, playerNextPosition, applePosition, bombPosition);
	}
}

void aiCode(int &playerNextPosition, int &applePosition, int &bombPosition)
{
	playerNextPosition = rand() % MAX_MOVE_AREA;
	applePosition = (rand() * 2) % MAX_SIZE_AREA;
	bombPosition = (rand() * 4) % MAX_SIZE_AREA;
}

vector<double> featureDescript(Mat& m) {
	vector<double> ret;

	/*-----don't touch-----*/

	/*
	you have to fill in here with a code 
	that make a feature vector of image m
	*/

	/*-----don't touch-----*/

	return ret;
}

int classify(Mat example, vector<pair<vector<double>, int> > &training, int nb_class) {
	const int k = 20;
	int predict = rand() % 4;

	/*-----don't touch-----*/

	/*
	you have to fill in here
	with a code that classify example class
	the class is 0 : apple, 1 : bomb, 2 : player, 3 : background
	you have to make a kNN code to classify and don't use other machine learning algorithm
	*/

	/*-----don't touch-----*/

	return predict;
}

vector<pair<int, int> > predict(vector<pair<Mat, int> > sample, vector<pair<vector<double>, int> > model, int nb_class) {
	vector<pair<int, int> > ret;

	for (auto example : sample) {
		int pd_res = classify(example.first, model, nb_class);
		ret.push_back({ example.second, pd_res });
	}

	return ret;
}

char* next_pos(vector<pair<Mat, int> > input, vector<pair<vector<double>, int> > train, int nb_class) {
	char ret[12];
	vector<pair<int, int> > result = predict(input, train, nb_class);
	int next = -1;
	/*-----don't touch-----*/

	/*
	fill here and decide the next movement
	in the vector result, the first element is the coordinate and the next pair is {predicted class, distance}
	you have to decide the next coordinate - 'next'
	*/

	/*-----don't touch-----*/
	*((int*)ret) = next;
	*((int*)ret + 1) = 3;
	*((int*)ret + 2) = 4;

	return ret;
}


float model_evaluate(vector<pair<Mat, int> > training, int nb_class) {
	float error = 0.0;
	const int k = 11;
	vector<pair<vector<double>, int> > images;
	vector<vector<int> > k_fold(k + 1);
	vector<bool> check(training.size(), false);
	int sz = training.size() / k;

	for (auto i : training)
		images.push_back({ featureDescript(i.first), i.second });

	for (int i = 0; i < k; i++) {
		for (int j = 0; j < sz; j++) {
			int next = rand() % training.size();

			if (check[next]) {
				j--;
				continue;
			}

			k_fold[i].push_back(next);
			check[next] = true;
		}
	}

	for (int i = 0; i < k; i++) {
		vector<pair<Mat, int> > test;
		vector<pair<vector<double>, int> > train;

		for (int j = 0; j < training.size(); j++) {
			bool check = false;
			for (auto next : k_fold[i])
				if (next == j) {
					check = true;
					break;
				}

			if (check)
				test.push_back(training[j]);
			else
				train.push_back({ images[j].first, images[j].second });

		}

		int result = 0;
		for (auto here : test) {
			int res = classify(here.first, train, nb_class);
			if (here.second != res)
				result++;
		}

		error += result;
	}

	return (float)error / (k * sz);
}

bool input(vector<pair<Mat, int> > &list, string folder, int cs) {
	HANDLE hFind;
	WIN32_FIND_DATA FindData;
	char path[255];

	string make_path = ".\\" + folder + "\\*.*";
	string realPath = ".\\" + folder + "\\";

	strcpy(path, make_path.c_str());

	hFind = FindFirstFile((LPCSTR)path, &FindData);

	if (hFind == INVALID_HANDLE_VALUE)
		return 0;

	do
	{
		if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, ".."))
			continue;

		string name(FindData.cFileName);
		Mat file = imread(realPath + name, -1);

		list.push_back({ file, cs });

	} while (FindNextFile(hFind, &FindData));

	FindClose(hFind);

	return 1;
}