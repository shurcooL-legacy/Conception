#pragma once
#ifndef __Operations_H__
#define __Operations_H__

typedef std::function<void(std::string &)> Operation;

const std::string ParseGistId(const std::string & JsonResponse);
bool IsFileTrackedByGit(const std::string Path);
std::function<Color(uint32, const std::string &)> GetLineHighlighting();
std::string ParsePath(const std::string Path, uint8 Mode);
std::function<std::vector<std::string>(std::string::size_type)> GetAutocompletions(const std::string Path);
void SkipFirstLines(std::string & InOut, uint32 LinesToSkip);
std::string TrimFirstSpace(const std::string & In);
void TrimLastNewline(std::string & InOut);
std::string Diff(const std::string & Content1, const std::string & Content2);
void WriteToFile(const std::string & Path, const std::string & Content);
void PlayBeep();
void Gofmt(std::string & InOut);
std::vector<std::string> Ls(const std::string Path);
void LaunchProcessInBackground(std::initializer_list<std::string> Argv);

#endif // __Operations_H__
