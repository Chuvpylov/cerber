#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEP "\\"
#else
    #include <unistd.h>
    #define PATH_SEP "/"
#endif

void run_command(const char *command) {
    printf("Running: %s\n", command);
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "Command failed with code %d\n", result);
        exit(1);
    }
}

int is_file_modified_after(const char *file1, const char *file2) {
    struct stat stat1, stat2;
    if (stat(file1, &stat1) != 0) {
        perror(file1);
        return 0;
    }
    if (stat(file2, &stat2) != 0) {
        perror(file2);
        return 1;
    }
    return difftime(stat1.st_mtime, stat2.st_mtime) > 0;
}

char** list_files(const char *dir_path, const char *extension, int *count) {
    DIR *dir;
    struct dirent *ent;
    char **file_list = NULL;
    *count = 0;

    if ((dir = opendir(dir_path)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, extension) != NULL) {
                file_list = realloc(file_list, (*count + 1) * sizeof(char*));
                file_list[*count] = malloc(strlen(dir_path) + strlen(ent->d_name) + 2);
                sprintf(file_list[*count], "%s%s%s", dir_path, PATH_SEP, ent->d_name);
                (*count)++;
            }
        }
        closedir(dir);
    } else {
        perror("Could not open directory");
        exit(EXIT_FAILURE);
    }

    return file_list;
}

void free_file_list(char **file_list, int count) {
    for (int i = 0; i < count; i++) {
        free(file_list[i]);
    }
    free(file_list);
}

void build_project() {
    int src_count, header_count;
    char **source_files = list_files("../src", ".c", &src_count);
    char **header_files = list_files("../include", ".h", &header_count);

    char server_output_file[100] = "../bin/server";
    char test_output_file[100] = "../bin/test_main";
    char raylib_output_file[100] = "../bin/raylib_app";

#ifdef _WIN32
    strcat(server_output_file, ".exe");
    strcat(test_output_file, ".exe");
    strcat(raylib_output_file, ".exe");
#endif

    int rebuild_server = 0;
    int rebuild_raylib = 0;

    for (int i = 0; i < src_count; ++i) {
        if (strstr(source_files[i], "main.c") || strstr(source_files[i], "controller.c") || strstr(source_files[i], "model.c") || strstr(source_files[i], "view.c") || strstr(source_files[i], "log_manager.c")) {
            if (is_file_modified_after(source_files[i], server_output_file)) {
                rebuild_server = 1;
                break;
            }
        }
    }

    for (int i = 0; i < src_count; ++i) {
        if (strstr(source_files[i], "raylib_app.c")) {
            if (is_file_modified_after(source_files[i], raylib_output_file)) {
                rebuild_raylib = 1;
                break;
            }
        }
    }

    if (rebuild_server) {
        printf("Building server...\n");

        char command[1024] = "gcc -I../include -o ";
        strcat(command, server_output_file);

        for (int i = 0; i < src_count; ++i) {
            if (strstr(source_files[i], "main.c") || strstr(source_files[i], "controller.c") || strstr(source_files[i], "model.c") || strstr(source_files[i], "view.c") || strstr(source_files[i], "log_manager.c")) {
                // Exclude test_main.c from the main server build
                if (strstr(source_files[i], "test_main.c") == NULL) {
                    strcat(command, " ");
                    strcat(command, source_files[i]);
                }
            }
        }

#ifdef _WIN32
        strcat(command, " -lws2_32 -lpthread");
#else
        strcat(command, " -lpthread");
#endif

        run_command(command);

        printf("Server build completed.\n");
    } else {
        printf("No changes detected for the server. Skipping build.\n");
    }

    if (rebuild_raylib) {
        printf("Building Raylib app...\n");

        char command[1024] = "gcc -I../include -o ";
        strcat(command, raylib_output_file);

        strcat(command, " ../src/raylib_app.c");

#ifdef _WIN32
        strcat(command, " -lraylib -lopengl32 -lgdi32 -lwinmm");
#else
        strcat(command, " -lraylib -lGL -lm -lpthread -ldl -lrt -lX11");
#endif

        run_command(command);

        printf("Raylib app build completed.\n");
    } else {
        printf("No changes detected for the Raylib app. Skipping build.\n");
    }

    // Build test executable
    printf("Building test executable...\n");

    char test_command[1024] = "gcc -I../include -o ";
    strcat(test_command, test_output_file);
    strcat(test_command, " ../test/test_main.c ../src/controller.c ../src/log_manager.c");

#ifdef _WIN32
    strcat(test_command, " -lws2_32 -lpthread");
#else
    strcat(test_command, " -lpthread");
#endif

    run_command(test_command);

    printf("Test executable build completed.\n");

    free_file_list(source_files, src_count);
    free_file_list(header_files, header_count);
}

int main(int argc, char **argv) {
    build_project();
    return 0;
}
