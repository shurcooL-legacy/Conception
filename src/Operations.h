#pragma once
#ifndef __Operations_H__
#define __Operations_H__

typedef std::function<void(std::string &)> Operation;

std::function<Color(uint32, const std::string &)> GetLineHighlighting();
std::string ParsePath(std::string Path, uint8 Mode);
std::function<std::vector<std::string>(std::string::size_type)> GetAutocompletions(std::string Path);
void SkipFirstLines(std::string & InOut, uint32 LinesToSkip);
std::string TrimFirstSpace(const std::string & In);
void TrimLastNewline(std::string & InOut);
std::string Diff(const std::string & Content1, const std::string & Content2);
void WriteToFile(const std::string & Path, const std::string & Content);
void PlayBeep();
void Gofmt(std::string & InOut);
std::vector<std::string> Ls(std::string & InOut);
void LaunchProcessInBackground(std::initializer_list<std::string> Argv);

#endif // __Operations_H__
