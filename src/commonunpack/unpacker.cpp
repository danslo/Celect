#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "types.h"
#include "stringtable.h"

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <linux/limits.h>
#endif

typedef struct {
    u32 offset;
    u32 length;
} index_node;

void decrypt(u8 *data, u32 size) {
    u32 delta = 0xB59AF178, i;
    for(i = 0; i < size; i++) {
        data[i] ^= (delta & 0xff);
        u32 prev = delta;
        delta <<= 1;
        if((prev & 0x80000000) != 0) {
            delta |= 1;
        }
    }
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s <media folder>\n", argv[0]);
		return 0;
	}

    /* Get our filesnames. */
    char *table[1024] = { 0 };
    get_string_table(table, argv[1]);
    
    /* Open the index and the data files. */
	char path_buffer[PATH_MAX];

	sprintf(path_buffer, "%s/common.idx", argv[1]);
    FILE *index_fp = fopen(path_buffer, "rb");
    assert(index_fp != 0);

	sprintf(path_buffer, "%s/common.dat", argv[1]);
    FILE *data_fp = fopen(path_buffer, "rb");
    assert(data_fp != 0);
   
    u32 count = 0;
    while(!feof(index_fp)) {
        /* Grab a valid node from our index. */
        index_node node;
        if(!fread(&node, sizeof(node), 1, index_fp)) continue;
        if(!node.offset && !node.length) continue;

        /* Seek to offset and read data. */
        fseek(data_fp, node.offset, SEEK_SET);
        u8 *data = (u8*)malloc(node.length);
        assert(data != 0);
        fread(data, node.length, 1, data_fp);

        /* Decrypt data... */
        decrypt(data, node.length);
        
        /* I don't want to write mkdir code, so here's a quick workaround. */
        char *fn = table[count];
        do {
            if(*fn == '/') *fn = '-';
            fn++;
        } while(*fn);

        /* Out path. */
        sprintf(path_buffer, "unpacked/%s", table[count]);

        /* Write it. */
        FILE *file_fp = fopen(path_buffer, "wb");
        assert(file_fp != 0);
        printf("Unpacking: %s\n", table[count]);
        fwrite(data, node.length, 1, file_fp);
        fclose(file_fp);

        count++;

        /* Cleanup. */
        free(data);
    }
    
    /* Cleanup. */
    fclose(data_fp);
    fclose(index_fp);
    free_string_table(table);
    return 0;
}
