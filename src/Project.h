#pragma once
#ifndef __Project_H__
#define __Project_H__

class Project
{
public:
	Project();
	~Project();

	void LoadSampleGenProgram(Canvas & CanvasTEST);

	void GenerateProgram(std::string ProgramContent);
	void RunProgram(TextFieldWidget * OutputWidget);

	std::vector<ConceptId> & GetStdIncludes() { return std_includes; }

private:
	Project(const Project &);
	Project & operator =(const Project &);

	std::vector<ConceptId> std_includes;
	std::vector<Function> functions2;
};

#endif // __Project_H__
