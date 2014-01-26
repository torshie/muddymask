#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef CMAKE_C_COMPILER
#	define CMAKE_C_COMPILER "cc"
#endif
#ifndef CMAKE_CXX_COMPILER
#	define CMAKE_CXX_COMPILER "c++"
#endif
#ifndef CMAKE_BINARY_DIR
#	define CMAKE_BINARY_DIR ""
#endif

char* getCompilerName(const char* name) {
	static char kCC[] = CMAKE_C_COMPILER;
	static char kCXX[] = CMAKE_CXX_COMPILER;

	const char* slash = strrchr(name, '/');
	if (slash == NULL) {
		if (strcmp(name, "mycc") == 0) {
			return kCC;
		} else if (strcmp(name, "mycxx") == 0) {
			return kCXX;
		} else {
			return NULL;
		}
	} else {
		if (strcmp(slash + 1, "mycc") == 0) {
			return kCC;
		} else if (strcmp(slash + 1, "mycxx") == 0) {
			return kCXX;
		} else {
			return NULL;
		}
	}
}

void printVector(char** vector) {
	while (*vector != NULL) {
		fprintf(stderr, " %s", *vector);
		++vector;
	}
	fprintf(stderr, "\n");
}

int main(int argc, char** argv) {
	static char kExtraArg[][256] = {
		"-I" CMAKE_BINARY_DIR "/third_party/include",
		"-L" CMAKE_BINARY_DIR "/third_party/lib",
		"-Wl,-rpath=$ORIGIN:$ORIGIN/../lib:$ORIGIN/../third_party/lib",
	};

	static char* vector[100000];
	vector[0] = getCompilerName(argv[0]);
	if (vector[0] == NULL) {
		fprintf(stderr, "This program can only be invoked "
				"as \"mycc\" or \"mycxx\"\n");
		return 1;
	}

	int target = 1;

	for (unsigned i = 0; i < sizeof(kExtraArg) / sizeof(kExtraArg[0]);
			++i, ++target) {
		vector[target] = kExtraArg[i];
	}
	for (int i = 1; i < argc; ++i, ++target) {
		vector[target] = argv[i];
	}
	vector[target] = NULL;

	if (getenv("MYWRAPPER_VERBOSE") != NULL) {
		printVector(vector);
	}
	execvp(vector[0], vector);
	fprintf(stderr, "Invoke real compiler failed:");
	printVector(vector);

	return 1;
}
