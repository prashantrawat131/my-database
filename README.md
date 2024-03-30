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

## Key points about database
1. Every user has their own database file name according to their email id.

## Different Codes
### 1 to 100: Success Codes
1. 1: Success

### 101 to 200: Error Codes
1. 101: Key not found
2. 102: Key already present