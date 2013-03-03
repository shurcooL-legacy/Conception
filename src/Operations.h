#pragma once
#ifndef __Operations_H__
#define __Operations_H__

typedef std::function<void(std::string &)> Operation;

std::string TrimFirstSpace(const std::string & In);
std::string Diff(const std::string & Content1, const std::string & Content2);
void WriteToFile(const std::string & Path, const std::string & Content);
void PlayBeep();
void TrimLastNewline(std::string & InOut);
void Gofmt(std::string & InOut);
std::vector<std::string> Ls(std::string & InOut);
void LaunchProcessInBackground(std::initializer_list<std::string> Argv);

#endif // __Operations_H__
