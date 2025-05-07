#include <string.h>
#include <stdlib.h>
#include "conflitos.h"
#include "verificacao.h" /*para obter_prerequisitos*/

/*função auxiliar: pega os horarios*/
void obter_horarios_por_disciplina(Horario *horarios, int qtd_horarios, const char *codigo, Horario *resultados[], int *qtd) {
    *qtd = 0;
    for (int i = 0; i < qtd_horarios; i++) {
        if (strcmp(horarios[i].codigo_disciplina, codigo) == 0) {
            resultados[(*qtd)++] = &horarios[i];
        }
    }
}


/*ve se dois horarios especificos tem conflitos*/
int tem_conflito(Horario *h1, Horario *h2) {
    if (h1->turno != h2->turno) return 0;

    /*verifica os dias comuns*/
    int dia_comum = 0;
    for (int i = 0; i < h1->qtd_dias; i++) {
        for (int j = 0; j < h2->qtd_dias; j++) {
            if (h1->dias[i] == h2->dias[j]) {
                dia_comum = 1;
                break;
            }
        }
        if (dia_comum) break;
    }
    if (!dia_comum) return 0;

    /*verifica aulas*/
    for (int i = 0; i < h1->qtd_aulas; i++) {
        for (int j = 0; j < h2->qtd_aulas; j++) {
            if (h1->aulas[i] == h2->aulas[j]) {
                return 1;
            }
        }
    }
    return 0;
}

/*Verifica se duas disciplinas têm horários conflitantes*/
int disciplinas_conflitam(DisciplinaAlocada *d1, DisciplinaAlocada *d2, Horario *horarios, int qtd_horarios) {
    Horario *horarios_d1[100];
    int qtd_horarios_d1 = 0;
    obter_horarios_por_disciplina(horarios, qtd_horarios, d1->disciplina.codigo, horarios_d1, &qtd_horarios_d1);

    Horario *horarios_d2[100];
    int qtd_horarios_d2 = 0;
    obter_horarios_por_disciplina(horarios, qtd_horarios, d2->disciplina.codigo, horarios_d2, &qtd_horarios_d2);

    for (int i = 0; i < qtd_horarios_d1; i++) {
        for (int j = 0; j < qtd_horarios_d2; j++) {
            if (tem_conflito(horarios_d1[i], horarios_d2[j])) {
                return 1;
            }
        }
    }
    return 0;
}

/*função principal para resolver conflitos (ordena por código crescente e prioriza menor código)*/
void resolver_conflitos(DisciplinaAlocada *alocadas, int qtd_pendentes,  Horario *horarios, int qtd_horarios, int periodo_atual_aluno) {
    qsort(alocadas, qtd_pendentes, sizeof(DisciplinaAlocada), comparar_codigo_crescente);

    for (int i = 0; i < qtd_pendentes; i++) {
        /*Inicializa o período com base no período ideal ou atual do aluno*/
        int periodo_inicial = (alocadas[i].disciplina.periodo_ideal < periodo_atual_aluno) 
                                ? periodo_atual_aluno 
                                : alocadas[i].disciplina.periodo_ideal;
        int periodo_atual = periodo_inicial;

        /*verificar pré-requisitos*/
        char prereqs[10][10];
        int qtd_prereqs = obter_prerequisitos(alocadas[i].disciplina.prerequisitos, prereqs);

        /*Encontrar o último período dos pré-requisitos*/
        for (int p = 0; p < qtd_prereqs; p++) {
            for (int j = 0; j < qtd_pendentes; j++) {
                if (strcmp(alocadas[j].disciplina.codigo, prereqs[p]) == 0) {
                    if (alocadas[j].periodo_alocado >= periodo_atual) {
                        periodo_atual = alocadas[j].periodo_alocado + 1;
                    }
                }
            }
        }
        
        int conflito;
        /*resolver conflitos de horário*/ 
        do {
            conflito = 0;
            for (int j = 0; j < i; j++) {
                if (alocadas[j].periodo_alocado == periodo_atual) {
                    if (disciplinas_conflitam(&alocadas[i], &alocadas[j], horarios, qtd_horarios)) {
                        conflito = 1;
                        break;
                    }
                }
            }
            if (conflito) periodo_atual++;
        } while (conflito);

        alocadas[i].periodo_alocado = periodo_atual;
    }
}