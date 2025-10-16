#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/_endian.h>
#include <sys/_types/_off_t.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdint.h>


int main() {
    int fd = open("./data/tubhiswt_C0.ome.tif", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 69;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) { perror("fstat"); close(fd); return 69; }

    size_t filesize = st.st_size;

    uint32_t *map = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {perror("mmap"); close(fd); return 69;}

    off_t offset = 32;

    if (offset + sizeof(uint32_t) > (off_t)filesize) {
        fprintf(stderr, "offset out of range\n");
        munmap(map, filesize);
        close(fd);
        return 1;
    }


    uint16_t bo = 0;
    memcpy(&bo, map, 2);

    printf("%x\n", bo);


    munmap(map, filesize);
    close(fd);
    return 0;
}
