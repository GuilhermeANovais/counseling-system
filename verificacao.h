#ifndef VERIFICACAO_H
#define VERIFICACAO_H

#include "leitura.h"
#include "conflitos.h"

int disciplinas_pendentes(Disciplina todas[], int qtd_disc, Aluno *aluno, Disciplina pendentes[]);

int aluno_aprovado_em(Aluno *aluno, const char *codigo_disciplina);

int comparar_codigo_crescente(const void *a, const void *b);

int obter_prerequisitos(const char *str_prereq, char prereqs[][10]);

#endif