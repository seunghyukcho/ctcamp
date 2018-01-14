#include"client.h"
#include"ai.h"

int main()
{
	int nb_class = 4;
	srand(time(NULL));
	vector<pair<Mat, int> > train_set;

	cout << "\n\nLOADING IMAGE...\n";

	for (int i = 0; i < nb_class; i++)
	{
		string path = to_string(i);
		if (!input(train_set, path, i)) {
			cout << "error!\n";
			return 0;
		}
	}

	cout << "SUCCESS\n\n";

	cout << "YOUR MODEL ACCURACY IS : ";
	cout << (1 - model_evaluate(train_set, nb_class)) * 100 << "%\n\n\n";

	system("pause");

	return 0;
}