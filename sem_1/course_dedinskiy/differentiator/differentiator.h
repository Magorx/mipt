#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

//=============================================================================
// DifNode ====================================================================

class DifNode {
private:
// data =======================================================================

//=============================================================================


public:
	DifNode():
	{}

	~DifNode() {}

	void ctor() {

	}

	static DifNode *NEW() {
		DifNode *cake = calloc(1, sizeof(DifNode));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor() {

	}

	static void DELETE(DifNode *classname) {
		if (!classname) {
			return;
		}

		classname->dtor();
		free(classname);
	}

};

//=============================================================================
// Differentiator =============================================================

class Differentiator {
private:
// data =======================================================================

//=============================================================================


public:
	Differentiator():
	{}

	~Differentiator() {}

	void ctor() {

	}

	static Differentiator *NEW() {
		Differentiator *cake = calloc(1, sizeof(Differentiator));
		if (!cake) {
			return nullptr;
		}

		cake->ctor();
		return cake;
	}

	void dtor() {

	}

	static void DELETE(Differentiator *classname) {
		if (!classname) {
			return;
		}

		classname->dtor();
		free(classname);
	}

};

#endif // DIFFERENTIATOR