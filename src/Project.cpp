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

void Project::GenerateProgram()
{
	std::ofstream Out("GenProgram.cpp");

	for (uint32 i = 0; i < std_includes.size(); ++i)
	{
		Out << "#include <" << Concepts[std_includes.at(i)] << ">" << endl;
	}

	for (uint32 i = 0; i < functions2.size(); ++i)
	{
		Out << endl;

		Function & f = functions2.at(i);

		Out << f;
	}

	Out.close();
}

void GLFWCALL RunProgramThread(void *)
{
	// TODO: Figure out if I can clean up the temporary files after execution finishes (i.e. by waiting for ./GenProgram.command process to finish)
	auto Command = "clang++ GenProgram.cpp -o ./GenProgram.command ; open ./GenProgram.command ; sleep 1 ; rm ./GenProgram.cpp ./GenProgram.command";

	system(Command);
}

void Project::RunProgram()
{
#ifdef WIN32
	STARTUPINFO StartupInfo;
	memset(&StartupInfo, 0, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);

	PROCESS_INFORMATION ProcessInfo;
	memset(&ProcessInfo, 0, sizeof(ProcessInfo));

	LPTSTR CommandLine = "cmd.exe /C \"C:\\Program Files (x86)\\Microsoft Visual Studio 11.0\\VC\\vcvarsall.bat\" amd64 & (cl GenProgram.cpp >NUL 2>NUL && GenProgram || echo Compilation error.) & pause & del GenProgram.*";
	auto Result = CreateProcess(NULL, CommandLine, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &StartupInfo, &ProcessInfo);
#elif defined(__APPLE__) && defined(__MACH__)
	glfwCreateThread(&RunProgramThread, nullptr);
#endif
}
