//
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NV_SDK_PATH_H
#define NV_SDK_PATH_H

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <string>

namespace nv {

	class SDKPath {

	public:
		SDKPath() {
#if 0
			const char *env = getenv("NVSDK10OGL_ROOT");
#endif

			//search local paths first, in-case someone has the SDK installed while hacking another copy
			_pathList.push_back("./");  // present directory
			_pathList.push_back("../"); // back one
			_pathList.push_back("../../"); // back two

#if 0
			if ( env) {
				_pathList.push_back(std::string(env) + "/");  // Path lacks a terminating slash
			}
#endif
		}

		void addPath( const std::string &path) {
			_pathList.push_back(path);
		}

		void clearPaths() {
			_pathList.clear();
		}

		bool getFilePath( const std::string &file, std::string &path) {
			std::string pathString;

			for ( std::vector<std::string>::iterator it = _pathList.begin(); it != _pathList.end(); it++) {
				pathString = *it + file;
				FILE *fp = fopen( pathString.c_str(), "rb");
				if (fp) {
					fclose(fp);
					path = pathString;
					return true;
				}
			}

			return false;
		}

		bool getPath( const std::string &file, std::string &path) {
			std::string pathString;

			for ( std::vector<std::string>::iterator it = _pathList.begin(); it != _pathList.end(); it++) {
				pathString = *it + file;
				FILE *fp = fopen( pathString.c_str(), "rb");
				if (fp) {
					fclose(fp);
					path = *it;
					return true;
				}
			}

			return false;
		}

	private:
		std::vector<std::string> _pathList;

	};
};

#endif
