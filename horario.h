#ifndef HORARIO_H
#define HORARIO_H

#include "leitura.h"

// Função para interpretar os dias da semana
void interpretar_dias(const char *dias_str, char *resultado);

// Função para imprimir horários detalhados
void imprimir_horarios_detalhados(Horario *horarios, int qtd_horarios);

// Função para imprimir os horários de uma disciplina específica
void imprimir_horarios_disciplina_detalhado(const char *codigo, const char *nome, Horario *horarios, int qtd_horarios);

#endif
