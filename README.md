# My Database

## my-database
This is a key-value store data base to store data in disk. 

## Commands
1. Get: Get the value corresponding to a key.
> Syntax: `GET <key>`

2. Set: Set the value corresponding to a key.
> Syntax: `SET <key> <value>`
Everthing after the key is considered as value and value could not have newline character (\n).

3. Delete: Delete the key-value pair corresponding to a key.
> Syntax: `DELETE <key>`

## Contents of the DB file
The contents of the DB file are stored in the following format:
```
hash value for key1
key1
value1
hash value for key2
key2
value2
...
```

## Different Codes
Visit [this](./include/codes.h) file to see the different codes used in the project. They are self explanatory.