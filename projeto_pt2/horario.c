#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "horario.h"
#include "leitura.h"

// Função para interpretar os dias da semana
void interpretar_dias(const char *dias_str, char *resultado) {
    char dias_semana[8][15] = {"", "Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
    
    // Inicializar resultado
    resultado[0] = '\0';
    
    // Verificar se a string é vazia ou apenas um traço
    if (dias_str == NULL || strlen(dias_str) == 0 || strcmp(dias_str, "-") == 0) {
        strcpy(resultado, "Nao definido");
        return;
    }
    
    // Analisar os dias
    char dias_buffer[100] = "";
    int i = 0;
    
    // Extrair dias
    while (i < strlen(dias_str) && isdigit(dias_str[i])) {
        int dia = dias_str[i] - '0';
        if (dia >= 1 && dia <= 7) {
            if (strlen(dias_buffer) > 0) {
                strcat(dias_buffer, ", ");
            }
            strcat(dias_buffer, dias_semana[dia]);
        }
        i++;
    }
    
    // Extrair turno (M, T, N)
    char turno = '\0';
    if (i < strlen(dias_str)) {
        turno = dias_str[i];
        i++;
    }
    
    // Extrair períodos de aula
    char aulas_buffer[100] = "";
    while (i < strlen(dias_str) && isdigit(dias_str[i])) {
        if (strlen(aulas_buffer) > 0) {
            strcat(aulas_buffer, ", ");
        }
        char aula_str[2] = {dias_str[i], '\0'};
        strcat(aulas_buffer, aula_str);
        i++;
    }
    
    // Combinar todas as informações
    sprintf(resultado, "%s - ", dias_buffer);
    
    if (turno == 'M') {
        strcat(resultado, "Manha");
    } else if (turno == 'T') {
        strcat(resultado, "Tarde");
    } else if (turno == 'N') {
        strcat(resultado, "Noite");
    } else {
        strcat(resultado, "Turno nao definido");
    }
    
    if (strlen(aulas_buffer) > 0) {
        strcat(resultado, " (Aulas: ");
        strcat(resultado, aulas_buffer);
        strcat(resultado, ")");
    }
}

void interpretar_dias_horario(const Horario *horario, char *resultado) {
    char dias_semana[8][15] = {"", "Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};
    
    // Inicializar resultado
    resultado[0] = '\0';
    
    // Verificar se há dias definidos
    if (horario->qtd_dias == 0) {
        strcpy(resultado, "Nao definido");
        return;
    }
    
    // Extrair dias
    char dias_buffer[100] = "";
    for (int i = 0; i < horario->qtd_dias; i++) {
        int dia = horario->dias[i];
        if (dia >= 1 && dia <= 7) {
            if (strlen(dias_buffer) > 0) {
                strcat(dias_buffer, ", ");
            }
            strcat(dias_buffer, dias_semana[dia]);
        }
    }
    
    // Extrair turno
    char turno_buffer[20] = "";
    if (horario->turno == 'M') {
        strcpy(turno_buffer, "Manha");
    } else if (horario->turno == 'T') {
        strcpy(turno_buffer, "Tarde");
    } else if (horario->turno == 'N') {
        strcpy(turno_buffer, "Noite");
    } else {
        strcpy(turno_buffer, "Turno nao definido");
    }
    
    // Extrair aulas
    char aulas_buffer[100] = "";
    for (int i = 0; i < horario->qtd_aulas; i++) {
        if (strlen(aulas_buffer) > 0) {
            strcat(aulas_buffer, ", ");
        }
        char aula_str[3];
        sprintf(aula_str, "%d", horario->aulas[i]);
        strcat(aulas_buffer, aula_str);
    }
    
    // Combinar todas as informações
    sprintf(resultado, "%s - %s", dias_buffer, turno_buffer);
    
    if (strlen(aulas_buffer) > 0) {
        strcat(resultado, " (Aulas: ");
        strcat(resultado, aulas_buffer);
        strcat(resultado, ")");
    }
}

// Função para imprimir horários detalhados
void imprimir_horarios_detalhados(Horario *horarios, int qtd_horarios) {
    printf("\n===== HORARIOS DAS AULAS (DETALHADO) =====\n\n");
    
    for (int i = 0; i < qtd_horarios; i++) {
        char dias_interpretados[200];
        
        // Criar uma string com o formato "DDTAA" para interpretar
        char dias_str[20] = "";
        for (int j = 0; j < horarios[i].qtd_dias; j++) {
            char dia_str[2] = {horarios[i].dias[j] + '0', '\0'};
            strcat(dias_str, dia_str);
        }
        
        if (horarios[i].turno != '\0') {
            char turno_str[2] = {horarios[i].turno, '\0'};
            strcat(dias_str, turno_str);
        }
        
        for (int j = 0; j < horarios[i].qtd_aulas; j++) {
            char aula_str[2] = {horarios[i].aulas[j] + '0', '\0'};
            strcat(dias_str, aula_str);
        }
        
        interpretar_dias(dias_str, dias_interpretados);
        
        printf("Disciplina: %s\n", horarios[i].codigo_disciplina);
        printf("Dias/Horario: %s\n", dias_interpretados);
        printf("Local: %s\n", 
               strlen(horarios[i].local) > 0 && strcmp(horarios[i].local, "-") != 0 ? 
                   horarios[i].local : "Nao definido");
        printf("Horario: %s - %s\n", 
               strlen(horarios[i].hora_inicial) > 0 && strcmp(horarios[i].hora_inicial, "-") != 0 ? 
                   horarios[i].hora_inicial : "Nao definido",
               strlen(horarios[i].hora_final) > 0 && strcmp(horarios[i].hora_final, "-") != 0 ? 
                   horarios[i].hora_final : "Nao definido");
        printf("Professor: %s\n", 
               strlen(horarios[i].professor) > 0 && strcmp(horarios[i].professor, "-") != 0 ? 
                   horarios[i].professor : "Nao definido");
        printf("----------------------------------------\n");
    }
}

// Função para imprimir os horários de uma disciplina específica
void imprimir_horarios_disciplina_detalhado(const char *codigo, const char *nome, Horario *horarios, int qtd_horarios) {
    int encontrou = 0;
    
    for (int i = 0; i < qtd_horarios; i++) {
        if (strcmp(horarios[i].codigo_disciplina, codigo) == 0) {
            char dias_interpretados[200];
            
            // Criar uma string com o formato "DDTAA" para interpretar
            char dias_str[20] = "";
            for (int j = 0; j < horarios[i].qtd_dias; j++) {
                char dia_str[2] = {horarios[i].dias[j] + '0', '\0'};
                strcat(dias_str, dia_str);
            }
            
            if (horarios[i].turno != '\0') {
                char turno_str[2] = {horarios[i].turno, '\0'};
                strcat(dias_str, turno_str);
            }
            
            for (int j = 0; j < horarios[i].qtd_aulas; j++) {
                char aula_str[2] = {horarios[i].aulas[j] + '0', '\0'};
                strcat(dias_str, aula_str);
            }
            
            interpretar_dias(dias_str, dias_interpretados);
            
            printf("  %s - %s\n", codigo, nome);
            printf("    Dias/Horario: %s\n", dias_interpretados);
            printf("    Local: %s\n", 
                   strlen(horarios[i].local) > 0 && strcmp(horarios[i].local, "-") != 0 ? 
                       horarios[i].local : "Nao definido");
            printf("    Horario: %s - %s\n", 
                   strlen(horarios[i].hora_inicial) > 0 && strcmp(horarios[i].hora_inicial, "-") != 0 ? 
                       horarios[i].hora_inicial : "Nao definido",
                   strlen(horarios[i].hora_final) > 0 && strcmp(horarios[i].hora_final, "-") != 0 ? 
                       horarios[i].hora_final : "Nao definido");
            printf("    Professor: %s\n", 
                   strlen(horarios[i].professor) > 0 && strcmp(horarios[i].professor, "-") != 0 ? 
                       horarios[i].professor : "Nao definido");
            
            encontrou = 1;
        }
    }
    
    if (!encontrou) {
        printf("  %s - %s (Sem horario definido)\n", codigo, nome);
    }
}