#include "hare.h"

int getFilesInfo(Context *cnt)
{
    int i;
    FileNode *tmpFileNode = cnt->filesTree;

    for(i = 0; !(cnt->workFiles)[i]; i++)
    {
        /* TODO: в объявлении функции FileInfo, передаёшь FileNode */
        if(0 != getFileInfo(cnt->workFiles[i], tmpFileNode->fileInfo))
        {
            /* ERROR! */
        }
        tmpFileNode = tmpFileNode->next;
    }

    return 0;
}
