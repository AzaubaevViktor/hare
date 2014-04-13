```
pathToCanon(path) ЛЁША
Возвращает имя файла в каноническом виде
Например:
name -> ./name
folder/ -> ./folder/.
./folder/dddd -> ./folder/dddd
/file -> /file
/fold/ -> /fold/.
/fold/. -> /fold/.
/ -> /.


pathInDest(pathCan, dest)
Если path в начале dest, то возвращает 1
1 == nameInDest("./folder/.", "./folder/file")
1 == nameInDest("./folder", "./folder/file")
1 == nameInDest("./file", "./file")
0 == nameInDest("./file/.", "./file")
1 == nameInDest("/fold/.", "/fold/file")
0 == nameInDest("/fold/.", "./fold/file")

Папки и файлы придётся определять на лету, т.к. пользователь может указать папку как `folder/`, так и `folder`

lelvels(pathCan) ЛЁША
Уровни вложенности конечного файла, например:
0 = levels("./file")
1 = levels("./folder/.")
1 = levels("./folder/123")
2 = levels("./folder/fol1/.")
2 = levels("./folder/fol1/lol")

0 = levels("/.")
1 = levels("/fold/.")

getFileByPath(pathCan, dest)
Возвращает имя файла, например:
"./file"          == getFileByPath("./file", "./file")
"./ololo"         == getFileByPath("./fololo/azaza/.", "./fololo/azaza/ololo")
"./fol1/."        == getFileByPath("./fololo/azaza/.", "./fololo/azaza/fold1/.")
"./fldr/123"      == getFileByPath("./fololo/azaza/.", "./fololo/azaza/fldr/123")

isFolder(pathCan) ЛЁША
```


