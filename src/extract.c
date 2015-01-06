
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fs.h"
#include "xld.h"


void usage();
void extract_all(const char *archive, const char *output_directory);
int is_directory(const char *path);

int
main(int argc, char *argv[]) {

	if (argc < 3) {
		usage();
		return 1;
	}

	extract_all(argv[1], argv[2]);

	return 0;
}

void
usage() {
	printf("extract <xld-archive> <directory>\n");
}

void
extract_all(const char *archive, const char *output_directory) {

	char filename[128];
	int n;
	int ofd;
	xld_t *x = xld_open(archive);

	// TODO: ensure that the output directory
	//       is actually a directory
	if (!is_directory(output_directory)) {
		errx(1, "output is not a directory");
	}

	if (!x) {
		errx(1, "unable to open archive");
	}
	
	for (n=0; n<x->count; n++) {
		
		sprintf(filename,
				"%s%02d.mid",
				output_directory,
				n);
	
		ofd = open(filename, O_RDWR | O_CREAT, 0666);
		
		if (!ofd) {
			errx(1, "unable to open %s", filename);
		}

		xld_resource_extract_fd(x, n, ofd);

		close(ofd);
				
	}

	xld_close(x);
}

int is_directory(const char *path) {
	struct stat sb;
	if (stat(path, &sb) != 0) {
		errx(1, "unable to stat directory");
	}
	return S_ISDIR(sb.st_mode);
}
