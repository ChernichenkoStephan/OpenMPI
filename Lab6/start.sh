clear && $HOME/opt/usr/local/bin/mpicc -o test ./main.cpp Utils/utils.cpp Long_Word/long_Word.cpp && $HOME/opt/usr/local/bin/mpirun -np 4 ./test
