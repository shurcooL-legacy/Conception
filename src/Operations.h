#pragma once
#ifndef __Operations_H__
#define __Operations_H__

typedef std::function<void(std::string &)> Operation;

void TrimLastNewline(std::string & InOut);
void Gofmt(std::string & InOut);
std::vector<std::string> Ls(std::string & InOut);
void LaunchProcessInBackground(std::initializer_list<std::string> Argv);

#endif // __Operations_H__