#include "Main.h"

Project::Project()
	: std_includes(),
	  functions2(),
	  m_BackgroundState(0),
	  m_LastPid(0),
	  m_ProcessStartedTime(0),
	  m_ProcessEndedTime(0),
	  m_ExpiredOutput(false),
	  m_PipeFd{-1, -1},
	  m_BackgroundThread(&BackgroundThread, this, "Background")
{
}

Project::~Project()
{
	// Close pipes
	{
		close(m_PipeFd[0]);
		close(m_PipeFd[1]);

		// Kill child processes
		if (0 != m_LastPid)
		{
			std::cout << "Sending kill to last child pid " << m_LastPid << ".\n";
			//auto Result = kill(0, SIGTERM);
			auto Result = killpg(m_LastPid, SIGKILL);
			//waitpid(m_LastPid, NULL, 0);

			if (0 != Result) {
				std::cerr << "Error: kill() failed with return " << Result << ", errno " << errno << ".\n";
				//throw 0;
			}
		}
	}
}

void Project::LoadSampleGenProgram(CanvasWidget & CanvasTEST)
{
	std_includes.push_back(FindConcept("iostream"));
	std_includes.push_back(FindConcept("list"));
	std_includes.push_back(FindConcept("vector"));

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

void Project::GenerateProgram(const std::string & ProgramContent = "")
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

#if DECISION_USE_CPP_INSTEAD_OF_GO
	std::ofstream Out("./GenProgram.cpp");
#else
	std::ofstream Out("./GenProgram.go");
#endif

	Out << ProgramContent;
}

void Project::GenerateProgramForFunction(std::ostream & Out, const std::string & InputContent, const std::string & FunctionContent, std::vector<ConceptId> & Imports)
{
	Out <<
	"package main""\n"
	"""\n"
	"import (""\n"
	"	\"fmt\"""\n";
	for (auto & Import : Imports)
	{
		Out << "	\"" + GetConcept(Import).GetContent() + "\"""\n";
	}
	Out << ")""\n"
	"""\n"
	<< FunctionContent << "\n"
	"""\n"
	"func MyGetString(args ...interface{}) string {""\n"
	"	var str string""\n"
	"	for index, arg := range args {""\n"
	"		str = str + fmt.Sprintf(\"%#v\", arg) //VariableToGoSyntaxFormatted(arg)""\n"
	"		if (len(args) - 1 != index) {""\n"
	"			str = str + \", \"""\n"
	"		}""\n"
	"	}""\n"
	"	return str""\n"
	"}""\n"
	"""\n"
	"func main() {""\n"
	"	a := "
	<< InputContent << "\n"
	"""\n"
	"	out := MyGetString(MySort(a))""\n"
	"	in_after := MyGetString(a)""\n"
	"""\n"
	"	fmt.Printf(\"%s; %s\", in_after, out)""\n"
	"}";
}

volatile pid_t LastPid = 0;

#if 0
void GLFWCALL RunProgramThread(void * Parameter)
{
	// TODO: Figure out if I can clean up the temporary files after execution finishes (i.e. by waiting for ./GenProgram.command process to finish)
	//auto Command = "clang++ ./GenProgram.cpp -o ./GenProgram.command ; open ./GenProgram.command ; sleep 1 ; rm ./GenProgram.cpp ./GenProgram.command";
	//auto Command = "/usr/local/go/bin/go run ./GenProgram.go > out.txt";// ; sleep 1 ; rm ./GenProgram.go";
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
	//std::system(Command);

	TextFieldWidget * OutputWidget = static_cast<TextFieldWidget *>(Parameter);

	{
		if (0 != LastPid)
		{
			std::cout << "Sending kill to last child pid " << LastPid << ".\n";
			//auto Result = kill(0, SIGTERM);
			auto Result = killpg(LastPid, SIGKILL);
			waitpid(LastPid, NULL, 0);

			if (0 != Result) {
				std::cerr << "Error: kill() failed with return " << Result << ", errno " << errno << ".\n";
				//throw 0;
			}
		}

		int pipefd[2];
		pipe(pipefd);

		auto pid = fork();

		if (0 == pid)
		{
			close(pipefd[0]);    // close reading end in the child

			dup2(pipefd[1], 1);  // send stdout to the pipe
			dup2(pipefd[1], 2);  // send stderr to the pipe

			close(pipefd[1]);    // this descriptor is no longer needed

			//execl("/bin/echo", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
			//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
			//execl("/usr/local/go/bin/go", "go", "version", (char *)0);
			execl("/usr/local/go/bin/go", "go", "build", "/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/GenProgram.go", (char *)0);

			//exit(1);		// Not needed, just in case I comment out the above
		}
		else if (-1 == pid)
		{
			std::cout << "Error forking.\n";
		}
		else
		{
			LastPid = pid;

			std::cout << "Before: " << getpgid(pid) << ".\n";
			//setpgrp();
			setpgid(pid, pid);
			std::cout << "After: " << getpgid(pid) << ".\n";

			std::string str;

			std::cout << "In parent, created pid " << pid << ".\n";

			//OutputWidget->SetBackground(Color(0.9, 0.9, 0.9));

			close(pipefd[1]);  // close the write end of the pipe in the parent

			char buffer[1024];
			ssize_t n;
			while (0 != (n = read(pipefd[0], buffer, sizeof(buffer))))
			{
				if (-1 == n)
					std::cout << "Error: " << errno << std::endl;
				else
				{
					str.append(buffer, n);
					OutputWidget->SetContent(str);
				}
			}

			// Wait for child process to complete
			{
				//sleep(3);
				int status;
				waitpid(pid, &status, 0);
				LastPid = 0;

				std::cout << "Child finished with status " << status << ".\n";

				// If killed, just return
				if (   WIFSIGNALED(status)
					&& 9 == WTERMSIG(status))
				{
					return;
				}

				uint8 Status = static_cast<uint8>(status >> 8);

				if (0 == Status)
					OutputWidget->SetBackground(Color(1.0, 1, 1));
				else
					OutputWidget->SetBackground(Color(1.0, 0, 0));
			}
		}
	}
}
#endif

void Project::RunProgram(TextFieldWidget * OutputWidget)
{
#ifdef WIN32
	STARTUPINFO StartupInfo;
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	PROCESS_INFORMATION ProcessInfo;
	memset(&ProcessInfo, 0, sizeof(ProcessInfo));

	LPTSTR CommandLine = "cmd.exe /C \"C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\vcvarsall.bat\" amd64 & (cl GenProgram.cpp >NUL 2>NUL && GenProgram || echo Compilation error.) & pause & del GenProgram.*";
	auto Result = CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo);
#elif (defined(__APPLE__) && defined(__MACH__)) || defined(__linux)
	//glfwCreateThread(&RunProgramThread, OutputWidget);
#endif
}

void GLFWCALL Project::BackgroundThread(void * Argument)
{
	Thread * Thread = Thread::GetThisThreadAndRevertArgument(Argument);

	auto Project = static_cast<class Project *>(Argument);

	std::string GoPath = "GOPATH=";
	// TODO: Move this elsewhere?!
	// Get current working directory
	{
		auto cwd = getcwd(nullptr, 0);
		if (nullptr != cwd) {
			//printf("Using cwd = >%s<\n", cwd);
			GoPath += cwd;
			free(cwd);
		}
	}
	GoPath += "/GoLand";

	// Main loop
	while (Thread->ShouldBeRunning())
	{
		//std::cout << "Sleeping in background thread " << glfwGetTime() << ".\n";
		//glfwSleep(0.5);
		glfwSleep(0.001);

		if (0 == Project->m_BackgroundState)
			continue;

		TextFieldWidget * OutputWidget = g_OutputWidget;

		Project->m_BackgroundState = 2;
		OutputWidget->SetBackground(Project->m_CompilingColor);

		/*auto PipeFd = App->m_PipeFd[1];
		auto Write = [&](std::string String) {
			//OutputWidget->AppendContent(String);
			write(PipeFd, String.c_str(), String.length());
		};*/

		/*Write("Compiling ");
		for (int i = 0; i < 15; ++i) {
			Write(".");
			glfwSleep(0.2);
		}
		Write(" Done.\n");*/

		// Clean up temporary files
#if (defined(__APPLE__) && defined(__MACH__)) || defined(__linux)
		system("rm ./GenProgram");		// TODO: Error check?
#endif

		close(Project->m_PipeFd[1]);		// Close the write end of the pipe in the parent

		pipe(Project->m_PipeFd);
		fcntl(Project->m_PipeFd[0], F_SETFL, O_NONBLOCK);
		std::cout << "opened " << Project->m_PipeFd[0] << " and " << Project->m_PipeFd[1] << ".\n";

		uint8 ProcessResult;

		{
			Project->m_LastPid = fork();

			if (0 == Project->m_LastPid)
			{
				close(Project->m_PipeFd[0]);    // close reading end in the child

				dup2(Project->m_PipeFd[1], 1);  // send stdout to the pipe
				dup2(Project->m_PipeFd[1], 2);  // send stderr to the pipe

				close(Project->m_PipeFd[1]);    // this descriptor is no longer needed

				//execl("/bin/echo", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/usr/local/go/bin/go", "go", "version", (char *)0);
#if DECISION_USE_CPP_INSTEAD_OF_GO
				execl("/usr/bin/clang++", "/usr/bin/clang++", "./GenProgram.cpp", "-o", "./GenProgram", (char *)0);
#else
				putenv(const_cast<char *>(GoPath.c_str()));		// HACK: Const cast
				execl("/usr/local/go/bin/go", "/usr/local/go/bin/go", "build", "./GenProgram.go", (char *)0);
#endif

				//exit(1);		// Not needed, just in case I comment out the above
				throw;		// Needed to stop "ProcessResult is uninitialized here" warning
			}
			else if (-1 == Project->m_LastPid)
			{
				std::cerr << "Error forking.\n";
				ProcessResult = 1;
			}
			else
			{
				std::cout << "Before: " << getpgid(Project->m_LastPid) << ".\n";
				//setpgrp();
				setpgid(Project->m_LastPid, Project->m_LastPid);
				std::cout << "After: " << getpgid(Project->m_LastPid) << ".\n";

				std::cout << "In parent, created pid " << Project->m_LastPid << ".\n";

				//OutputWidget->SetBackground(Color(0.9, 0.9, 0.9));

				// Wait for child process to complete
				{
					int status;
					waitpid(Project->m_LastPid, &status, 0);
					Project->m_LastPid = 0;

					std::cout << "Child finished with status " << status << ".\n";

					// If killed, just skip
					if (   WIFSIGNALED(status)
						&& 9 == WTERMSIG(status))
					{
						continue;
					}

					ProcessResult = static_cast<uint8>(status >> 8);
				}

				// If successful return value, double check the output file exists
				if (0 == ProcessResult)
				{
					auto Path = std::string("./GenProgram");

					auto List = Ls(Path);
					if (   1 == List.size()
						&& List.front() == "ls: " + Path + ": No such file or directory")
					{
						ProcessResult = 1;

						write(Project->m_PipeFd[1], List.front().c_str(), List.front().length());
						close(Project->m_PipeFd[1]);
					}
				}
			}
		}

		if (0 == ProcessResult) {
			OutputWidget->SetBackground(Project->m_RunningColor);

			// HACK: This is dangerous, shouldn't modify OutputWidget contents from this thread, should send a signal to main thread, or use a mutex (but too lazy ATM to add all the code for a mutex)
			/*if (Project->m_ExpiredOutput)
			{
				OutputWidget->SetContent("");
				Project->m_ExpiredOutput = false;
			}*/
		} else {
			OutputWidget->SetBackground(Project->m_ErrorCompileColor);
			Project->m_ProcessEndedTime = glfwGetTime();
			Project->m_BackgroundState = 0;
		}

		if (2 != Project->m_BackgroundState)
			continue;

		{
			Project->m_LastPid = fork();

			if (0 == Project->m_LastPid)
			{
				close(Project->m_PipeFd[0]);    // close reading end in the child

				dup2(Project->m_PipeFd[1], 1);  // send stdout to the pipe
				dup2(Project->m_PipeFd[1], 2);  // send stderr to the pipe

				close(Project->m_PipeFd[1]);    // this descriptor is no longer needed

				//execl("/bin/echo", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/Users/Dmitri/Dmitri/^Work/^GitHub/Conception/print-args", "echo", "-n", "hello", "there,", "how are you?", (char *)0);
				//execl("/usr/local/go/bin/go", "go", "version", (char *)0);
				execl("./GenProgram", "./GenProgram", (char *)0);
				//execl("./gocode", "gocode", "-in=./GenProgram.go", "autocomplete", "./GenProgram.go", std::to_string(App->m_SourceWidget->GetCaretPosition()).c_str(), (char *)0);

				//exit(1);		// Not needed, just in case I comment out the above
			}
			else if (-1 == Project->m_LastPid)
			{
				std::cerr << "Error forking.\n";
				ProcessResult = 1;
			}
			else
			{
				std::cout << "Before: " << getpgid(Project->m_LastPid) << ".\n";
				//setpgrp();
				setpgid(Project->m_LastPid, Project->m_LastPid);
				std::cout << "After: " << getpgid(Project->m_LastPid) << ".\n";

				std::cout << "In parent, created pid " << Project->m_LastPid << ".\n";

				// Wait for child process to complete
				{
					int status;
					waitpid(Project->m_LastPid, &status, 0);
					Project->m_LastPid = 0;

					std::cout << "Child finished with status " << status << ".\n";

					// If killed, just skip
					if (   WIFSIGNALED(status)
						&& 9 == WTERMSIG(status))
					{
						continue;
					}

					ProcessResult = static_cast<uint8>(status >> 8);
				}
			}
		}

		/*uint8 Char = '\0';
		write(App->m_PipeFd[1], &Char, 1);*/

		// HACK: This is dangerous, shouldn't modify OutputWidget contents from this thread, should send a signal to main thread, or use a mutex
		if (Project->m_ExpiredOutput)
		{
			OutputWidget->SetContent("");
			Project->m_ExpiredOutput = false;
		}

		//close(App->m_PipeFd[0]);		// Close the read end of the pipe in the parent
		//close(App->m_PipeFd[1]);		// Close the write end of the pipe in the parent

		if (0 == ProcessResult) {
			OutputWidget->SetBackground(Project->m_FinishedSuccessColor);
		} else {
			OutputWidget->SetBackground(Project->m_FinishedErrorColor);
		}

		if (2 == Project->m_BackgroundState) {
			Project->m_ProcessEndedTime = glfwGetTime();
			Project->m_BackgroundState = 0;
		}
	}

	Thread->ThreadEnded();
}

void Project::StartBackgroundThread()
{
	m_BackgroundThread.Start();
}
