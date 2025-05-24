#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "leitura.h"
#include "verificacao.h"
#include "conflitos.h"
#include "horario.h"
#include "professor.h"

#define MAX_ENFASES 20

int main() {
    Disciplina disciplinas[MAX_DISCS];
    Horario horarios[MAX_HORARIOS];
    Professor professores[MAX_PROFESSORES];
    Aluno aluno;

    int qtd_disciplinas = ler_disciplinas("Disciplinas.txt", disciplinas);
    int qtd_horarios = ler_horarios_detalhados("horarios.txt", horarios);
    if (qtd_horarios == 0) {
        printf("Formato detalhado de horarios.txt nao encontrado ou arquivo vazio, tentando formato antigo...\n");
        qtd_horarios = ler_horarios("horarios.txt", horarios); // Fallback
    }
    int qtd_professores = ler_professores("professores.txt", professores);
    ler_historico("historico.txt", &aluno);

    printf("Leitura concluida com sucesso!\n");
    printf("Aluno: %s | Matricula: %s | Periodo: %d\n",
           aluno.nome, aluno.matricula, aluno.periodo_atual);

    // ... (existing code for Disciplinas Pendentes and Grade Sugerida for OBRIGATORIAS) ...
    // This part prints the mandatory pending courses and their suggested schedule
    printf("\n--- Disciplinas Obrigatorias Pendentes ---\n");
    Disciplina pendentes[MAX_DISCS];
    int qtd_pendentes = disciplinas_pendentes(disciplinas, qtd_disciplinas, &aluno, pendentes);

    for (int i = 0; i < qtd_pendentes; i++) {
        printf("%s - %s (Periodo ideal: %d)\n",
               pendentes[i].codigo, pendentes[i].nome, pendentes[i].periodo_ideal);
    }

    DisciplinaAlocada *alocadas = malloc(qtd_pendentes * sizeof(DisciplinaAlocada));
    if (!alocadas && qtd_pendentes > 0) { 
        perror("Falha ao alocar memoria para disciplinas alocadas (obrigatorias)");
        return 1;
    }
    for (int i = 0; i < qtd_pendentes; i++) {
        alocadas[i].disciplina = pendentes[i];
        alocadas[i].periodo_alocado = (pendentes[i].periodo_ideal < aluno.periodo_atual)
                                        ? aluno.periodo_atual
                                        : pendentes[i].periodo_ideal;
    }

    if (qtd_pendentes > 0) {
        resolver_conflitos(alocadas, qtd_pendentes, horarios, qtd_horarios, aluno.periodo_atual);
    }
    
    printf("\n--- Grade Sugerida para Disciplinas Obrigatorias Pendentes ---\n");
    int max_periodo_sugerido_obrigatorias = aluno.periodo_atual; // Start with current period
        for(int i=0; i < qtd_pendentes; ++i) {
            if(alocadas[i].periodo_alocado > max_periodo_sugerido_obrigatorias) {
                max_periodo_sugerido_obrigatorias = alocadas[i].periodo_alocado;
            }
        }
    // Ensure we display at least a few upcoming periods even if few mandatory courses
    int display_end_period_obrigatorias = (qtd_pendentes == 0) ? aluno.periodo_atual + 2 : max_periodo_sugerido_obrigatorias + 1;


    for (int periodo = aluno.periodo_atual; periodo <= display_end_period_obrigatorias; periodo++) {
        printf("\nPeriodo Sugerido %d:\n", periodo);
        int disciplinas_no_periodo = 0;
        for (int i = 0; i < qtd_pendentes; i++) {
            if (alocadas[i].periodo_alocado == periodo) {
                imprimir_horarios_disciplina_detalhado(
                    alocadas[i].disciplina.codigo,
                    alocadas[i].disciplina.nome,
                    horarios,
                    qtd_horarios
                );
                disciplinas_no_periodo++;
            }
        } 
        if (disciplinas_no_periodo == 0) {
            printf("  Nenhuma disciplina obrigatoria alocada para este periodo.\n");
        }
        printf("\n------------------------------------------------------\n");
    }


    // --- NOVA PARTE: SELEÇÃO E IMPRESSÃO DE ÊNFASE ---
    printf("\n------------------------------------------------------");
    printf("\n--- Selecao de Enfase para Disciplinas Eletivas ---\n");

    char enfases_disponiveis[MAX_ENFASES][50];
    int qtd_enfases_disponiveis = 0;

    for (int i = 0; i < qtd_disciplinas; i++) {
        if (strcmp(disciplinas[i].tipo, "ELETIVA") == 0 && disciplinas[i].enfase[0] != '\0') {
            bool ja_listada = false;
            for (int j = 0; j < qtd_enfases_disponiveis; j++) {
                if (strcmp(enfases_disponiveis[j], disciplinas[i].enfase) == 0) {
                    ja_listada = true;
                    break;
                }
            }
            if (!ja_listada && qtd_enfases_disponiveis < MAX_ENFASES) {
                strcpy(enfases_disponiveis[qtd_enfases_disponiveis], disciplinas[i].enfase);
                qtd_enfases_disponiveis++;
            }
        }
    }

    if (qtd_enfases_disponiveis == 0) {
        printf("Nenhuma enfase com disciplinas eletivas encontrada no curriculo.\n");
    } else {
        printf("\nEnfases disponiveis:\n");
        for (int i = 0; i < qtd_enfases_disponiveis; i++) {
            printf("%d. %s\n", i + 1, enfases_disponiveis[i]);
        }

        int escolha_num;
        char *enfase_escolhida_ptr = NULL; // Use pointer for selected emphasis string
        do {
            printf("Digite o numero da enfase desejada (1 a %d): ", qtd_enfases_disponiveis);
            if (scanf("%d", &escolha_num) != 1) {
                 // Clear input buffer in case of non-integer input
                while (getchar() != '\n');
                escolha_num = 0; // Invalid input
            }
            if (escolha_num >= 1 && escolha_num <= qtd_enfases_disponiveis) {
                enfase_escolhida_ptr = enfases_disponiveis[escolha_num - 1];
            } else {
                printf("Opcao invalida. Tente novamente.\n");
            }
        } while (enfase_escolhida_ptr == NULL);

        printf("\n--- Disciplinas Eletivas da Enfase: %s ---\n", enfase_escolhida_ptr);
        printf("(A ordem e periodo de alocacao destas eletivas devem ser planejados com seu orientador)\n\n");

        int eletivas_da_enfase_impressas = 0;
        for (int i = 0; i < qtd_disciplinas; i++) {
            if (strcmp(disciplinas[i].tipo, "ELETIVA") == 0 &&
                strcmp(disciplinas[i].enfase, enfase_escolhida_ptr) == 0) {
                
                // Verificar se o aluno já foi aprovado nesta eletiva
                if (!aluno_aprovado_em(&aluno, disciplinas[i].codigo)) {
                    // Imprimir no formato similar à grade sugerida (detalhes da disciplina e seus horários)
                    // Não estamos alocando a um período específico aqui, apenas listando com horários.
                    printf("Disciplina Eletiva Sugerida (Pendente):\n");
                    imprimir_horarios_disciplina_detalhado(
                        disciplinas[i].codigo,
                        disciplinas[i].nome,
                        horarios,
                        qtd_horarios
                    );
                    printf("    Carga Horaria: %dh\n", disciplinas[i].carga_horaria);
                    printf("    Pre-requisitos: %s\n", (strcmp(disciplinas[i].prerequisitos,"-") == 0 || strlen(disciplinas[i].prerequisitos) == 0) ? "Nenhum" : disciplinas[i].prerequisitos);
                    printf("  --------------------------------------\n");
                    eletivas_da_enfase_impressas++;
                }
            }
        }
        if(eletivas_da_enfase_impressas == 0){
            printf("Nao ha disciplinas eletivas pendentes para a enfase '%s' ou todas ja foram cursadas com aprovacao.\n", enfase_escolhida_ptr);
        }
    }
    printf("\n------------------------------------------------------\n");

    // ... (existing code for Alocacao de Professores) ...
    if (qtd_pendentes > 0) { // Only try to allocate professors if there were mandatory courses processed
        alocar_professores(professores, qtd_professores, alocadas, qtd_pendentes, horarios, qtd_horarios);
    } else {
        printf("\nNenhuma disciplina obrigatoria pendente para alocar professores.\n");
    }


    if (alocadas) { // Free only if malloc was successful
      free(alocadas);
    }
    return 0;
}