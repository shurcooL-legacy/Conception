#include <iostream>
#include <string>

using namespace std;

int main(int argc, const char * argv[])
{
	std::string Output = "hi\nhey\nbye";
	
	// Find clear code, cater to it
	{
		//const char ClearCode[] = { 0x1B, 0x5B, 0x48, 0x1B, 0x5B, 0x32, 0x4A };
		const char ClearCode[] = { 'h', 'e', 'y' };

	printf(">size of %d<\n", sizeof(ClearCode));
		auto n = Output.rfind(ClearCode, std::string::npos, sizeof(ClearCode));
	printf(">%d<\n", n);

		if (std::string::npos != n)
		{
			Output = Output.substr(n);
		}
	}
	
	cout << Output;

	return 0;
}