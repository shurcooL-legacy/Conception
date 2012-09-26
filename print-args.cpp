//#include <iostream>
#include <fstream>

int main(int argc, char * argv[])
{
	std::ofstream out("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/out.txt");

	for (int i = 0; i < argc; ++i)
	{
		out << i << ": >" << argv[i] << "<\n";
	}

	return 0;
}
