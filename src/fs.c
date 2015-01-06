#if WIN32

#include <Windows.h>

int
fs_directory_create(const char *path) {

	char *c;
	char newpath[MAX_PATH];

	newpath[0] = '\0';
	c = strtok(path, "\\");

	while (c != NULL) {

		strcat(newpath, c);
		strcat(newpath, "\\");

		if (!fs_directory_exists(newpath))
			CreateDirectory(newpath, NULL);

		c = strtok(NULL, "\\");
	}
}

int
fs_directory_exists(const char *path) {
	DWORD dwAttributes = GetFileAttributes(path);
	if (dwAttributes == INVALID_HANDLE_VALUE)
		return 0;
	if (dwAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return 1;
	return 0;
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

#endif
