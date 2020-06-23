// Copyright (c) 2020 ZuoFuhong. All rights reserved.
// Use of this source code is governed by a MIT style
// license that can be found in the LICENSE file.

#include <stdio.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    DIR *dp;
    struct dirent *dirp;

    if (argc != 2) {
        printf("usage: ls directory_name");
        return 1;
    }

    if ((dp = opendir(argv[1])) == NULL) {
        printf("can't open %s", argv[1]);
        return 1;
    }
    while ((dirp = readdir(dp)) != NULL) {
        printf("%s\n", dirp ->d_name);
    }
    closedir(dp);
    return 0;
}
