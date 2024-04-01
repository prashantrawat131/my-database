# rm -r ./databases/*

g++ .././db.cpp -o a

# If the compilation was successfull then echo ok
if [ $? -eq 0 ]; then
    echo "Compilation successful"
else
    echo "Compilation failed"
    exit 1
fi

../a.exe put apple Keeps the doctor away.


../a.exe put guava It is also healthy.

echo "Data inserted successfully"

echo "Press any key to continue"
read -n1 -s