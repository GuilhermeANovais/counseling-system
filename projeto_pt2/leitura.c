#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "leitura.h"

void remove_newline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

int ler_disciplinas(const char *filename, Disciplina disciplinas[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir Disciplinas.txt");
        exit(1); // Or return 0 and handle in main
    }

    int count = 0;
    char linha[300];
    char linha_original_para_debug[300]; // For error messages

    while (fgets(linha, sizeof(linha), fp) && count < MAX_DISCS) {
        strcpy(linha_original_para_debug, linha); // Keep original for debug messages
        remove_newline(linha);

        if (linha[0] == '#' || strlen(linha) < 5) { // Skip comments and very short lines
            continue;
        }

        // Initialize struct fields
        strcpy(disciplinas[count].codigo, "");
        strcpy(disciplinas[count].nome, "");
        disciplinas[count].carga_horaria = 0;
        disciplinas[count].periodo_ideal = 0;
        strcpy(disciplinas[count].tipo, "");
        strcpy(disciplinas[count].enfase, ""); // Default to empty string
        strcpy(disciplinas[count].prerequisitos, "-"); // Default to no prerequisites

        char *parts[6] = {NULL, NULL, NULL, NULL, NULL, NULL}; // Max 6 expected fields
        int p_idx = 0;
        char *token = strtok(linha, ";");
        while (token != NULL && p_idx < 6) {
            parts[p_idx++] = token;
            token = strtok(NULL, ";");
        }

        if (p_idx < 5) { // Need at least 5 fields (COD;NOME;CH;PERIODO_OR_TIPO;TIPO_OR_ENFASE)
            fprintf(stderr, "AVISO: Linha de disciplina com formato invalido (poucos campos): %s\n", linha_original_para_debug);
            continue; // Skip this malformed line
        }

        if (parts[0]) strcpy(disciplinas[count].codigo, parts[0]);
        if (parts[1]) strcpy(disciplinas[count].nome, parts[1]);
        if (parts[2]) disciplinas[count].carga_horaria = atoi(parts[2]);

        // Check 4th field to determine if OBRIGATORIA or ELETIVA
        // Format OBRIGATORIA: COD;NOME;CH;PERIODO_INT;OBRIGATORIA_STR;PREREQ_STR (6 fields, or 5 if no prereq)
        // Format ELETIVA:    COD;NOME;CH;ELETIVA_STR;ENFASE_STR;PREREQ_STR (6 fields)
        if (parts[3] && strcmp(parts[3], "ELETIVA") == 0) { // It's an ELETIVA
            strcpy(disciplinas[count].tipo, parts[3]); // "ELETIVA"
            if (parts[4]) {
                strcpy(disciplinas[count].enfase, parts[4]); // Emphasis name
            }
            if (parts[5]) {
                strcpy(disciplinas[count].prerequisitos, parts[5]);
            }
            disciplinas[count].periodo_ideal = 0; // Electives might not have a strict ideal period
        } else if (parts[3] && (isdigit(parts[3][0]) || (parts[3][0] == '-' && strlen(parts[3]) > 1 && isdigit(parts[3][1])) ) ) { // It's an OBRIGATORIA with an integer period
            disciplinas[count].periodo_ideal = atoi(parts[3]);
            if (parts[4]) {
                strcpy(disciplinas[count].tipo, parts[4]); // Should be "OBRIGATORIA"
            }
            if (parts[5]) {
                strcpy(disciplinas[count].prerequisitos, parts[5]);
            }
        } else {
            fprintf(stderr, "AVISO: Formato do 4º campo da disciplina nao reconhecido: %s em linha: %s\n", parts[3] ? parts[3] : "NULO", linha_original_para_debug);
            continue; // Skip this problematic line
        }
        count++;
    }

    fclose(fp);
    return count;
}

// Função original para ler horários no formato antigo
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

        // Inicializar os novos campos com valores vazios
        strcpy(horarios[count].local, "");
        strcpy(horarios[count].hora_inicial, "");
        strcpy(horarios[count].hora_final, "");
        strcpy(horarios[count].professor, "");

        count++;
    }

    fclose(fp);
    return count;
}

// Nova função para ler horários no formato detalhado
int ler_horarios_detalhados(const char *filename, Horario horarios[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir o arquivo de horários");
        return 0;
    }

    int count = 0;
    char linha[300];

    while (fgets(linha, sizeof(linha), fp) && count < MAX_HORARIOS) {
        remove_newline(linha);
        
        // Pular linhas vazias
        if (strlen(linha) <= 1) continue;
        
        // Inicializar todos os campos com strings vazias
        strcpy(horarios[count].codigo_disciplina, "");
        strcpy(horarios[count].local, "");
        strcpy(horarios[count].hora_inicial, "");
        strcpy(horarios[count].hora_final, "");
        strcpy(horarios[count].professor, "");
        
        // Inicializar arrays
        horarios[count].qtd_dias = 0;
        horarios[count].qtd_aulas = 0;
        
        // Analisar a linha
        char *token;
        int field = 0;
        
        token = strtok(linha, ";");
        while (token != NULL && field < 6) {
            // Remover espaços no início e fim
            while (*token == ' ') token++;
            char *end = token + strlen(token) - 1;
            while (end > token && *end == ' ') {
                *end = '\0';
                end--;
            }
            
            switch (field) {
                case 0: // Código da disciplina
                    strcpy(horarios[count].codigo_disciplina, token);
                    break;
                case 1: // Dias/Horário
                    {
                        // Extrair dias e horários do formato (ex: 6T12)
                        int i = 0;
                        horarios[count].qtd_dias = 0;
                        
                        // Extrair dias
                        while (i < strlen(token) && token[i] >= '0' && token[i] <= '9') {
                            horarios[count].dias[horarios[count].qtd_dias++] = token[i] - '0';
                            i++;
                        }
                        
                        // Extrair turno
                        if (i < strlen(token)) {
                            horarios[count].turno = token[i++];
                        } else {
                            horarios[count].turno = ' ';
                        }
                        
                        // Extrair aulas
                        horarios[count].qtd_aulas = 0;
                        while (i < strlen(token) && token[i] >= '0' && token[i] <= '9') {
                            horarios[count].aulas[horarios[count].qtd_aulas++] = token[i] - '0';
                            i++;
                        }
                    }
                    break;
                case 2: // Local
                    strcpy(horarios[count].local, token);
                    break;
                case 3: // Hora inicial
                    strcpy(horarios[count].hora_inicial, token);
                    break;
                case 4: // Hora final
                    strcpy(horarios[count].hora_final, token);
                    break;
                case 5: // Professor
                    strcpy(horarios[count].professor, token);
                    break;
            }
            
            token = strtok(NULL, ";");
            field++;
        }
        
        // Verificar se temos pelo menos o código da disciplina
        if (strlen(horarios[count].codigo_disciplina) > 0) {
            count++;
        }
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
