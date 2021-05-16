clear && $HOME/opt/usr/local/bin/mpicc -o test ./main.c ./Matrix/matrix.c && $HOME/opt/usr/local/bin/mpirun -np 4 ./test
