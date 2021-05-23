clear && $HOME/opt/usr/local/bin/mpicc -o test ./main.c ./Long_Word/long_word.c ./Utils/utils.c && $HOME/opt/usr/local/bin/mpirun -np 4 ./test
