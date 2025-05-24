#ifndef PROFESSOR_H
#define PROFESSOR_H

#include "leitura.h" // Included for Disciplina, Horario types
#include "conflitos.h" // Included for DisciplinaAlocada

#define MAX_PROFESSORES 100
#define MAX_DISCIPLINAS_PROF 20
#define MAX_FORMACAO 200
#define MAX_ESPECIALIZACOES 5

typedef struct {
    char nome[100];
    int carga_horaria; // em horas
    char formacao[MAX_FORMACAO];
    char especializacoes[MAX_ESPECIALIZACOES][100];
    int qtd_especializacoes;
    char disciplinas_habilitadas[MAX_DISCIPLINAS_PROF][10]; // códigos das disciplinas
    int qtd_disciplinas_habilitadas;
    char disciplinas_alocadas[MAX_DISCIPLINAS_PROF][10]; // disciplinas que o professor está lecionando
    int qtd_disciplinas_alocadas;
    int carga_alocada; // carga horária já alocada
} Professor;

// Função para ler o arquivo de professores
int ler_professores(const char *filename, Professor professores[]);

// Função para verificar se um professor já tem aula em um determinado dia/horário
int professor_tem_aula_no_horario(Professor *professor, Horario *horarios, int qtd_horarios, Horario *horario_verificar);

// Função para verificar se um professor pode lecionar uma disciplina
// MODIFICADO: Passa o struct Disciplina completo para verificar o periodo_ideal
int professor_pode_lecionar(Professor *professor, const Disciplina *disciplina);

// Função para alocar professores às disciplinas
void alocar_professores(Professor professores[], int qtd_professores,
                       DisciplinaAlocada disciplinas[], int qtd_disciplinas,
                       Horario *horarios, int qtd_horarios);

// Função para imprimir a alocação de professores
// MODIFICADO: Adiciona Horario *todos_horarios e int qtd_todos_horarios
void imprimir_alocacao_professores(Professor professores[], int qtd_professores,
                                  DisciplinaAlocada disciplinas_alocadas_array[], int qtd_disciplinas_alocadas,
                                  Horario *todos_horarios, int qtd_todos_horarios);

#endif