#include <mpi.h>

#include "../Long_Word/long_word.h"

MPI_Datatype make_dto_type() {
  MPI_Datatype res;
  int fields_amount         = 2;
  MPI_Datatype old_types[2] = {MPI_INT, MPI_LONG};
  int          blocklens[2] = {MAX_WORD_LENGTH, 1};
  MPI_Aint displacements[2] = {
    offsetof(Long_Word_DTO, numbers),
    offsetof(Long_Word_DTO, word_length)
  };

  MPI_Type_create_struct(fields_amount, blocklens, displacements, old_types, &res);
  MPI_Type_commit(&res);
  return res;
}

void fill_words_array_test(Long_Word_DTO* array, int amount) {
  for (size_t i = 0; i < amount; i++) {
    array[i].word_length = 3;
    for (size_t j = 0; j < 3; j++)
      array[i].numbers[j] = 3 - j;
  }
}

void print_array(Long_Word* array, int length) {
  for (size_t i = 0; i < length; i++) Long_Word_print(array + i);
}

int send(Long_Word* word, int receiver, MPI_Datatype* type, MPI_Comm* comm) {
  Long_Word_DTO message = Long_Word_to_DTO(word);
  return MPI_Send(&message, 1, *type, receiver, 0, *comm);
}

int receive(Long_Word_DTO* message, int sender, MPI_Datatype* type, MPI_Comm* comm) {
  MPI_Status status;
  return MPI_Recv(message, 1, *type, sender, MPI_ANY_TAG, *comm, &status);
}
