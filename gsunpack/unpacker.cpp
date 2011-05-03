#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "types.h"
#include "conversion.h"

#ifdef _WIN32
	#include <windows.h>
	#include <direct.h>
	#define GetCurrentDir _getcwd
#else
	#include <unistd.h>
	#include <linux/limits.h>
	#define GetCurrentDir getcwd
#endif

#pragma pack(push, 2)
typedef struct _index_node {
    u16 string_index;
    u32 offset;
    u32 length;
} index_node;
#pragma pack(pop)

typedef struct _string_table {
    u32 size;
    char **strings;
} string_table;

void decrypt(u32 *v, u32 n, u32 *k) {
    u32 delta = 0x9E3779B9 * n, i;
    for(i = 0; i < n; i++) {
        v[1]  -= (((v[0] << 4) ^ (v[0] >> 5)) + v[0]) ^ (delta + k[(delta >> 0xB) & 3]);
        delta -= 0x9E3779B9;
        v[0]  -= (((v[1] << 4) ^ (v[1] >> 5)) + v[1]) ^ (delta + k[delta & 3]);
    }
}

/* 
 * Not the most efficient way but written like this because the speed difference is minor.
 * Should at one point just load in the table and assign pointers like in the parser.
*/
string_table *get_string_table(char *class_path) {
	char path_buffer[MAX_PATH];
	sprintf(path_buffer, "%s/gsclassesall.idx.str", class_path);
    FILE *str_index_fp = fopen(path_buffer, "rb");
    assert(str_index_fp != 0);

    string_table *table = (string_table*)malloc(sizeof(string_table));
    assert(table != 0);

    /* First find the biggest string and stringcount. */
    u32 biggest = 0, count = 0;
    (*table).size = 0;
    while(!feof(str_index_fp)) {
        char c;
        fread(&c, sizeof(c), 1, str_index_fp);
        if(c) count++;
        else {
            if(count > biggest) biggest = count;
            count = 0;
            (*table).size++;
        }
    }

    /* We'll use size of the biggest string as a temporary string buffer. */
    char *buffer = (char*)malloc(biggest + 1);
    assert(buffer != 0);
    
    (*table).strings = (char**)malloc((*table).size * sizeof(char*));
    assert((*table).strings != 0);
    fseek(str_index_fp, 0, SEEK_SET);

    /* Then just allocate a char array for each string and copy from the buffer. */
    u32 pos = 0, strcnt = 0;
    while(!feof(str_index_fp)) {
        fread(&(buffer[pos++]), 1, 1, str_index_fp);
        if(!buffer[pos-1]) {
            (*table).strings[strcnt] = (char*)malloc(pos);
            assert((*table).strings[strcnt] != 0);
            
            memcpy((*table).strings[strcnt], buffer, pos - 1);
            (*table).strings[strcnt++][pos-1] = '\0';
            pos = 0;
        }
    }
    (*table).size--;

    /* Cleanup. */
    free(buffer);
    fclose(str_index_fp);

    return table;
}

void free_string_table(string_table *table) {
    /* Clear up individual strings. */
    u32 i;
    for(i = 0; i < (*table).size; i++) {
        free(((*table).strings)[i]);
    }

    /* Then the string-array and table struct. */
    free((*table).strings);
    free(table);
}

int main(int argc, char *argv[]) {
	if(argc < 2) {
		printf("Usage: %s <class path>\n", argv[0]);
		return 0;
	}

	char path_buffer[MAX_PATH];
	sprintf(path_buffer, "%s/gsclassesall.idx", argv[1]);
    FILE *index_fp = fopen(path_buffer, "rb");
    assert(index_fp != 0);
    
	sprintf(path_buffer, "%s/gsclassesall.dat", argv[1]);
    FILE *data_fp = fopen(path_buffer, "rb");
    assert(data_fp != 0);

    /* Generate the string table. */
    string_table *table = get_string_table(argv[1]);

    while(!feof(index_fp)) {
        /* Find a node. NOTE: There is a node every 10 bytes. */
        index_node node;
        if(!fread(&node, sizeof(node), 1, index_fp)) continue;
        node.offset = ntohl(node.offset);
        node.length = ntohl(node.length);
        node.string_index = ntohs(node.string_index);
        // printf("Found node with offset: %.8X and length: %d, string table index: %d.\n", node.offset, node.length, node.string_index);

        /* Set FILE pointer to offset and allocate memory. */
        fseek(data_fp, node.offset, SEEK_SET);
        u8 *data = (u8*)malloc(node.length * sizeof(u8));
        assert(data != 0);

        /* This seems static, but key generation can be found here: 0x130C68E0 - GSINTERP.?makeKey@@YAHPAUkeyInstance@@EHPAD@Z */
        u32 key[] = { 0xF7DA33EE, 0x9D43E52D, 0x536EF7DA, 0x3423DEE3, 0x131036E4 };

        /* Process the file in 8 byte blocks. */
        u32 blocks = node.length >> 3, i;
        for(i = 0; i < blocks; i++) {
            u8 *ptr = data + (i << 3);
            fread(ptr, sizeof(u32), 2, data_fp);

            /* Decrypt 8 byte block using pre-generated key. */
            decrypt((u32*)ptr, 0x11, key);
        }

        /* For now, just save to disk. */
        sprintf(path_buffer, "classes/%s", (*table).strings[node.string_index]);
        printf("Writing %s to file.\n\n", (*table).strings[node.string_index]);
        FILE *out = fopen(path_buffer, "wb");
        assert(out != 0);

        fwrite(data, node.length, 1, out);
        fclose(out);
        
        /* Cleanup. */
        free(data);
    }

    /* Cleanup. */
    free_string_table(table);
    fclose(data_fp);
    fclose(index_fp);

    return 0;
}
