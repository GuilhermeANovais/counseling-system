# Aconselhamento de Disciplinas para Ciência da Computação

Este projeto oferece um sistema de aconselhamento acadêmico inteligente para estudantes de Ciência da Computação, alocando disciplinas de forma a evitar conflitos de horários, respeitar pré-requisitos e priorizar a progressão ideal da grade curricular.

## Funcionalidades
- **Leitura de dados**: Carrega disciplinas, horários e histórico do aluno de arquivos.
- **Identificação de pendências**: Filtra disciplinas obrigatórias não concluídas.
- **Resolução inteligente de conflitos**:
  - 🕒 **Evita sobreposição de horários** (turnos, dias e aulas).
  - 📚 **Respeita pré-requisitos** (ex: Programação 1 antes de Estrutura de Dados).
  - 🎯 **Prioriza a ordem ideal da grade curricular**.
- **Visualização intuitiva**: Exibe disciplinas alocadas por período, facilitando o planejamento semestral.

## Estrutura do Projeto
├── main.c -> Ponto de entrada <br>
├── leitura.h -> Declarações para leitura de dados <br>
├── leitura.c -> Implementação da leitura de arquivos <br>
├── verificacao.h -> Verificação de pré-requisitos e aprovações <br>
├── verificacao.c -> Lógica de validação acadêmica <br>
├── conflitos.h -> Declarações para gestão de conflitos <br>
├── conflitos.c -> Algoritmos de alocação de horários <br>
├── Disciplinas.txt -> Grade curricular do curso (ex: COMP359, COMP364) <br>
├── horarios.txt -> Horários ofertados (ex: 6T12, 24M34) <br>
└── historico.txt -> Histórico do aluno (disciplinas cursadas) <br>

## Componentes Principais

### 1. **Leitura de Dados (`leitura.h`/`leitura.c`)**
- Processa arquivos com a estrutura do curso:
  - **Disciplinas**: Código, nome, carga horária, período ideal, tipo (obrigatória/eletiva), pré-requisitos.
  - **Horários**: Dias da semana (1-7), turnos (M/T/N), aulas (1-6).
  - **Histórico**: Notas, status (aprovado/reprovado).

### 2. **Verificação Acadêmica (`verificacao.h`/`verificacao.c`)**
- **`disciplinas_pendentes()`**: Lista disciplinas obrigatórias não aprovadas.
- **`aluno_aprovado_em()`**: Verifica se o aluno atingiu nota mínima (≥7.0) ou status "APRM".
- **`obter_prerequisitos()`**: Extrai cadeias de pré-requisitos (ex: "COMP359;COMP360").

### 3. **Gestão de Conflitos (`conflitos.h`/`conflitos.c`)**
- **`resolver_conflitos()`**:
  - Ordena disciplinas por código (prioriza menores, ex: COMP359 antes de COMP360).
  - Aloca no período ideal ou atualiza para períodos posteriores em caso de:
    - ⚠️ **Conflito de horário**: Disciplinas com sobreposição de dias/turnos/aulas.
    - ⛓️ **Pré-requisitos pendentes**: Exige conclusão de disciplinas anteriores.
- **`tem_conflito()`**: Compara horários de duas disciplinas.

## Como Executar

### Pré-requisitos
- Compilador GCC (ex: `sudo apt install build-essential` no Linux).
- Arquivos de entrada no formato correto (veja `Disciplinas.txt` e `horarios.txt`).

### Compilação e Execução
```bash
# Compilar
gcc -o main main.c leitura.c verificacao.c conflitos.c

# Executar
./main
```

## Exemplo de saída
```
--- Disciplinas Alocadas sem Conflitos ---
Período 2:
  COMP359 - Programação 1 (Pré-requisito: Nenhum)
  COMP360 - Lógica para Computação

Período 3:
  COMP364 - Estrutura de Dados (Pré-requisito: COMP359)
```

### Nota: 
Este projeto foi desenvolvido para otimizar o planejamento acadêmico de estudantes de Ciência da Computação, mas pode ser adaptado para outros cursos.
