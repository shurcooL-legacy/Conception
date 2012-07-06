#pragma once
#ifndef __Function_H__
#define __Function_H__

class Function
{
public:
	Function();
	~Function();

	Vector2n Position;

	ConceptString name;
	ConceptString args;
	ConceptString return_type;
	ConceptString body_lines;

	template <typename StreamT> friend StreamT & operator << (StreamT & out, const Function & f)
	{
		out << f.return_type << " " << f.name << "(" << f.args << ")" << endl;

		out << "{" << endl;

		out << "\t";

		//for (auto it = f.body_lines.begin(); it != f.body_lines.end(); ++it)
		/*for (vector<unsigned int>::size_type i = 0; i < f.body_lines.size(); ++i)
		{
			if (i != 0 && Concepts[f.body_lines[i]].m_Concept.compare("."))
				out << " ";

			if (i != 0)
				out << Concepts[f.body_lines[i]].m_Concept.c_str();
			else
			{
				out << static_cast<char>(toupper(Concepts[f.body_lines[i]].m_Concept.c_str()[0]));
				if (Concepts[f.body_lines[i]].m_Concept.length() > 1)
					out << Concepts[f.body_lines[i]].m_Concept.c_str() + 1;
			}
		}*/
		out << f.body_lines;

		out << "}" << endl;

		return out;
	}

private:
	//Function(const Function &) = delete;		// TODO: See if I can uncomment this
	//Function & operator = (const Function &) = delete;		// TODO: See if I can uncomment this
};

#endif // __Function_H__
