#ifndef LEITURA_H
#define LEITURA_H

#define MAX_DISCS 100
#define MAX_HORARIOS 100
#define MAX_HISTORICO 100

// ---------- Structs ----------
typedef struct {
    char codigo[10];
    char nome[100];
    int carga_horaria;
    int periodo_ideal;
    char tipo[15];
    char prerequisitos[200];
} Disciplina;

typedef struct {
    char codigo_disciplina[10];
    int dias[7];
    int qtd_dias;
    char turno;
    int aulas[6];
    int qtd_aulas;
} Horario;

typedef struct {
    char codigo[10];
    float nota;
    char status[10];
} DisciplinaCursada;

typedef struct {
    char nome[100];
    char matricula[20];
    int periodo_atual;
    DisciplinaCursada historico[MAX_HISTORICO];
    int qtd_disciplinas;
} Aluno;

// ---------- Funções ----------
int ler_disciplinas(const char *filename, Disciplina disciplinas[]);
int ler_horarios(const char *filename, Horario horarios[]);
void ler_historico(const char *filename, Aluno *aluno);

#endif