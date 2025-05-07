#ifndef CONFLITOS_H
#define CONFLITOS_H

#include "leitura.h"

typedef struct {
    Disciplina disciplina;
    int periodo_alocado;
} DisciplinaAlocada;

// Funções de resolução de conflitos
void resolver_conflitos(DisciplinaAlocada *alocadas, int qtd_pendentes, Horario *horarios, int qtd_horarios, int periodo_atual_aluno);

int disciplinas_conflitam(DisciplinaAlocada *d1, DisciplinaAlocada *d2, Horario *horarios, int qtd_horarios);

int tem_conflito(Horario *h1, Horario *h2);

void obter_horarios_por_disciplina(Horario *horarios, int qtd_horarios, const char *codigo, Horario *resultados[], int *qtd);

#endif