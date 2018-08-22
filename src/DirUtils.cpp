#include "DirUtils.h"

bool DirUtils::dirExits(std::string path)
{
    DWORD ftype = GetFileAttributesA(path.c_str());
    if(ftype == INVALID_FILE_ATTRIBUTES)
    {
        std::cout << "Error: " << GetLastError() << "\n";
        return false;
    }
    return ftype & FILE_ATTRIBUTE_DIRECTORY;
}

void DirUtils::dirCreate(std::string path)
{
    //std::cout << "path: " << path << "\n";
    if(!dirExits(path))
    {
        CreateDirectory(path.c_str(), NULL);
    }
}

std::string DirUtils::getcwd()
{
    char buffer[200];
    GetModuleFileName(NULL, buffer, 200);
    std::string cwd(buffer);

    // The path returned by GetModuleFileName look like this: C:\data\prog.exe
    // And all this code does is to find the last occurence of "\" and remove
    // the chars from that occurence up to the end of the string
    size_t idx = cwd.rfind("\\");
    cwd.erase(idx, cwd.size() - idx);
    return cwd;
}

std::vector<std::string> DirUtils::getFiles(std::string path)
{
    std::vector<std::string> files;
    WIN32_FIND_DATA fd;
    HANDLE fh = FindFirstFile((path + "/*.bmp").c_str(), &fd);

    if(fh == INVALID_HANDLE_VALUE)
    {
        std::cout << "Can't file first file\n";
    }
    else
    {
        do
        {
            if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                files.push_back(path + "\\" + fd.cFileName);
            }
        }while(FindNextFile(fh, &fd));
    }
    return files;
}
