#ifndef CPP_FILE_H
#define CPP_FILE_H

#error Hey boy i'm not written

#include <>
#include <sys/stat.h>

//=============================================================================
// File ==================================================================

class File {
private:
// data =======================================================================
char *data;
char *filename;
FILE *fileptr;
//=============================================================================


public:
	struct stat info;

	File():
	{}

	~File() {}

	void ctor() {

	}

	static File *NEW() {
		File *cake = (File*) calloc(1, sizeof(File));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor() {

	}

	static void DELETE(File *file) {
		if (!file) {
			return;
		}

		file->dtor();
		free(file);
	}

};

#endif // CPP_FILE_H