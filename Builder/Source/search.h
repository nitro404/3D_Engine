#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include <iostream>

#include <windows.h>
#include <conio.h>

int searchForMaps(vector<string> &refvecFiles,
                  const string   &refcstrRootDirectory,
                  bool                bSearchSubdirectories = false)
{
  string     strFilePath;             // Filepath
  string     strPattern;              // Pattern
  string     strFileName;
  HANDLE          hFile;                   // Handle to file
  WIN32_FIND_DATA FileInformation;         // File information


  strPattern = refcstrRootDirectory + "\\*.uni";

  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
      if(FileInformation.cFileName[0] != '.')
      {
        strFilePath.erase();
        strFilePath = refcstrRootDirectory + "\\" + FileInformation.cFileName;

        if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
          if(bSearchSubdirectories)
          {
            // Search subdirectory
            int iRC = searchForMaps(refvecFiles,
                                    strFilePath,
                                    bSearchSubdirectories);
            if(iRC)
              return iRC;
          }
        }
        else
        {
			strFileName = FileInformation.cFileName;
			
			// Save filename
			refvecFiles.push_back(strFilePath);
        }
      }
    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);

    DWORD dwError = ::GetLastError();
    if(dwError != ERROR_NO_MORE_FILES)
      return dwError;
  }

  return 0;
}

#endif