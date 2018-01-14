#include"client.h"
#include"ai.h"

int main()
{
	int nb_class = 2;
	srand(time(NULL));
	vector<pair<Mat, int> > train_set;

	for (int i = 0; i < nb_class; i++)
	{
		string path = to_string(i);
		if (!input(train_set, path, i))
			cout << "error!\n";
	}

	cout << 1 - model_evaluate(train_set, nb_class) << "\n";

	while (1);

	return 0;
}