
#include <sys/stat.h>

#if WIN32
#include <Windows.h>
#endif

#if WIN32

/*
 * Determines if directory exists. Returns 1 if it does, zero in all other cases including error.
 */
int
fs_directory_exists(const char *path) {
	DWORD dwAttributes = GetFileAttributes(path);
	if (dwAttributes == INVALID_HANDLE_VALUE)
		return 0;
	if (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return 1;
	return 0;
}

/*
 * Creates the specified directory. Will create all parent directories. Returns 1 on success, zero in all other cases.
 */
int
fs_directory_create(const char *path) {

	char *c;
	char newpath[MAX_PATH];
	BOOL failed = FALSE;

	newpath[0] = '\0';
	c = strtok(path, "\\");

	while (c != NULL) {

		strcat(newpath, c);
		strcat(newpath, "\\");

		if (!fs_directory_exists(newpath))
			if (!CreateDirectory(newpath, NULL)) {
				failed = TRUE;
				break;
			}

		c = strtok(NULL, "\\");
	}

	return failed == TRUE ? 0 : 1;
}

#else

int 
fs_directory_exists(const char *path) {
	struct stat sb;
	if (stat(path, &sb) != 0) {
		errx(1, "unable to stat directory");
	}
	return S_ISDIR(sb.st_mode);
}

int
fs_directory_create(const char *path) {
	// TODO: Implement
	return 0;
}

#endif
