#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <vector>
#include "dirent.h"

using std::cout; using std::cin;
using std::endl; using std::vector;
using std::string;

int main() {
    DIR* dir; struct dirent* diread;
    vector<std::string> files;
    if ((dir = opendir("/Users/ghufr/source/repos/fileList 
experiment")) != nullptr) {
        while ((diread= readdir(dir)) != nullptr) {
            files.push_back((*diread).d_name);
        }
     closedir(dir);
    }
    else {
        perror("opendir");
        return EXIT_FAILURE;
    }

    for (int i = 2; i < files.size(); i++)
    {
        string f = files[i];
        for (int j = 0; j<f.size()&&f[j] != '.'; j++)
            cout << f[j];
        cout << " ";
   }

    return EXIT_SUCCESS;
}

