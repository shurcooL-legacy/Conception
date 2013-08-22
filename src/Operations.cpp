#include "Main.h"

// Really stupid hardcoded method to parse Json for id field, enough to hold me over until Golang reimplementation
const std::string ParseGistId(const std::string & JsonResponse)
{
	std::string FirstMarkerString = "\"id\": \"";
	auto FirstMarker = JsonResponse.find(FirstMarkerString);
	if (std::string::npos == FirstMarker) throw 0;
	auto SecondMarker = JsonResponse.find('\"', FirstMarker + FirstMarkerString.length());
	if (std::string::npos == SecondMarker) throw 0;
	return JsonResponse.substr(FirstMarker + FirstMarkerString.length(), SecondMarker - (FirstMarker + FirstMarkerString.length()));
}

bool IsFileTrackedByGit(const std::string Path)
{
	const std::string Folder = ParsePath(Path, 0);
	const std::string Filename = ParsePath(Path, 1);

	auto Shell = std::unique_ptr<ShellWidget>(new ShellWidget(Vector2n::ZERO, *static_cast<TypingModule *>(nullptr)));
	std::string Command = "cd \'" + Folder + "\'\ngit ls-files --error-unmatch -- \'" + Filename + "\' > /dev/null 2> /dev/null\necho -n $?";
	Shell->m_CommandWidget->SetContent(Command);
	Shell->m_ExecuteWidget->GetAction()();
	return "0" == Shell->m_OutputWidget->GetContent();
}

std::function<Color(uint32, const std::string &)> GetLineHighlighting()
{
	return [](uint32 LineNumber, const std::string & Line) -> Color
	{
		if (Line.length() >= 1 && Line[0] == '+') return Color(0.9, 1, 0.9);
		else if (Line.length() >= 1 && Line[0] == '-') return Color(1.0, 0.9, 0.9);
		else return Color::WHITE;
	};
}

std::string ParsePath(const std::string Path, uint8 Mode)
{
	auto Separator = Path.find_last_of('/');
	if (0 == Separator) throw 0;		// TODO: Handle "/rooted_paths" properly
	std::string Folder = std::string::npos != Separator ? Path.substr(0, Separator) : "./";
	std::string Filename = std::string::npos != Separator ? Path.substr(Separator + 1) : Path;

	if (0 == Mode) return Folder;
	else if (1 == Mode) return Filename;
	else throw 0;
}

std::function<std::vector<std::string>(std::string::size_type)> GetAutocompletions(const std::string Path)
{
	return [Path](std::string::size_type CaretPosition) -> std::vector<std::string>
	{
		std::vector<std::string> Autocompletions;

#if 0
		Autocompletions = { "Line 1", "Line 2", "Line 3" };
#else
		// Get autocompletion using gocode
		std::string Output = "";
		{
			int PipeFd[2];
			pipe(PipeFd);
			fcntl(PipeFd[0], F_SETFL, O_NONBLOCK);
			//std::cout << "gocode: Opened " << PipeFd[0] << " and " << PipeFd[1] << ".\n";

			uint8 ProcessResult;

			{
				auto Pid = fork();

				if (0 == Pid)
				{
					close(PipeFd[0]);    // close reading end in the child

					dup2(PipeFd[1], 1);  // send stdout to the pipe
					dup2(PipeFd[1], 2);  // send stderr to the pipe

					close(PipeFd[1]);    // this descriptor is no longer needed

					// TODO: Should I be using the ./GenProgram.go file or something else?
					execl("./bin/gocode/gocode", "./bin/gocode/gocode", "-f=nice", ("-in=" + Path).c_str(), "autocomplete", Path.c_str(), std::to_string(CaretPosition).c_str(), (char *)0);

					//exit(1);		// Not needed, just in case I comment out the above
					throw 0;
				}
				else if (-1 == Pid)
				{
					perror("Error forking");
					throw 0;
				}
				else
				{
					// Wait for child process to complete
					{
						int status;
						waitpid(Pid, &status, 0);
						Pid = 0;

						//std::cout << "Child finished with status " << status << ".\n";

						ProcessResult = static_cast<uint8>(status >> 8);
					}

					// Read output from pipe and put it into Output
					if (0 == ProcessResult)
					{
						char buffer[1024];
						ssize_t n;
						while (0 != (n = read(PipeFd[0], buffer, sizeof(buffer))))
						{
							if (-1 == n) {
								if (EAGAIN == errno) {
									break;
								} else {
									std::cerr << "Error: Reading from pipe " << PipeFd[0] << " failed with errno " << errno << ".\n";
									break;
								}
							}
							else
							{
								Output.append(buffer, n);
							}
						}
					}
				}
			}

			close(PipeFd[0]);
			close(PipeFd[1]);
		}

		// Parse Output and populate Autocompletions
		// TODO: Clean up
		{
			std::stringstream ss;
			ss << Output;
			std::string Line;

			std::getline(ss, Line);		// Skip first line
			std::getline(ss, Line);
			while (!Line.empty() && !ss.eof())
			{
				Autocompletions.push_back(Line);
				std::getline(ss, Line);
			}
			if (!Line.empty())
				Autocompletions.push_back(Line);
		}
#endif

		// TODO: Ability to insert stuff from autocompletions list
		Autocompletions.push_back("This feature is not finished.");

		return Autocompletions;
	};
}

void SkipFirstLines(std::string & InOut, uint32 LinesToSkip)
{
	std::string::size_type FoundNewline = 0;
	while (LinesToSkip-- >= 1)
	{
		FoundNewline = InOut.find('\n', FoundNewline);
		if (std::string::npos == FoundNewline)
			break;
		else
			++FoundNewline;
	}

	if (std::string::npos == FoundNewline)
		InOut = "";
	else if (0 != FoundNewline)
	{
		InOut = InOut.substr(FoundNewline);
	}
}

std::string TrimFirstSpace(const std::string & In)
{
	if (   In.length() >= 1
		&& In[0] == ' ')
		return In.substr(1);
	else
		return In;
}

// Trim last newline, if there is one
void TrimLastNewline(std::string & InOut)
{
	if (   InOut.size() >= 1
		&& InOut.back() == '\n')
	{
		InOut.pop_back();
	}
}

std::string Diff(const std::string & Content1, const std::string & Content2)
{
	WriteToFile("./GenDiff1.txt", Content1);
	WriteToFile("./GenDiff2.txt", Content2);

	std::string Output = "";
	{
		int PipeFd[2];			// Pipe for reading from child's stdout+stderr
		pipe(PipeFd);
		fcntl(PipeFd[0], F_SETFL, O_NONBLOCK);

		uint8 ProcessResult;

		{
			auto Pid = fork();

			if (0 == Pid)
			{
				close(PipeFd[0]);    // close reading end in the child

				dup2(PipeFd[1], 1);  // send stdout to the pipe
				dup2(PipeFd[1], 2);  // send stderr to the pipe

				close(PipeFd[1]);    // this descriptor is no longer needed

				execl("/usr/bin/diff", "/usr/bin/diff", "--unified", "./GenDiff1.txt", "./GenDiff2.txt", (char *)0);

				// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
				//exit(1);		// Not needed, just in case I comment out the above
				throw 0;
			}
			else if (-1 == Pid)
			{
				perror("Error forking");
				throw 0;
			}
			else
			{
				// Wait for child process to complete
				{
					int status;
					waitpid(Pid, &status, 0);
					Pid = 0;

					//std::cout << "Child finished with status " << status << ".\n";

					ProcessResult = static_cast<uint8>(status >> 8);
				}

				// Read output from pipe and put it into Output
				//if (0 == ProcessResult)
				{
					char buffer[1024];
					ssize_t n;
					while (0 != (n = read(PipeFd[0], buffer, sizeof(buffer))))
					{
						if (-1 == n) {
							if (EAGAIN == errno) {
								break;
							} else {
								std::cerr << "Error: Reading from pipe " << PipeFd[0] << " failed with errno " << errno << ".\n";
								break;
							}
						}
						else
						{
							Output.append(buffer, n);
						}
					}
				}
			}
		}

		close(PipeFd[0]);
		close(PipeFd[1]);
	}

	return Output;
}

void WriteToFile(const std::string & Path, const std::string & Content)
{
	//std::cout << "Writing the following to '" << Path << "':\n" << Content << endl;
	std::ofstream OutFile(Path);
	OutFile << Content;
}

void PlayBeep()
{
	//std::cout << "Beep.\n";
	//BeepWidget->m_ExecuteWidget->GetAction()();
	LaunchProcessInBackground({"/usr/bin/afplay", "--volume", "0.5", "data/Cannot Distribute/hitsound.wav"});		// HACK: OS X dependency
}

void Gofmt(std::string & InOut)
{
	InOut = WaitProcessWithStdinStdout("/usr/local/go/bin/gofmt", InOut);
}

std::string WaitProcessWithStdinStdout(std::string Path, std::string In)
{
	std::string Output = "";

	int PipeFd[2];			// Pipe for reading from child's stdout+stderr
	int PipeInFd[2];		// Pipe for writing to child process stdin
	pipe(PipeFd);
	pipe(PipeInFd);
	fcntl(PipeFd[0], F_SETFL, O_NONBLOCK);

	uint8 ProcessResult;

	{
		auto Pid = fork();

		if (0 == Pid)
		{
			close(PipeFd[0]);    // close reading end in the child

			dup2(PipeFd[1], 1);  // send stdout to the pipe
			dup2(PipeFd[1], 2);  // send stderr to the pipe

			close(PipeFd[1]);    // this descriptor is no longer needed

			close(PipeInFd[1]);    // close writing end in the child
			dup2(PipeInFd[0], 0);  // get stdin from the pipe
			close(PipeInFd[0]);    // this descriptor is no longer needed

			execl(Path.c_str(), Path.c_str(), (char *)0);

			// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
			//exit(1);		// Not needed, just in case I comment out the above
			throw 0;
		}
		else if (-1 == Pid)
		{
			perror("Error forking");
			throw 0;
		}
		else
		{
			// Write to child's stdin and end it
			// TODO: Error check the write, perhaps need multiple tries to fully flush it
			write(PipeInFd[1], In.c_str(), In.length());
			close(PipeInFd[1]);

			// Wait for child process to complete
			{
				int status;
				waitpid(Pid, &status, 0);
				Pid = 0;

				//std::cout << "Child finished with status " << status << ".\n";

				ProcessResult = static_cast<uint8>(status >> 8);
			}

			// Read output from pipe and put it into Output
			//if (0 == ProcessResult)
			{
				char buffer[1024];
				ssize_t n;
				while (0 != (n = read(PipeFd[0], buffer, sizeof(buffer))))
				{
					if (-1 == n) {
						if (EAGAIN == errno) {
							break;
						} else {
							std::cerr << "Error: Reading from pipe " << PipeFd[0] << " failed with errno " << errno << ".\n";
							break;
						}
					}
					else
					{
						Output.append(buffer, n);
					}
				}
			}
		}
	}

	close(PipeFd[0]);
	close(PipeFd[1]);
	close(PipeInFd[0]);

	return Output;
}

std::vector<std::string> Ls(const std::string Path)
{
	std::vector<std::string> Entries;

	std::string Output = "";
	{
		int PipeFd[2];			// Pipe for reading from child's stdout+stderr
		pipe(PipeFd);
		fcntl(PipeFd[0], F_SETFL, O_NONBLOCK);

		uint8 ProcessResult;

		{
			auto Pid = fork();

			if (0 == Pid)
			{
				close(PipeFd[0]);    // close reading end in the child

				dup2(PipeFd[1], 1);  // send stdout to the pipe
				dup2(PipeFd[1], 2);  // send stderr to the pipe

				close(PipeFd[1]);    // this descriptor is no longer needed

				// TODO: Option -p misses symbolic link folders, so change to using -F option and do some parsing, or something
				if (Path.empty())
					execl("/bin/ls", "/bin/ls", "-p", (char *)0);
				else
					execl("/bin/ls", "/bin/ls", "-p", Path.c_str(), (char *)0);

				// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
				//exit(1);		// Not needed, just in case I comment out the above
				throw 0;
			}
			else if (-1 == Pid)
			{
				perror("Error forking");
				throw 0;
			}
			else
			{
				// Wait for child process to complete
				{
					int status;
					waitpid(Pid, &status, 0);
					Pid = 0;

					//std::cout << "Child finished with status " << status << ".\n";

					ProcessResult = static_cast<uint8>(status >> 8);
				}

				// Read output from pipe and put it into Output
				//if (0 == ProcessResult)
				{
					char buffer[1024];
					ssize_t n;
					while (0 != (n = read(PipeFd[0], buffer, sizeof(buffer))))
					{
						if (-1 == n) {
							if (EAGAIN == errno) {
								break;
							} else {
								std::cerr << "Error: Reading from pipe " << PipeFd[0] << " failed with errno " << errno << ".\n";
								break;
							}
						}
						else
						{
							Output.append(buffer, n);
						}
					}
				}
			}
		}

		close(PipeFd[0]);
		close(PipeFd[1]);
	}

	// Parse Output and populate Entries
	// TODO: Clean up
	{
		std::stringstream ss;
		ss << Output;
		std::string Line;

		std::getline(ss, Line);
		while (!Line.empty() && !ss.eof())
		{
			Entries.push_back(Line);
			std::getline(ss, Line);
		}
		if (!Line.empty())
			Entries.push_back(Line);
	}

	return Entries;
}

void LaunchProcessInBackground(std::initializer_list<std::string> Argv)
{
	auto Pid = fork();

	if (0 == Pid)
	{
		std::vector<char *> argv;
		for (auto & arg : Argv)
		{
			argv.push_back(const_cast<char *>(arg.c_str()));
		}
		argv.push_back(nullptr);

		execv(argv[0], &argv[0]);

		// TODO: Add error checking on above execl(), and do exit() in case execution reaches here
		//exit(1);		// Not needed, just in case I comment out the above
		throw 0;
	}
	else if (-1 == Pid)
	{
		perror("Error forking");
	}
	else
	{
	}
}
