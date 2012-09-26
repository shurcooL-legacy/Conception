#include "Main.h"

Project::Project()
	: std_includes(),
	  functions2()
{
}

Project::~Project()
{
}

void Project::LoadSampleGenProgram(Canvas & CanvasTEST)
{
	std_includes.push_back(FindConcept("iostream"));

	{
		Function printhi_func2;
		printhi_func2.Position = Vector2n(-450, -200);
		printhi_func2.name.push_back(17);
		printhi_func2.args.push_back(FindConcept(""));
		printhi_func2.return_type.push_back(FindConcept("void"));

		printhi_func2.body_lines.push_back(1);
		printhi_func2.body_lines.push_back(FindConcept("<<"));
		printhi_func2.body_lines.push_back(3);
		printhi_func2.body_lines.push_back(FindConcept("<<"));
		printhi_func2.body_lines.push_back(FindConcept("\" & size of pointer == \""));
		printhi_func2.body_lines.push_back(FindConcept("<<"));
		printhi_func2.body_lines.push_back(FindConcept("sizeof"));
		printhi_func2.body_lines.push_back(FindConcept("("));
		printhi_func2.body_lines.push_back(FindConcept("void"));
		printhi_func2.body_lines.push_back(FindConcept("*"));
		printhi_func2.body_lines.push_back(FindConcept(")"));
		printhi_func2.body_lines.push_back(FindConcept("<<"));
		printhi_func2.body_lines.push_back(10);
		printhi_func2.body_lines.push_back(FindConcept(";"));

		functions2.push_back(printhi_func2);
	}

	{
		Function main_func2;
		main_func2.Position = Vector2n(0, -300);
		main_func2.name.push_back(FindConcept("main"));
		main_func2.args.push_back(FindConcept("int"));
		main_func2.args.push_back(FindConcept("argc"));
		main_func2.args.push_back(FindConcept(","));
		main_func2.args.push_back(FindConcept("char"));
		main_func2.args.push_back(FindConcept("*"));
		main_func2.args.push_back(FindConcept("argv"));
		main_func2.args.push_back(FindConcept("["));
		main_func2.args.push_back(FindConcept("]"));
		main_func2.return_type.push_back(FindConcept("int"));

		main_func2.body_lines.push_back(17);
		main_func2.body_lines.push_back(FindConcept("("));
		main_func2.body_lines.push_back(FindConcept(")"));
		main_func2.body_lines.push_back(FindConcept(";"));
		main_func2.body_lines.push_back(FindConcept("return"));
		main_func2.body_lines.push_back(FindConcept("0"));
		main_func2.body_lines.push_back(FindConcept(";"));

		functions2.push_back(main_func2);
	}

#if 0
	for (auto & Function : functions2)
	{
		CanvasTEST.AddWidget(new FunctionWidget(Function.Position, Function));
	}
#endif
}

void Project::GenerateProgram(std::string ProgramContent = "")
{
	/*std::ofstream Out("GenProgram.cpp");

	for (uint32 i = 0; i < std_includes.size(); ++i)
	{
		Out << "#include <" << GetConcept(std_includes.at(i)) << ">" << endl;
	}

	for (uint32 i = 0; i < functions2.size(); ++i)
	{
		Out << endl;

		Function & f = functions2.at(i);

		Out << f;
	}

	Out.close();*/

	std::ofstream Out("./GenProgram.go");

	Out << ProgramContent;
}

void GLFWCALL RunProgramThread(void *)
{
	// TODO: Figure out if I can clean up the temporary files after execution finishes (i.e. by waiting for ./GenProgram.command process to finish)
	//auto Command = "clang++ ./GenProgram.cpp -o ./GenProgram.command ; open ./GenProgram.command ; sleep 1 ; rm ./GenProgram.cpp ./GenProgram.command";
	auto Command = "/usr/local/go/bin/go run ./GenProgram.go > out.txt";// ; sleep 1 ; rm ./GenProgram.go";
	//auto Command = "go run GenProgram.go > out.txt";
	//auto Command = "echo -n conc > out.txt";
	//auto Command = "./print-args -n \"Conc eption\" > out.txt";
	//auto Command = "go run launcher.go";
	//auto Command = "./launcher";
	//auto Command = "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/launcher";
	//auto Command = "touch /Users/Dmitri/Dmitri/^Work/^GitHub/Conception/out3_Conc.txt";
	//auto Command = "open -a Chess.app";

	//std::cout << "Launching " << Command << std::endl;
	//throw 0;
	std::system(Command);
}

std::string Project::RunProgram()
{
	std::string str;

#ifdef WIN32
	STARTUPINFO StartupInfo;
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	PROCESS_INFORMATION ProcessInfo;
	memset(&ProcessInfo, 0, sizeof(ProcessInfo));

	LPTSTR CommandLine = "cmd.exe /C \"C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\vcvarsall.bat\" amd64 & (cl GenProgram.cpp >NUL 2>NUL && GenProgram || echo Compilation error.) & pause & del GenProgram.*";
	auto Result = CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo);
#elif defined(__APPLE__) && defined(__MACH__)
	//glfwCreateThread(&RunProgramThread, nullptr);
	//system("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/launcher");
	if (1)
	{
		int pipefd[2];
		pipe(pipefd);

		auto pid = fork();

		if (0 == pid)
		{
			std::cout << "Child process!\n";
			//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/launcher", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/launcher", (char *)0);
			//execl("/usr/bin/touch", "touch", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/out4_ffs.txt", (char *)0);
			//execl("/usr/local/go/bin/go", "go", "run", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/launcher.go", (char *)0);
			//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "print-args", "run", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/launcher.go", (char *)0);

			close(pipefd[0]);    // close reading end in the child

			dup2(pipefd[1], 1);  // send stdout to the pipe
			dup2(pipefd[1], 2);  // send stderr to the pipe

			close(pipefd[1]);    // this descriptor is no longer needed

			//execl("/bin/echo", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
			//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
			//execl("/usr/local/go/bin/go", "go", "version", (char *)0);
			execl("/usr/local/go/bin/go", "go", "run", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/GenProgram.go", (char *)0);

			//exit(1);		// Not needed, just in case I comment out the above
		}
		else if (-1 == pid)
		{
			std::cout << "Error forking.\n";
		}
		else
		{
			std::cout << "In parent, created pid " << pid << ".\n";

			char buffer[1024];

			close(pipefd[1]);  // close the write end of the pipe in the parent

			//sleep(100000);
			//sleep(1);

			ssize_t n;
			while (0 != (n = read(pipefd[0], buffer, sizeof(buffer))))
			{
				if (-1 == n)
					std::cout << "Error: " << errno << std::endl;
				else
				{
					std::cout << "Read " << n << " chars from pipe: >";
					for (int i = 0; i < n; ++i) {
						std::cout << buffer[i];
					}
					std::cout << "<\n";

					str.append(buffer, n);
				}
			}

			//sleep(3);

			std::cout << "Done in parent!\n";
		}
	}
	/*else
	{
		//system("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args I dont know\\ whatever");
		auto Result = system("/usr/local/go/bin/go run /Users/Dmitri/Desktop/launcher.go");
		std::cout << "System returns " << Result << endl;
	}*/
#endif

	return str;
}
