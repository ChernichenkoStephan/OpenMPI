clear && $HOME/opt/usr/local/bin/mpicc -o test ./main.c ./Utils/utils.c && $HOME/opt/usr/local/bin/mpirun -np 4 ./test
