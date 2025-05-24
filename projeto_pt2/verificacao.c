#include <string.h>
#include <stdlib.h>
#include "verificacao.h"

// Verifica se o aluno foi aprovado em uma disciplina (nota >= 7 ou APRM)
int aluno_aprovado_em(Aluno *aluno, const char *codigo_disciplina) {
    for (int i = 0; i < aluno->qtd_disciplinas; i++) {
        if (strcmp(aluno->historico[i].codigo, codigo_disciplina) == 0) {
            float nota = aluno->historico[i].nota;
            char *status = aluno->historico[i].status;

            if (nota >= 7.0 || strcmp(status, "APRM") == 0) {
                return 1; // aprovado em alguma tentativa
            }
        }
    }
    return 0; // nenhuma tentativa teve aprovação
}

// Gera uma lista de disciplinas obrigatórias pendentes
int disciplinas_pendentes(
    Disciplina todas[], int qtd_disc,
    Aluno *aluno,
    Disciplina pendentes[]
) {
    int qtd_pendentes = 0;

    for (int i = 0; i < qtd_disc; i++) {
        // Só considera obrigatórias
        if (strcmp(todas[i].tipo, "OBRIGATORIA") != 0) continue;

        if (!aluno_aprovado_em(aluno, todas[i].codigo)) {
            pendentes[qtd_pendentes++] = todas[i];
        }
    }

    return qtd_pendentes;
}

/*função pra ordenar disciplinas por crescentes*/
int comparar_codigo_crescente(const void *a, const void *b) {
    const DisciplinaAlocada *da = a;
    const DisciplinaAlocada *db = b;
    return strcmp(da->disciplina.codigo, db->disciplina.codigo);
}

/*extrai pré-requisitos de uma string*/
int obter_prerequisitos(const char *str_prereq, char prereqs[][10]) {
    if (strcmp(str_prereq, "-") == 0) return 0; /*sem pré-requisitos*/

    char buffer[200];
    strcpy(buffer, str_prereq);
    char *token = strtok(buffer, ";");
    int count = 0;

    while (token != NULL && count < 10) {
        strcpy(prereqs[count++], token);
        token = strtok(NULL, ";");
    }
    return count;
}

