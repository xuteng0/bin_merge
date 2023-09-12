#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int merge(const char* src_path, const unsigned int offset, const char* dest_path) {
    FILE *src, *dest;
    int ch;

    // Check if the source file exists
    src = fopen(src_path, "rb");
    if (!src) {
        perror("Error: Source file does not exist");
        return -1;
    }

    // Check if the destination file exists, if not create it
    dest = fopen(dest_path, "ab+");
    if (!dest) {
        perror("Error: Failed to open/create destination file");
        fclose(src);
        return -1;
    }

    // Check if destination file size is less than the offset to prevent overwrite
    fseek(dest, 0, SEEK_END);
    long dest_size = ftell(dest);

    if (dest_size > offset) {
        printf("Error: Destination file size is greater than offset. Exiting to prevent overwriting.\n");
        fclose(src);
        fclose(dest);
        return -1;
    }

    // 4. Add padding and then add the source file to the destination file
    for (unsigned int i = dest_size; i < offset; i++) {
        fputc(0, dest); // Padding with zeros
    }

    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }

    fclose(src);
    fclose(dest);
    return 0;
}

void print_file_stat(const char* path) {
    struct stat st;
    if (stat(path, &st) == 0) {
        printf("Statistics for %s:\n", path);
        printf("\tFile size: %lld bytes\n", (long long) st.st_size);
        printf("\tMode: %o\n", st.st_mode & 0777); // Print permission bits
        printf("\tInode number: %lld\n", (long long) st.st_ino);
        // You can add more file stats here if required
        printf("\n");
    } else {
        perror("stat");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4 || argc % 2 != 0) {
        for (int i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, argv[i]);
        }
        printf("Usage: %s <output_file> <input1.bin> <0xoffset1> <input2.bin> <0xoffset2> ...\n", argv[0]);
        return 1;
    }

    const char* output_file = argv[1];

    if (remove(output_file) == 0) {
        printf("Existing output file %s deleted successfully.\n", output_file);
    } else {
        // It's common for remove to fail if the file doesn't exist, so we don't need to raise an error in that case
        perror("Note: Failed to delete the existing output file (if it existed)");
    }

    for (int i = 2; i < argc; i += 2) {
        const char* input_file = argv[i];
        unsigned int offset = strtol(argv[i + 1], NULL, 0);  // Convert hex string to integer

        print_file_stat(input_file);

        if (merge(input_file, offset, output_file) != 0) {
            fprintf(stderr, "Failed to merge %s into %s.\n", input_file, output_file);
            return 1;
        }
    }

    printf("Files merged successfully into %s.\n", output_file);
    print_file_stat(output_file);
    return 0;
}
