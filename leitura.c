#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura.h"

void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int ler_disciplinas(const char *filename, Disciplina disciplinas[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir Disciplinas.txt");
        exit(1);
    }

    int count = 0;
    char linha[300];

    while (fgets(linha, sizeof(linha), fp)) {
        remove_newline(linha);
        sscanf(linha, "%[^;];%[^;];%d;%d;%[^;];%[^\n]",
               disciplinas[count].codigo,
               disciplinas[count].nome,
               &disciplinas[count].carga_horaria,
               &disciplinas[count].periodo_ideal,
               disciplinas[count].tipo,
               disciplinas[count].prerequisitos);
        count++;
    }

    fclose(fp);
    return count;
}

int ler_horarios(const char *filename, Horario horarios[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir horarios.txt");
        exit(1);
    }

    int count = 0;
    char codigo[10], horario[20];

    while (fscanf(fp, "%s %s", codigo, horario) != EOF) {
        strcpy(horarios[count].codigo_disciplina, codigo);

        int i = 0;
        horarios[count].qtd_dias = 0;
        while (horario[i] >= '0' && horario[i] <= '9') {
            horarios[count].dias[horarios[count].qtd_dias++] = horario[i] - '0';
            i++;
        }

        horarios[count].turno = horario[i++];
        horarios[count].qtd_aulas = 0;
        while (horario[i] >= '0' && horario[i] <= '9') {
            horarios[count].aulas[horarios[count].qtd_aulas++] = horario[i] - '0';
            i++;
        }

        count++;
    }

    fclose(fp);
    return count;
}

void ler_historico(const char *filename, Aluno *aluno) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir historico.txt");
        exit(1);
    }

    char linha[200];
    fgets(aluno->nome, sizeof(aluno->nome), fp);
    remove_newline(aluno->nome);

    fgets(aluno->matricula, sizeof(aluno->matricula), fp);
    remove_newline(aluno->matricula);

    fgets(linha, sizeof(linha), fp);
    aluno->periodo_atual = atoi(linha);

    aluno->qtd_disciplinas = 0;
    while (fgets(linha, sizeof(linha), fp)) {
        remove_newline(linha);
        char *token = strtok(linha, ";");
        strcpy(aluno->historico[aluno->qtd_disciplinas].codigo, token);

        token = strtok(NULL, ";");
        aluno->historico[aluno->qtd_disciplinas].nota = atof(token);

        token = strtok(NULL, ";");
        if (token)
            strcpy(aluno->historico[aluno->qtd_disciplinas].status, token);
        else
            aluno->historico[aluno->qtd_disciplinas].status[0] = '\0';

        aluno->qtd_disciplinas++;
    }

    fclose(fp);
}