```
pathToCanon(path)
Возвращает имя файла в каноническом виде
Например:
name -> ./name
folder/ -> ./folder/.
./folder/dddd -> ./folder/dddd

pathInDest(pathCan, dest)
Если path в начале dest, то возвращает 1
1 == nameToDest("./folder/.", "./folder/file")

isFolder(pathCon)
Папка ли (определяется по '/.' в конце)
0 = isFolder("./ololo")
1 = isFolder("./fol/.")

lelvels(pathCan)
Уровни вложенности конечного файла, например:
0 = levels("./file")
1 = levels("./folder/.")
1 = levels("./folder/123")
2 = levels("./folder/fol1/.")
2 = levels("./folder/fol1/lol")

getFileByPath(pathCan, dest)
Возвращает имя файла, например:
"./file"          == getFileByPath("./file", "./file")
"./ololo"         == getFileByPath("./fololo/azaza/.", "./fololo/azaza/ololo")
"./fol1/."        == getFileByPath("./fololo/azaza/.", "./fololo/azaza/fold1/.")
"./fldr/123"      == getFileByPath("./fololo/azaza/.", "./fololo/azaza/fldr/123")
```


