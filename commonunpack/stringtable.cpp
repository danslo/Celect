#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#define PATH_MAX MAX_PATH
#else
#include <linux/limits.h>
#endif

#include "types.h"
#include "stringtable.h"

void get_string_table(char **table, char *media_path) {
	char path_buffer[PATH_MAX];
	sprintf(path_buffer, "%s/common.str.idx", media_path);
    FILE *str_index_fp = fopen(path_buffer, "rb"); // Open the file in (read)(binary) mode.
    assert(str_index_fp != 0);

    /* Next, we will use some C file functions to get the size of the whole file. */
    fseek(str_index_fp, 0, SEEK_END);
    int file_size = ftell(str_index_fp); 
    fseek(str_index_fp, 0, SEEK_SET);

    /* Now allocate some memory and read the file... */
    u8 *data = (u8*)malloc(file_size);
    assert(data != 0);                      // Make sure we actually allocated it :)
    if(!fread(data, sizeof(u8), file_size, str_index_fp)) {
        perror("fread() call failed");
        exit(-1);
    }

    /* So we have the data... now we must process it. */
    u8 *ptr = data;
    u32 loc = 0;
    while(++ptr != data + file_size) {
        if(!*(ptr-1)) {
            table[loc++] = (char*)ptr;
        }
    }

    /* Cleanup. */
    fclose(str_index_fp);
}

void free_string_table(char **table) {
    free(table[0] - 1);
}
