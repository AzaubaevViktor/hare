Сначала:
```
Нужно написать функцию, через которую будут идти блоки байт, а она их будет преобразовывать, например:
// Название можешь поменять на другое
isEnd = coding(file, returnBytes)
// Что она делает: кодирует каким-либо образом данные
```

С тебя требуется:
```
Передаётся:
оффсет, откуда писать
header
file, который нужно закодировать

Возвращаешь:
код возврата

Имена файлов:
Внутри архива только *файлы*.
В каждой папке есть миннимум один файл "."

То есть, если у нас есть такая структура:
./
  fol_lol/
    file1
    file2
  fol_azaza/
    oh_lol/
      file3
    uhuhu/
    azaza1
    azaza2
    
То внутри архива она станет такой:

./fol_lol/.
./fol_lol/file1
./fol_lol/file2
./fol_azaza/.
./fol_azaza/oh_ool/.
./fol_azaza/oh_ool/file3
./fol_azaza/uhuhu/.
./fol_azaza/azaza1
./fol_azaza/azaza2

./fol_lol/.
./fol_azaza/.
./fol_lol/file2
./fol_azaza/azaza2
./fol_lol/file1
./fol_azaza/oh_ool/.
./fol_azaza/oh_ool/file3
./fol_azaza/uhuhu/.
./fol_azaza/azaza1
```
