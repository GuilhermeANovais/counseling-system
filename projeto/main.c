#include <stdio.h>
#include <stdlib.h>
#include "leitura.h"
#include "verificacao.h"
#include "conflitos.h"
#include "horario.h"

int main() {
    Disciplina disciplinas[MAX_DISCS];
    Horario horarios[MAX_HORARIOS];
    Aluno aluno;

    int qtd_disciplinas = ler_disciplinas("Disciplinas.txt", disciplinas);
    
    // Tentar ler horários no formato detalhado primeiro
    int qtd_horarios = ler_horarios_detalhados("horarios.txt", horarios);
    
    // Se não conseguir, tentar o formato antigo
    if (qtd_horarios == 0) {
        printf("Formato detalhado nao encontrado, tentando formato antigo...\n");
        qtd_horarios = ler_horarios("horarios.txt", horarios);
    }
    
    ler_historico("historico.txt", &aluno);

    printf("Leitura concluida com sucesso!\n");
    printf("Aluno: %s | Matricula: %s | Periodo: %d\n\n",
           aluno.nome, aluno.matricula, aluno.periodo_atual);

    // Verificar disciplinas pendentes
    Disciplina pendentes[MAX_DISCS];
    int qtd_pendentes = disciplinas_pendentes(disciplinas, qtd_disciplinas, &aluno, pendentes);

    printf("--- Disciplinas Pendentes ---\n");
    for (int i = 0; i < qtd_pendentes; i++) {
        printf("%s - %s (Periodo ideal: %d)\n",
               pendentes[i].codigo, pendentes[i].nome, pendentes[i].periodo_ideal);
    }

    DisciplinaAlocada *alocadas = malloc(qtd_pendentes * sizeof(DisciplinaAlocada));
    for (int i = 0; i < qtd_pendentes; i++) {
        alocadas[i].disciplina = pendentes[i];
        alocadas[i].periodo_alocado = (pendentes[i].periodo_ideal < aluno.periodo_atual) 
                                        ? aluno.periodo_atual 
                                        : pendentes[i].periodo_ideal;
    }

    resolver_conflitos(alocadas, qtd_pendentes, horarios, qtd_horarios, aluno.periodo_atual);

    printf("\n--- Disciplinas Alocadas sem Conflitos ---\n");
    for (int periodo = 1; periodo <= 8; periodo++) {
        printf("\nPeriodo %d:\n", periodo);
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
            printf("  Nenhuma disciplina alocada neste periodo\n");
        }
    }

    free(alocadas); // Liberar memória alocada
    return 0;
}
