
#ifndef LIBXLD_XLD_H
#define LIBXLD_XLD_H

#include <stdint.h>

typedef struct xld_archive xld_t;
struct xld_archive {
	int fd;
	uint16_t count;
	uint32_t *length;
};

/*
 * Open archive file.
 */
xld_t *
xld_open(const char *filename);

/*
 * Close archive file.
 */
void
xld_close(xld_t *xld);

/** PRIVATE (make them static) **/

/*
 * Calculate offset of resource within the archive.
 */
uint32_t
xld_resource_offset(xld_t *xld, int n);

/*
 * Get resource length.
 */
uint32_t
xld_resource_length(xld_t *xld, int n);

/*
 * Calculate length of the file header.
 */
uint32_t
xld_file_header_length(xld_t *xld);

/*
 * Extract the specified resource to a buffer.
 */
uint32_t 
xld_resource_extract(xld_t *xld, int n, char *buf);

/*
 * Extract the specified resource to a file descriptor.
 */
uint32_t 
xld_resource_extract_fd(xld_t *xld, int n, int fd);

/*
 * Extract the specified resource to a memory buffer.
 */
/*
uint32_t
xld_resource_extract(xld_t *xld, int n, char *buf, int bufsize);
*/

#endif
