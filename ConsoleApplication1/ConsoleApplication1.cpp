#include <windows.h>
#include <iostream>
#include <fileapi.h>
#include <errhandlingapi.h>
#include <fstream>
#include <string.h>

using namespace std;
//#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

void CopyFileStandart(std::string file1, std::string file2);
void CopyFileCF(std::string file1, std::string file2);
void CopyFileCW(std::string file1, std::string file2);

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        return 0;
    }
    std::string mode = argv[1]; // cpC, cpW, cpCF
    std::string filePath1 = argv[2];
    std::string filePath2 = argv[3];
    if (mode == "cpC")
    {
        CopyFileStandart(filePath1, filePath2);
    }
    else if (mode == "cpW")
    {
        CopyFileCW(filePath1, filePath2);
    }
    else if (mode == "cpCF")
    {
        CopyFileCF(filePath1, filePath2);
    }
    else if (mode == "All")
    {
        CopyFileStandart(filePath1, filePath2);
        CopyFileCW(filePath1, filePath2);
        CopyFileCF(filePath1, filePath2);
    }
    else
    {
        printf("Unknow command. Try: cpC, cpW, cpCF\n");
    }
    system("pause");
    return 0;
}

void CopyFileCF(std::string file1, std::string file2)
{
    DWORD start = GetTickCount64();
    CopyFileA(file1.c_str(), file2.c_str(), false);
    auto err = GetLastError();
    if (err != 0)
    {
        printf("Copy error %s in %s\nError: %d\n", file1.c_str(), file2.c_str(), GetLastError());
    }
    printf("time: %i\n", GetTickCount64() - start);
}

void CopyFileCW(std::string file1, std::string file2) {
    DWORD start = GetTickCount64();

    auto hFile1 = CreateFileA(file1.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    auto hFile2 = CreateFileA(file2.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile1 == INVALID_HANDLE_VALUE)
    {
        printf("Произошла ошибка при открытии %s \n", file1);
    }

    char buffer[256];
    DWORD fin, fout;

    while ((ReadFile(hFile1, buffer, 256, &fin, NULL)) > 0 && fin > 0)
    {
        WriteFile(hFile2, buffer, fin, &fout, NULL);
        if (fin != fout)
        {
            printf("Copy error %s in %s\nError: %d\n", file1.c_str(), file2.c_str(), GetLastError());
            return;
        }
    }
    CloseHandle(hFile1);
    CloseHandle(hFile2);
    printf("time: %i\n", GetTickCount64() - start);

}

void CopyFileStandart(std::string file1, std::string file2) {
    DWORD start = GetTickCount64();

    FILE *fin, *fout;
    char buffer[256];
    int bIN, bOut;

    fopen_s(&fin, file1.c_str(), "r");
    fopen_s(&fout, file2.c_str(), "w");

    if (fin == NULL || fout == NULL )
    {
        printf("Open error %s, %s\nError: %d\n", file1.c_str(), file2.c_str(), GetLastError());
        return;
    }

    while ((bIN = fread(buffer, 1, 256, fin)) > 0)
    {
        bOut = fwrite(buffer, 1, bIN, fout);
        if (bOut != bIN)
        {
            printf("Copy error %s in %s\nError: %d\n", file1.c_str(), file2.c_str(), GetLastError());
        }
    }

    fclose(fin);
    fclose(fout);
    printf("time: %i\n", GetTickCount64() - start);
}
