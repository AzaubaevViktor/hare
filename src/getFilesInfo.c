#include "hare.h"

int getFilesInfo(Context *cnt)
{
    int i;
    struct _FileNode *tmpFileNode = cnt->filesTree;

    for(i = 0; !(cnt->workFiles)[i]; i++)
    {
        if(0 != getFileInfo(cnt->workFiles[i], tmpFileNode->fileInfo))
        {
            /* ERROR! */
        }
        tmpFileNode = tmpFileNode->next;
    }

    return 0;
}
