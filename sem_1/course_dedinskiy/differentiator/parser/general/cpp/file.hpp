#ifndef CPP_FILE_H
#define CPP_FILE_H

#include <cctype>
#include <sys/stat.h>

//=============================================================================
// Filepp =======================================================================

class Filepp {
public:
// data =======================================================================
	char *data;
	const char *name;
	FILE *fileptr;
	struct stat info;
//=============================================================================

	Filepp            (const Filepp&) = delete;
	Filepp &operator= (const Filepp&) = delete;

	Filepp():
	data(nullptr),
	name(nullptr),
	fileptr(nullptr),
	info()
	{}

	~Filepp() {}

	void ctor() {
		data = nullptr;
		name = nullptr;
		fileptr = nullptr;
	}

	static Filepp *NEW() {
		Filepp *cake = (Filepp*) calloc(1, sizeof(Filepp));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void ctor(const char *name_) {
		name = name_;
		if (!name) {
			return;
		}

		fileptr = fopen(name, "rb");
		if (!fileptr) {
			return;
		}

		fstat(fileno(fileptr), &(info));

		data = (char*) calloc((size_t) info.st_size + 2, sizeof(char));
	    if (!data) {
	    	fclose(fileptr);
	        return;
	    }

		fread(data, (size_t) info.st_size, 1, fileptr);
	}

	static Filepp *NEW(const char *name_) {
		Filepp *cake = (Filepp*) calloc(1, sizeof(Filepp));
		if (!cake) {
			return nullptr;
		}

		cake->ctor(name_);
		return cake;
	}

	void dtor() {
		if (name) {
			name = nullptr;
		}

		if (data) {
			free(data);
			data = nullptr;
		}

		if (fileptr) {
			fclose(fileptr);
			fileptr = nullptr;
		}
	}

	static void DELETE(Filepp *file) {
		if (!file) {
			return;
		}

		file->dtor();
		free(file);
	}

};

#endif // CPP_FILE_H