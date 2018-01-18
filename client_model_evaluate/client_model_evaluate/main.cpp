#include"client.h"

#include"ai.h"

int main()
{
	int caseNumber;
	int nb_class = 4;

	srand(time(NULL));
	vector<pair<Mat, int> > train_set;

	socketSet socketInfo;

	cout << "MENU (1: testing, 2: network)\n" << endl << "> ";

	cin >> caseNumber;

	switch (caseNumber)
	{
	case 1:
		cout << "\n\nLOADING IMAGE...\n";
		for (int i = 0; i < nb_class; i++)
		{
			string path = to_string(i);
			if (!input(train_set, path, i))
				std::cout << "error" << endl;
		}
		cout << "COMPLETE!\n\n";
		cout << "EVALUATING...\n";

		cout << "ACCURACY : " << (1 - model_evaluate(train_set, nb_class)) * 100 << "%" << "\n\n\n";

		break;

	case 2:
		initClient(&socketInfo);
		runClient(&socketInfo);
		endClient(&socketInfo);

		break;
	}

	system("pause");

	return 0;
}