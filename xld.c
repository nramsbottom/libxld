
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "xld.h"

xld_t *
xld_open(const char *filename) {
	
	int fd;
	uint32_t magic;
	uint16_t unknown;
	uint16_t count;
	
	if (!(fd = open(filename, O_RDONLY))) {
		close(fd);
		return 0;
	}
	
	// first, get the magic number - the first four digits
	// of the file that should identify the file type
	if (read(fd, &magic, sizeof(magic)) != sizeof(magic)) {
		close(fd);
		return 0;
	}
	
	// TODO: verify magic file number is correct (XLD0)
	
	// read a single byte here... the purpose is unknown
	if (read(fd, &unknown, sizeof(unknown)) != sizeof(unknown)) {
		close(fd);
		return 0;
	}
	
	// read two bytes which contain the number of resources
	// within the archive
	if (read(fd, &count, sizeof(count)) != sizeof(count)) {
		close(fd);
		return 0;
	}
	
	xld_t *xld = malloc(sizeof(xld) + count * sizeof(uint32_t));
	
	xld->fd = fd;
	xld->count = count;	
	xld->length = (uint32_t*)(xld + 1);

	// read content lengths
	if (read(fd, xld->length, sizeof(uint32_t) * count) != sizeof(uint32_t) * count) {
		free(xld);
		return 0;		
	}
	
	return xld; 
}

void
xld_close(xld_t *xld) {
	if (xld)
		free(xld);
}

uint32_t
xld_resource_offset(xld_t *xld, int n) {
	uint32_t *o = xld->length;
	uint32_t r = xld_file_header_length(xld);
	int x;
	for (x=0;x<n;x++,o++) {
		r+=*o;
	}
	return r;
}

uint32_t
xld_resource_length(xld_t *xld, int n) {
	return *(xld->length + n);
}

uint32_t
xld_file_header_length(xld_t *xld) {
	return sizeof(uint32_t)
			+ sizeof(uint32_t)
			+ (xld->count * sizeof(uint32_t));
}

uint32_t 
xld_resource_extract_fd(xld_t *xld, int n, int fd) {

	uint32_t offset = xld_resource_offset(xld, n);
	uint32_t length = xld_resource_length(xld, n);

	lseek(xld->fd, offset, SEEK_SET);
	
	// reads the resource to the specified file descriptor
	char buf[4096];
	uint32_t t = 0;
	ssize_t r = read(xld->fd, buf, length - t < 4096 ? length - t : 4096 );
	while (t < length) {
		write(fd, buf, r);
		t+=r;
		r = read(xld->fd, buf, length - t < 4096 ? length - t : 4096 );
	}
	return t;
}

uint32_t 
xld_resource_extract(xld_t *xld, int n, char *buf) {

	uint32_t offset = xld_resource_offset(xld, n);
	uint32_t length = xld_resource_length(xld, n);

	lseek(xld->fd, offset, SEEK_SET);
	
	return read(xld->fd, buf, length);
}
