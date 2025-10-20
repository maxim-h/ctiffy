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

typedef struct TIFF_HEADER {
    enum  Byte_order {MM, II} byte_order;
    uint64_t first_ifd_offset;

} TIFF_HEADER;

typedef struct IFD {
    uint16_t n_entries;
} IFD;

uint16_t swap16(uint16_t x) {
    return (x >> 8) | (x << 8);
}

int main() {


    char input_file[] = "./data/tubhiswt_C0.ome.tif";
    int fd = open(input_file, O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 69;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) { perror("fstat"); close(fd); return 69; }

    size_t filesize = st.st_size;

    uint8_t *map = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
    if (map == MAP_FAILED) {perror("mmap"); close(fd); return 69;}


    uint16_t byte_order = 0;
    memcpy(&byte_order, map, 2);

    printf("Byte order: %x\n", byte_order);

    uint16_t magic_number = 0;
    memcpy(&magic_number, map+2, 2);

    printf("Magic number: %d\n", (magic_number));


    munmap(map, filesize);
    close(fd);
    return 0;
}
