#include <iostream>

using namespace std;

int main(int argc, const char * argv[])
{
	//cout << (unsigned char)(65) << endl;

	cout << "Test.\n\n";

	int fd[2];

	{
		pipe(fd);

		cout << fd[0] << " and " << fd[1] << "\n";//xxxxxxxxxxxxxxxxyzxxxxxxxxxxx
	}

	close(fd[0]);
	close(fd[1]);

	//sleep(10);

	//cout << endl;

	{
		pipe(fd);

		cout << fd[0] << " and " << fd[1] << "\n";
	}

	close(fd[0]);
	close(fd[1]);

	return 0;
}