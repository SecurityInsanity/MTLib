#include "PrintStackTrace.h"

/**
* Prints stack trace to user defined buffer.
* Always terminates the buffer with 0.
*/
extern EXPORT void printStackTrace(char* buffer, int bufferSize) {
#if defined(WIN32) && defined(_DEBUG)
	// find out map file name
	char modname[500];
	MapFile::getModuleMapFilename(modname, sizeof(modname));

	// parse map file
	char buf[600];
	MapFile map(modname);
	switch (map.error()) {
	case MapFile::ERROR_OPEN:
		sprintf(buf, "Failed to open map file %s\n", modname);
		break;
	case MapFile::ERROR_READ:
		sprintf(buf, "Error while reading map file %s(%i)\n", modname, map.line());
		break;
	case MapFile::ERROR_PARSE:
		sprintf(buf, "Parse error in map file %s(%i)\n", modname, map.line());
		break;
	default:
		break;
	}

	// print stack trace to buffer
	if (!map.error()) {
		MapFile* maps[] = { &map };
		StackTrace::printStackTrace(maps, 1, 1, 16, buf, sizeof(buf));
	}

	// copy to user buffer
	if (bufferSize > 0) {
		strncpy(buffer, buf, bufferSize);
		buffer[bufferSize - 1] = 0;
	}
#endif
}

/**
* Prints stack trace to stdout
*/
extern EXPORT void printStackTrace() {
#if defined(WIN32) && defined(_DEBUG)
	char buffer[6400];
	printStackTrace(&buffer[0], 6400);
	printf("%s", buffer);
#endif
}

/**
* Used for assertions
*/
extern EXPORT void AssertFailed(const char* fname, int line, const char* expr) {
	printf("Assertion Failed: (%s)\n", expr);
	printf("Location: %s(%i)\n", fname, line);
	printf("Expression: %s\n", expr);
#if defined(WIN32) && defined(_DEBUG)
	printf("Stack trace:\n");
	printStackTrace();
#endif
}
