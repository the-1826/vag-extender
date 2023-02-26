#include <stdio.h>
#include <string>

using namespace std;

int main() {
    for (int i = 1; i < 1001; i++) {
        FILE* sourcefile = fopen(('s' + to_string(i) + ".vag").c_str(), "rb+");
        if (!sourcefile) {
            printf("Can't open source file in set %d.\n", i);
            continue;
        }
        FILE* targetfile = fopen(('t' + to_string(i) + ".vag").c_str(), "rb+");
        if (!targetfile) {
            printf("Can't open target file in set %d.\n", i);
            continue;
        }
        fseek(sourcefile, 0L, SEEK_END);
        int source_length = ftell(sourcefile);
        rewind(sourcefile);
        fseek(targetfile, 0L, SEEK_END);
        int target_length = ftell(targetfile);
        rewind(targetfile);
        int delta = target_length - source_length;
        if (delta < 0) {
            printf("Target file is smaller than the original one. Please change files.\n");
            continue;
        }
        fclose(targetfile);
        fseek(sourcefile, 0, SEEK_SET);
        FILE* newfile = fopen(('n' + to_string(i) + ".vag").c_str(), "wb");
        char buffer[2], temp;
        int found = 0;
        for (int j = 0; j < source_length; j++) {
            fseek(sourcefile, j, SEEK_SET);
            fread(&temp, 1, 1, sourcefile);
            fwrite(&temp, 1, 1, newfile);
            fseek(sourcefile, j + 1, SEEK_SET);
            fread(&buffer[0], 1, 1, sourcefile);
            fseek(sourcefile, j + 2, SEEK_SET);
            fread(&buffer[1], 1, 1, sourcefile);
            if ((buffer[0] == 7) && (buffer[1] == 119)) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            printf("Bad source .vag, can't change file weight.\n");
            continue;
        }
        temp = 0;
        for (int j = 0; j < delta; j++)
            fwrite(&temp, 1, 1, newfile);
        char last[15] = {'', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w', 'w'};
        fwrite(last, 15, 1, newfile);if (found == 0) {
            printf("Bad source .vag, can't change file weight.\n");
            continue;
        }
        fclose(sourcefile);
        fclose(newfile);
        if(remove(('s' + to_string(i) + ".vag").c_str()) != 0 ) {
            printf("Error deleting original file.");
            break;
        }
        int result = rename(('n' + to_string(i) + ".vag").c_str(), ('s' + to_string(i) + ".vag").c_str());
        if (result != 0) {
            printf("The file could not be renamed.");
            break;
        }
        printf("File set %d switched.\n", i);
    }
    system("pause");
}
