#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "professor.h"
#include "conflitos.h" 
#include "horario.h"   

// Função auxiliar para remover espaços em branco no início e fim de uma string
void trim(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    while (*start && (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n')) start++;
    
    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
    
    int len = strlen(str);
    while (len > 0 && (str[len-1] == ' ' || str[len-1] == '\t' || str[len-1] == '\r' || str[len-1] == '\n')) { // Added \r
        str[--len] = '\0';
    }
}

// Função para ler o arquivo de professores usando strtok
int ler_professores(const char *filename, Professor professores[]) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo de professores");
        return 0;
    }

    int count = 0;
    char linha[1000];

    while (fgets(linha, sizeof(linha), fp) && count < MAX_PROFESSORES) {
        linha[strcspn(linha, "\n\r")] = '\0';
        if (strlen(linha) < 5) continue;

        memset(&professores[count], 0, sizeof(Professor));

        char *token_ptr;
        char *linha_mutavel = linha;

        // 1. Nome do professor
        token_ptr = strtok(linha_mutavel, ";");
        if (!token_ptr) continue;
        strcpy(professores[count].nome, token_ptr);
        trim(professores[count].nome);

        // 2. Carga horária - MODIFIED HANDLING
        token_ptr = strtok(NULL, ";");
        if (!token_ptr) { // Should not happen based on format, but good to be safe
            printf("AVISO: Token de carga horaria ausente para %s. Assumindo 360h.\n", professores[count].nome);
            professores[count].carga_horaria = 360;
        } else {
            char carga_num_str[10] = "";
            int k = 0;
            for (int char_idx = 0; token_ptr[char_idx] != '\0'; char_idx++) {
                if (isdigit(token_ptr[char_idx])) {
                    if (k < 9) { // Prevent buffer overflow for carga_num_str
                        carga_num_str[k++] = token_ptr[char_idx];
                    }
                }
            }
            carga_num_str[k] = '\0';

            if (k > 0) { // Digits were found
                professores[count].carga_horaria = atoi(carga_num_str);
                if (professores[count].carga_horaria == 0) {
                     printf("AVISO: Carga horaria para %s parseada como 0 a partir de '%s'. Verifique o arquivo. Assumindo 360h para alocacao.\n", professores[count].nome, token_ptr);
                     professores[count].carga_horaria = 360;
                }
            } else {
                printf("AVISO: Carga horaria para %s ('%s') nao contem numeros. Assumindo carga padrao alta (360h).\n", professores[count].nome, token_ptr);
                professores[count].carga_horaria = 360;
            }
        }
        professores[count].carga_alocada = 0;

        // 3. Formação e especializações (até encontrar '|')
        char formacao_completa[MAX_FORMACAO] = "";
        professores[count].qtd_especializacoes = 0;
        char *disciplinas_habilitadas_str = NULL;

        while ((token_ptr = strtok(NULL, ";")) != NULL) {
            char *pipe_pos = strchr(token_ptr, '|');
            if (pipe_pos != NULL) {
                *pipe_pos = '\0';
                trim(token_ptr);
                if (strlen(token_ptr) > 0) {
                    if (professores[count].qtd_especializacoes < MAX_ESPECIALIZACOES) {
                        strcpy(professores[count].especializacoes[professores[count].qtd_especializacoes], token_ptr);
                        professores[count].qtd_especializacoes++;
                    }
                    if (strlen(formacao_completa) > 0) strcat(formacao_completa, "; ");
                    strcat(formacao_completa, token_ptr);
                }
                disciplinas_habilitadas_str = pipe_pos + 1;
                break;
            } else {
                trim(token_ptr);
                 if (strlen(token_ptr) > 0) {
                    if (professores[count].qtd_especializacoes < MAX_ESPECIALIZACOES) {
                        strcpy(professores[count].especializacoes[professores[count].qtd_especializacoes], token_ptr);
                        professores[count].qtd_especializacoes++;
                    }
                    if (strlen(formacao_completa) > 0) strcat(formacao_completa, "; ");
                    strcat(formacao_completa, token_ptr);
                }
            }
        }
        strcpy(professores[count].formacao, formacao_completa);

        // 4. Disciplinas habilitadas
        professores[count].qtd_disciplinas_habilitadas = 0;
        if (disciplinas_habilitadas_str != NULL && strlen(disciplinas_habilitadas_str) > 0) {
            char *disciplina_token = strtok(disciplinas_habilitadas_str, ";,");
            while (disciplina_token != NULL && professores[count].qtd_disciplinas_habilitadas < MAX_DISCIPLINAS_PROF) {
                trim(disciplina_token);
                if(strlen(disciplina_token) > 0) {
                    strcpy(professores[count].disciplinas_habilitadas[professores[count].qtd_disciplinas_habilitadas], disciplina_token);
                    professores[count].qtd_disciplinas_habilitadas++;
                }
                disciplina_token = strtok(NULL, ";,");
            }
        }
        professores[count].qtd_disciplinas_alocadas = 0; // Inicializa alocar_professores
        count++;
    }

    fclose(fp);
    return count;
}

// Função para verificar se um professor já tem aula em um determinado dia/horário
int professor_tem_aula_no_horario(Professor *professor, Horario *horarios, int qtd_horarios, Horario *horario_verificar) {
    for (int i = 0; i < professor->qtd_disciplinas_alocadas; i++) {
        Horario *horarios_disciplina[MAX_HORARIOS]; 
        int qtd_horarios_disciplina = 0;
        
        obter_horarios_por_disciplina(horarios, qtd_horarios,
                                     professor->disciplinas_alocadas[i],
                                     horarios_disciplina,
                                     &qtd_horarios_disciplina);
        
        for (int j = 0; j < qtd_horarios_disciplina; j++) {
            if (tem_conflito(horarios_disciplina[j], horario_verificar)) {
                return 1;
            }
        }
    }
    return 0;
}

// Função para verificar se um professor pode lecionar uma disciplina
int professor_pode_lecionar(Professor *professor, const Disciplina *disciplina) {

 // Verifica as habilitações listadas para o professor
    for (int i = 0; i < professor->qtd_disciplinas_habilitadas; i++) {
        if (strcmp(professor->disciplinas_habilitadas[i], disciplina->codigo) == 0) {
            return 1; // Pode lecionar
        }
    }
    return 0; // Não pode lecionar

    // Para disciplinas a partir do 5º período, verificar habilitações
    for (int i = 0; i < professor->qtd_disciplinas_habilitadas; i++) {
        if (strcmp(professor->disciplinas_habilitadas[i], disciplina->codigo) == 0) {
            return 1; // Pode lecionar
        }
    }
    return 0; // Não pode lecionar
}

// Função para alocar professores às disciplinas
void alocar_professores(Professor professores[], int qtd_professores,
                       DisciplinaAlocada disciplinas[], int qtd_disciplinas,
                       Horario *horarios, int qtd_horarios) {
    int max_periodo = 0;
    for (int i = 0; i < qtd_disciplinas; i++) {
        if (disciplinas[i].periodo_alocado > max_periodo) {
            max_periodo = disciplinas[i].periodo_alocado;
        }
    }
    
    for (int periodo = 1; periodo <= max_periodo; periodo++) {
        for (int i = 0; i < qtd_disciplinas; i++) {
            if (disciplinas[i].periodo_alocado == periodo) {
                int ja_alocada_a_professor = 0;
                for(int p_idx = 0; p_idx < qtd_professores; ++p_idx) {
                    for(int da_idx = 0; da_idx < professores[p_idx].qtd_disciplinas_alocadas; ++da_idx) {
                        if(strcmp(professores[p_idx].disciplinas_alocadas[da_idx], disciplinas[i].disciplina.codigo) == 0) {
                            ja_alocada_a_professor = 1;
                            break;
                        }
                    }
                    if(ja_alocada_a_professor) break;
                }
                if(ja_alocada_a_professor) continue;


                const char *codigo_disciplina = disciplinas[i].disciplina.codigo;
                int carga_horaria_disciplina = disciplinas[i].disciplina.carga_horaria;
                
                Horario *horarios_da_disciplina_atual[MAX_HORARIOS];
                int qtd_horarios_da_disciplina_atual = 0;
                obter_horarios_por_disciplina(horarios, qtd_horarios,
                                             codigo_disciplina,
                                             horarios_da_disciplina_atual,
                                             &qtd_horarios_da_disciplina_atual);
                
                int melhor_professor_idx = -1;
                int menor_carga_alocada_do_melhor = 999999;
                
                for (int j_prof = 0; j_prof < qtd_professores; j_prof++) {
                    if (!professor_pode_lecionar(&professores[j_prof], &disciplinas[i].disciplina)) {
                        continue;
                    }
                    
                    int tem_conflito_horario_com_alocadas_prof = 0;
                    for (int k = 0; k < qtd_horarios_da_disciplina_atual; k++) {
                        if (professor_tem_aula_no_horario(&professores[j_prof], horarios, qtd_horarios, horarios_da_disciplina_atual[k])) {
                            tem_conflito_horario_com_alocadas_prof = 1;
                            break;
                        }
                    }
                    if (tem_conflito_horario_com_alocadas_prof) {
                        continue;
                    }
                    
                    if (professores[j_prof].carga_alocada + carga_horaria_disciplina > professores[j_prof].carga_horaria) {
                        continue;
                    }
                    
                    if (professores[j_prof].carga_alocada < menor_carga_alocada_do_melhor) {
                        melhor_professor_idx = j_prof;
                        menor_carga_alocada_do_melhor = professores[j_prof].carga_alocada;
                    }
                }
                
                if (melhor_professor_idx != -1) {
                    if (professores[melhor_professor_idx].qtd_disciplinas_alocadas < MAX_DISCIPLINAS_PROF) {
                        strcpy(professores[melhor_professor_idx].disciplinas_alocadas[professores[melhor_professor_idx].qtd_disciplinas_alocadas],
                               codigo_disciplina);
                        professores[melhor_professor_idx].qtd_disciplinas_alocadas++;
                        professores[melhor_professor_idx].carga_alocada += carga_horaria_disciplina;
                    }
                }
            }
        }
    }
}