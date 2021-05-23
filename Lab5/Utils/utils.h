#pragma once

/*
* Fabric function for MPI custom data type
* creates MPI_Datatype type for Long_Word_DTO
*/
MPI_Datatype make_dto_type();

/*
* Test dataset fabric function
* return: {123, 123, 123, 123, 123, 123, 123, 123}
*/
void fill_words_array_test(Long_Word_DTO* array, int amount);

/*
* Wrap for Long_Word_print to print array
*/
void print_array(Long_Word* array, int length);

/*
* Wrap for MPI_Send exchange Long_Word_DTO objects
*/
int send(Long_Word* word, int receiver, MPI_Datatype* type, MPI_Comm* comm);

/*
* Wrap for MPI_Recv exchange Long_Word_DTO objects
*/
int receive(Long_Word_DTO* message, int sender, MPI_Datatype* type, MPI_Comm* comm);
