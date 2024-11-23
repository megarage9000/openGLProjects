#include "GarageLog.h"

// --- Logging ---
// Restarting/Starting log
bool restart_gl_log() {
	FILE* file;
	int errno = fopen_s(&file, GL_LOG_FILE, "w");
	if (errno != 0) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE log file %s for writing \n",
			GL_LOG_FILE
		);
		return false;
	}

	// Writing to log date
	time_t now = time(NULL);
	char date[100];
	ctime_s(date, 100, &now);
	fprintf(file, "GL_LOG_FILE log local time = %s\n", date);
	fclose(file);

	return true;
}

// Logging messages onto our log file
// [NOTE] the "..." enables variable number of arguments in C, which will
// be appended to our string message
bool gl_log(const char* message, ...) {
	va_list argPtr;

	// Appending to our log new messages
	FILE* file;
	int errno = fopen_s(&file, GL_LOG_FILE, "w");
	if (errno != 0) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s, file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}

	va_start(argPtr, message);
	vfprintf(file, message, argPtr);
	va_end(argPtr);
	fclose(file);
	return true;
}

// Logging error messages onto our log file
bool gl_log_err(const char* message, ...) {
	va_list argPtr;

	FILE * file;
	int errno = fopen_s(&file, GL_LOG_FILE, "a");
	if (errno != 0) {
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s, file for appending\n",
			GL_LOG_FILE
		);
		return false;
	}

	va_start(argPtr, message);
	vfprintf(file, message, argPtr);
	va_end(argPtr);

	va_start(argPtr, message);
	vfprintf(stderr, message, argPtr);
	va_end(argPtr);

	fclose(file);
	return true;
}


