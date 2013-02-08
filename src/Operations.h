#pragma once
#ifndef __Operations_H__
#define __Operations_H__

typedef std::function<void(std::string &)> Operation;

void TrimLastNewline(std::string & InOut);
void Gofmt(std::string & InOut);

#endif // __Operations_H__