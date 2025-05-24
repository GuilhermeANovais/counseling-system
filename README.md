# Aconselhamento de Disciplinas para Ciência da Computação

Este projeto oferece um sistema de aconselhamento acadêmico inteligente para estudantes de Ciência da Computação, alocando disciplinas de forma a evitar conflitos de horários, respeitar pré-requisitos e priorizar a progressão ideal da grade curricular.

# Projeto: Assistente de Planejamento Acadêmico

## Visão Geral

Este projeto em C é uma ferramenta de linha de comando desenvolvida para auxiliar estudantes no planejamento de sua trajetória acadêmica. Ele processa diversas fontes de dados, como catálogos de disciplinas, horários de aulas, históricos acadêmicos de estudantes e informações sobre professores. Com base nesses dados, o sistema identifica as disciplinas obrigatórias pendentes para um aluno, sugere uma grade horária sem conflitos para essas disciplinas ao longo dos semestres subsequentes e aloca professores qualificados e disponíveis para as turmas. O sistema também permite que os alunos explorem disciplinas eletivas com base em ênfases acadêmicas e fornece relatórios detalhados do plano de estudos sugerido e das alocações de professores.

---

## Descrição dos Arquivos

### 1. `leitura.h`

* **Propósito:** Este arquivo de cabeçalho define as estruturas de dados centrais usadas em todo o projeto para representar entidades acadêmicas como disciplinas, horários, histórico de disciplinas cursadas, alunos e professores. Também declara os protótipos das funções responsáveis pela leitura e análise (parsing) dos arquivos de texto de entrada.
* **Cabeçalhos Incluídos:** Nenhum diretamente neste arquivo, mas é incluído por muitos arquivos `.c`.
* **Constantes Definidas:**
    * `MAX_DISCS 100`: Número máximo de disciplinas que o sistema pode manipular.
    * `MAX_HORARIOS 100`: Número máximo de entradas de horário.
    * `MAX_HISTORICO 100`: Número máximo de disciplinas no histórico de um aluno.
* **Estruturas Definidas:**
    * **`Disciplina`**: Representa uma única disciplina acadêmica.
        * `char codigo[10]`: Código único da disciplina (ex: "COMP359").
        * `char nome[100]`: Nome completo da disciplina.
        * `int carga_horaria`: Carga horária da disciplina em horas.
        * `int periodo_ideal`: Semestre ideal sugerido para cursar disciplinas obrigatórias; pode ser 0 ou um guia geral para eletivas.
        * `char tipo[20]`: Tipo da disciplina (ex: "OBRIGATORIA", "ELETIVA").
        * `char enfase[50]`: Nome da ênfase/trilha para disciplinas eletivas (ex: "COMPUTACAO_VISUAL"). Vazio para obrigatórias.
        * `char prerequisitos[200]`: String contendo os códigos das disciplinas pré-requisito, separados por ponto e vírgula.
    * **`Horario`**: Representa um horário específico de aula.
        * `char codigo_disciplina[10]`: Código da disciplina associada.
        * `int dias[7]`: Representação numérica dos dias da semana da aula.
        * `int qtd_dias`: Número de dias em que a aula ocorre.
        * `char turno`: Turno ('M' - Manhã, 'T' - Tarde, 'N' - Noite).
        * `int aulas[6]`: Números dos períodos/blocos de aula.
        * `int qtd_aulas`: Quantidade de períodos/blocos de aula.
        * `char local[100]`: Sala de aula ou local.
        * `char hora_inicial[20]`: Horário de início (ex: "13:30").
        * `char hora_final[20]`: Horário de término (ex: "15:20").
        * `char professor[100]`: Nome do(s) professor(es) atribuído(s) neste horário específico no arquivo `horarios.txt`.
    * **`DisciplinaCursada`**: Representa uma disciplina cursada pelo aluno.
        * `char codigo[10]`: Código da disciplina.
        * `float nota`: Nota obtida.
        * `char status[10]`: Situação acadêmica (ex: "APRM", "APROVADO").
    * **`Aluno`**: Representa o estudante.
        * `char nome[100]`: Nome do aluno.
        * `char matricula[20]`: Matrícula do aluno.
        * `int periodo_atual`: Período/semestre atual do aluno.
        * `DisciplinaCursada historico[MAX_HISTORICO]`: Vetor de disciplinas cursadas.
        * `int qtd_disciplinas`: Número de disciplinas no histórico.
* **Protótipos de Funções:**
    * `int ler_disciplinas(const char *filename, Disciplina disciplinas[])`: Lê os dados das disciplinas.
    * `int ler_horarios(const char *filename, Horario horarios[])`: Lê dados de horários (formato antigo, fallback).
    * `void ler_historico(const char *filename, Aluno *aluno)`: Lê o histórico acadêmico do aluno.
    * `int ler_horarios_detalhados(const char *filename, Horario horarios[])`: Lê dados detalhados de horários (primário).

---

### 2. `leitura.c`

* **Propósito:** Implementa as funções declaradas em `leitura.h` para realizar o parsing (análise) dos arquivos de texto de entrada (`Disciplinas.txt`, `horarios.txt`, `historico.txt`) e popular as estruturas de dados correspondentes.
* **Cabeçalhos Incluídos:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `leitura.h`.
* **Funções Principais:**
    * **`void remove_newline(char *str)`**
        * **Propósito:** Função utilitária para remover caracteres de nova linha (`\n`) e retorno de carro (`\r`) do final de uma string.
        * **Parâmetros:** `char *str` - A string a ser modificada.
    * **`int ler_disciplinas(const char *filename, Disciplina disciplinas[])`**
        * **Propósito:** Lê as informações das disciplinas do arquivo `Disciplinas.txt`. Utiliza `strtok` para uma análise robusta dos valores separados por ponto e vírgula, lidando com os diferentes formatos de linha para disciplinas obrigatórias e eletivas. Para disciplinas eletivas, extrai corretamente o campo `enfase`. Inicializa os campos da estrutura `Disciplina` antes de preenchê-los.
        * **Parâmetros:** `const char *filename` (nome do arquivo de disciplinas), `Disciplina disciplinas[]` (vetor para armazenar as disciplinas lidas).
        * **Valor de Retorno:** Número de disciplinas lidas com sucesso.
    * **`int ler_horarios(const char *filename, Horario horarios[])`**
        * **Propósito:** Realiza o parsing do arquivo `horarios.txt` assumindo um formato mais antigo e compacto (código da disciplina e uma string combinada de dia/turno/períodos de aula). Serve como um fallback caso `ler_horarios_detalhados` não encontre o arquivo no formato esperado ou falhe. Campos detalhados como `local`, `hora_inicial`, `hora_final` e `professor` são inicializados como strings vazias.
        * **Parâmetros:** `const char *filename` (nome do arquivo de horários), `Horario horarios[]` (vetor para armazenar as entradas de horário).
        * **Valor de Retorno:** Número de entradas de horário lidas.
    * **`int ler_horarios_detalhados(const char *filename, Horario horarios[])`**
        * **Propósito:** Função principal para o parsing do arquivo `horarios.txt`. Espera um formato detalhado separado por ponto e vírgula, incluindo código da disciplina, string compacta de horário (ex: `6T12`), local, hora de início, hora de término e nome do professor. Preenche todos os campos da estrutura `Horario`. Lida com campos vazios ou placeholders (como "-").
        * **Parâmetros:** `const char *filename` (nome do arquivo de horários), `Horario horarios[]` (vetor para armazenar as entradas de horário).
        * **Valor de Retorno:** Número de entradas de horário lidas, ou 0 em caso de erro ao abrir o arquivo.
    * **`void ler_historico(const char *filename, Aluno *aluno)`**
        * **Propósito:** Lê o histórico acadêmico de um aluno a partir do arquivo `historico.txt`. Analisa o nome do aluno, matrícula, período atual e uma lista de disciplinas cursadas com seus códigos, notas e situações.
        * **Parâmetros:** `const char *filename` (nome do arquivo de histórico), `Aluno *aluno` (ponteiro para a estrutura `Aluno` a ser preenchida).

---

### 3. `verificacao.h`

* **Propósito:** Declara funções relacionadas à verificação acadêmica, como checar se um aluno foi aprovado em uma disciplina, identificar disciplinas pendentes e analisar pré-requisitos.
* **Cabeçalhos Incluídos:** `leitura.h`, `conflitos.h`.
* **Estruturas Definidas:** Nenhuma diretamente neste arquivo (utiliza estruturas dos cabeçalhos incluídos).
* **Protótipos de Funções:**
    * `int disciplinas_pendentes(Disciplina todas[], int qtd_disc, Aluno *aluno, Disciplina pendentes[])`: Identifica disciplinas obrigatórias que o aluno ainda precisa cursar.
    * `int aluno_aprovado_em(Aluno *aluno, const char *codigo_disciplina)`: Verifica se um aluno foi aprovado em uma disciplina específica.
    * `int comparar_codigo_crescente(const void *a, const void *b)`: Função de comparação para ordenar disciplinas por código.
    * `int obter_prerequisitos(const char *str_prereq, char prereqs[][10])`: Extrai códigos de pré-requisitos de uma string.

---

### 4. `verificacao.c`

* **Propósito:** Implementa as funções de verificação acadêmica declaradas em `verificacao.h`.
* **Cabeçalhos Incluídos:** `string.h`, `stdlib.h`, `verificacao.h`.
* **Funções Principais:**
    * **`int aluno_aprovado_em(Aluno *aluno, const char *codigo_disciplina)`**
        * **Propósito:** Determina se o aluno concluiu com sucesso uma dada disciplina. A aprovação é baseada em uma nota >= 7.0 ou no status explícito "APRM" (aproveitamento de matéria) em seu histórico acadêmico.
        * **Parâmetros:** `Aluno *aluno`, `const char *codigo_disciplina`.
        * **Valor de Retorno:** `1` se aprovado, `0` caso contrário.
    * **`int disciplinas_pendentes(Disciplina todas[], int qtd_disc, Aluno *aluno, Disciplina pendentes[])`**
        * **Propósito:** Filtra a lista completa de disciplinas para encontrar os cursos obrigatórios (`OBRIGATORIA`) nos quais o aluno ainda não foi aprovado.
        * **Parâmetros:** `Disciplina todas[]`, `int qtd_disc`, `Aluno *aluno`, `Disciplina pendentes[]`.
        * **Valor de Retorno:** A quantidade de disciplinas obrigatórias pendentes.
    * **`int comparar_codigo_crescente(const void *a, const void *b)`**
        * **Propósito:** Função de callback para `qsort`. Compara duas estruturas `DisciplinaAlocada` com base no campo `disciplina.codigo`, facilitando a ordenação em ordem ascendente dos códigos das disciplinas.
        * **Parâmetros:** `const void *a`, `const void *b` (ponteiros para `DisciplinaAlocada`).
        * **Valor de Retorno:** Um inteiro menor que, igual a, ou maior que zero se o código da primeira disciplina for, respectivamente, menor que, igual a, ou maior que o da segunda.
    * **`int obter_prerequisitos(const char *str_prereq, char prereqs[][10])`**
        * **Propósito:** Analisa uma string de códigos de pré-requisitos separados por ponto e vírgula (ex: "COMP359;COMP362") e armazena cada código no vetor `prereqs`. Lida com o caso de não haver pré-requisitos (string de entrada é "-").
        * **Parâmetros:** `const char *str_prereq`, `char prereqs[][10]`.
        * **Valor de Retorno:** O número de pré-requisitos encontrados.

---

### 5. `horario.h`

* **Propósito:** Declara funções para interpretar strings de horário e para imprimir informações de horário em diversos formatos.
* **Cabeçalhos Incluídos:** `leitura.h`.
* **Protótipos de Funções:**
    * `void interpretar_dias(const char *dias_str, char *resultado)`: Converte strings compactas de horário para um formato legível por humanos.
    * `void imprimir_horarios_detalhados(Horario *horarios, int qtd_horarios)`: Imprime todos os horários em detalhe.
    * `void imprimir_horarios_disciplina_detalhado(const char *codigo, const char *nome, Horario *horarios, int qtd_horarios)`: Imprime o(s) horário(s) detalhado(s) para uma disciplina específica.

---

### 6. `horario.c`

* **Propósito:** Implementa funções para interpretação de strings de horário e impressão formatada de detalhes de horários.
* **Cabeçalhos Incluídos:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `horario.h`, `leitura.h`. (Pode precisar de `conflitos.h` se `obter_horarios_por_disciplina` for usado diretamente por `imprimir_horarios_disciplina_detalhado`).
* **Funções Principais:**
    * **`void interpretar_dias(const char *dias_str, char *resultado)`**
        * **Propósito:** Traduz uma string compacta de horário (ex: "24T12" para Segunda/Quarta, Tarde, 1º e 2º períodos) para uma string descritiva e legível (ex: "Segunda, Quarta - Tarde (Aulas: 1, 2)"). Lida com "Nao definido" para entradas inválidas ou vazias.
        * **Parâmetros:** `const char *dias_str`, `char *resultado`.
    * **`void interpretar_dias_horario(const Horario *horario, char *resultado)`**
        * **Propósito:** Semelhante a `interpretar_dias`, mas recebe uma estrutura `Horario` diretamente como entrada para gerar a string descritiva do horário. (Nota: Esta função estava presente no código fonte `horario.c` mas não exposta em `horario.h` nos arquivos fornecidos).
        * **Parâmetros:** `const Horario *horario`, `char *resultado`.
    * **`void imprimir_horarios_detalhados(Horario *horarios, int qtd_horarios)`**
        * **Propósito:** Imprime uma lista abrangente de todos os horários de aula disponíveis a partir do vetor `horarios`. Para cada entrada, exibe o código da disciplina, dias/horários interpretados, local, horários específicos de início/fim e o professor. A saída é formatada para clareza com indentação.
        * **Parâmetros:** `Horario *horarios`, `int qtd_horarios`.
    * **`void imprimir_horarios_disciplina_detalhado(const char *codigo, const char *nome, Horario *todos_os_horarios, int qtd_total_de_horarios)`**
        * **Propósito:** Imprime todas as ofertas de horário disponíveis para uma única disciplina especificada. Primeiro imprime o código e nome da disciplina como um cabeçalho, depois lista cada entrada de horário com seus dias/horários interpretados, local, horas específicas e professor. Se existirem múltiplas entradas de horário para o mesmo curso, elas são listadas sequencialmente. Lida com casos onde nenhum horário está definido. Usa indentação consistente para legibilidade (cabeçalho da disciplina com 2 espaços, detalhes com 4 espaços).
        * **Parâmetros:** `const char *codigo`, `const char *nome`, `Horario *todos_os_horarios`, `int qtd_total_de_horarios`.

---

### 7. `conflitos.h`

* **Propósito:** Declara estruturas e funções para gerenciar e resolver conflitos de horário entre disciplinas.
* **Cabeçalhos Incluídos:** `leitura.h`.
* **Estruturas Definidas:**
    * **`DisciplinaAlocada`**: Representa uma disciplina que foi atribuída a um semestre específico no plano do aluno.
        * `Disciplina disciplina`: A própria estrutura `Disciplina`.
        * `int periodo_alocado`: O semestre/período em que o sistema sugere que o aluno curse esta disciplina, após a resolução de conflitos.
* **Protótipos de Funções:**
    * `void resolver_conflitos(DisciplinaAlocada *alocadas, int qtd_pendentes, Horario *horarios, int qtd_horarios, int periodo_atual_aluno)`: Tenta alocar disciplinas pendentes em períodos futuros, resolvendo conflitos.
    * `int disciplinas_conflitam(DisciplinaAlocada *d1, DisciplinaAlocada *d2, Horario *horarios, int qtd_horarios)`: Verifica se duas disciplinas têm horários conflitantes.
    * `int tem_conflito(Horario *h1, Horario *h2)`: Verifica se dois horários específicos conflitam.
    * `void obter_horarios_por_disciplina(Horario *horarios, int qtd_horarios, const char *codigo, Horario *resultados[], int *qtd)`: Obtém todos os horários de uma disciplina específica.

---

### 8. `conflitos.c`

* **Propósito:** Implementa a lógica para detecção e resolução de conflitos de horário entre disciplinas, conforme declarado em `conflitos.h`.
* **Cabeçalhos Incluídos:** `string.h`, `stdlib.h`, `conflitos.h`, `verificacao.h`.
* **Funções Principais:**
    * **`void obter_horarios_por_disciplina(Horario *horarios, int qtd_horarios, const char *codigo, Horario *resultados[], int *qtd)`**
        * **Propósito:** Coleta todas as entradas da estrutura `Horario` que correspondem a um código de disciplina específico.
        * **Parâmetros:** `Horario *horarios` (vetor de todos os horários), `int qtd_horarios` (total de horários), `const char *codigo` (código da disciplina a buscar), `Horario *resultados[]` (vetor de ponteiros para armazenar os horários encontrados), `int *qtd` (ponteiro para armazenar a quantidade de horários encontrados).
    * **`int tem_conflito(Horario *h1, Horario *h2)`**
        * **Propósito:** Verifica se duas entradas de horário específicas (`h1` e `h2`) são conflitantes. Um conflito ocorre se ambas são no mesmo turno, têm pelo menos um dia da semana em comum e pelo menos um período/aula em comum nesse dia/turno.
        * **Parâmetros:** `Horario *h1`, `Horario *h2`.
        * **Valor de Retorno:** `1` se houver conflito, `0` caso contrário.
    * **`int disciplinas_conflitam(DisciplinaAlocada *d1, DisciplinaAlocada *d2, Horario *horarios, int qtd_horarios)`**
        * **Propósito:** Determina se quaisquer dos horários associados à disciplina `d1` conflitam com quaisquer dos horários associados à disciplina `d2`.
        * **Parâmetros:** `DisciplinaAlocada *d1`, `DisciplinaAlocada *d2`, `Horario *horarios`, `int qtd_horarios`.
        * **Valor de Retorno:** `1` se houver conflito entre as disciplinas, `0` caso contrário.
    * **`void resolver_conflitos(DisciplinaAlocada *alocadas, int qtd_pendentes, Horario *horarios, int qtd_horarios, int periodo_atual_aluno)`**
        * **Propósito:** Organiza as disciplinas pendentes do aluno em períodos futuros. Primeiro, ordena as disciplinas pendentes pelo código (usando `qsort` com `comparar_codigo_crescente`). Em seguida, tenta alocar cada disciplina ao seu `periodo_ideal` (ou ao `periodo_atual_aluno`, o que for maior). Se um pré-requisito de uma disciplina estiver alocado em um período posterior ou igual, o período da disciplina atual é ajustado para `periodo_prereq + 1`. Se ocorrer um conflito de horário com outra disciplina já alocada no mesmo período, o período da disciplina atual é incrementado até que um slot livre seja encontrado. Há uma lógica especial para tentar manter as disciplinas COMP372 (Programação 2) e COMP373 (Programação 3) no mesmo período, se possível e sem conflitos.
        * **Parâmetros:** `DisciplinaAlocada *alocadas` (vetor de disciplinas pendentes a serem agendadas), `int qtd_pendentes` (número de disciplinas pendentes), `Horario *horarios` (vetor de todos os horários), `int qtd_horarios` (total de horários), `int periodo_atual_aluno` (período atual do aluno).

---

### 9. `professor.h`

* **Propósito:** Define a estrutura `Professor` e declara funções para gerenciar dados de professores, incluindo leitura de suas informações, verificação de disponibilidade, qualificação para lecionar disciplinas, alocação de professores a disciplinas e impressão dessas alocações.
* **Cabeçalhos Incluídos:** `leitura.h`, `conflitos.h`.
* **Constantes Definidas:**
    * `MAX_PROFESSORES 100`: Número máximo de professores.
    * `MAX_DISCIPLINAS_PROF 20`: Número máximo de disciplinas que um professor pode ter (habilitadas ou alocadas).
    * `MAX_FORMACAO 200`: Tamanho máximo da string de formação.
    * `MAX_ESPECIALIZACOES 5`: Número máximo de especializações individuais armazenadas.
* **Estruturas Definidas:**
    * **`Professor`**:
        * `char nome[100]`: Nome completo do professor.
        * `int carga_horaria`: Capacidade total de carga horária de ensino do professor.
        * `char formacao[MAX_FORMACAO]`: String descrevendo as qualificações acadêmicas gerais.
        * `char especializacoes[MAX_ESPECIALIZACOES][100]`: Vetor de strings para especializações individuais.
        * `int qtd_especializacoes`: Número de especializações individuais listadas.
        * `char disciplinas_habilitadas[MAX_DISCIPLINAS_PROF][10]`: Códigos das disciplinas que o professor está qualificado para lecionar.
        * `int qtd_disciplinas_habilitadas`: Número de disciplinas habilitadas.
        * `char disciplinas_alocadas[MAX_DISCIPLINAS_PROF][10]`: Códigos das disciplinas atualmente alocadas ao professor pelo sistema.
        * `int qtd_disciplinas_alocadas`: Número de disciplinas alocadas.
        * `int carga_alocada`: Carga horária total atualmente alocada ao professor.
* **Protótipos de Funções:**
    * `int ler_professores(const char *filename, Professor professores[])`: Lê dados dos professores.
    * `int professor_tem_aula_no_horario(Professor *professor, Horario *horarios, int qtd_horarios, Horario *horario_verificar)`: Verifica se um professor já tem aula em um determinado horário.
    * `int professor_pode_lecionar(Professor *professor, const Disciplina *disciplina)`: Verifica se um professor pode lecionar uma disciplina.
    * `void alocar_professores(Professor professores[], int qtd_professores, DisciplinaAlocada disciplinas[], int qtd_disciplinas, Horario *horarios, int qtd_horarios)`: Aloca professores às disciplinas.
    * `void imprimir_alocacao_professores(Professor professores[], int qtd_professores, DisciplinaAlocada disciplinas_alocadas_array[], int qtd_disciplinas_alocadas, Horario *todos_horarios, int qtd_todos_horarios)`: Imprime a alocação de professores.

---

### 10. `professor.c`

* **Propósito:** Implementa as funcionalidades de gerenciamento e alocação de professores, incluindo a leitura de seus dados, verificação de qualificações e disponibilidade, e a atribuição a disciplinas agendadas.
* **Cabeçalhos Incluídos:** `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `professor.h`, `conflitos.h`, `horario.h`.
* **Funções Principais:**
    * **`void trim(char *str)`**
        * **Propósito:** Utilitário para remover espaços em branco, tabulações e quebras de linha do início e do fim de uma string.
        * **Parâmetros:** `char *str` - A string a ser modificada.
    * **`int ler_professores(const char *filename, Professor professores[])`**
        * **Propósito:** Lê e analisa o arquivo `professores.txt`. Para cada professor, extrai nome, carga horária total, qualificações/especializações (armazenadas tanto individualmente quanto concatenadas) e a lista de códigos de disciplinas que ele está habilitado a lecionar. Utiliza `strtok` para o parsing. Implementa uma lógica robusta para o campo de carga horária: se o valor lido for inválido (ex: `?h`) ou zero, atribui uma carga horária padrão alta (ex: 200h) e emite um aviso, permitindo que o professor seja considerado para alocação.
        * **Parâmetros:** `const char *filename`, `Professor professores[]`.
        * **Valor de Retorno:** Número de professores lidos.
    * **`int professor_tem_aula_no_horario(Professor *professor, Horario *horarios, int qtd_horarios, Horario *horario_verificar)`**
        * **Propósito:** Verifica se um determinado professor já possui alguma disciplina alocada (pelo sistema) que conflite com o `horario_verificar`.
        * **Parâmetros:** `Professor *professor`, `Horario *horarios` (todos os horários do sistema), `int qtd_horarios`, `Horario *horario_verificar` (o horário da nova disciplina em potencial).
        * **Valor de Retorno:** `1` se houver conflito, `0` caso contrário.
    * **`int professor_pode_lecionar(Professor *professor, const Disciplina *disciplina)`**
        * **Propósito:** Determina se um professor está qualificado para lecionar uma disciplina específica. A verificação é feita comparando o código da disciplina com a lista de `disciplinas_habilitadas` do professor. A regra anterior que permitia a qualquer professor lecionar disciplinas até o 4º período foi removida.
        * **Parâmetros:** `Professor *professor`, `const Disciplina *disciplina`.
        * **Valor de Retorno:** `1` se o professor pode lecionar, `0` caso contrário.
    * **`void alocar_professores(Professor professores[], int qtd_professores, DisciplinaAlocada disciplinas_alocadas_ao_aluno[], int qtd_disciplinas_aluno, Horario *todos_os_horarios, int qtd_total_horarios)`**
        * **Propósito:** Atribui professores às disciplinas que foram previamente agendadas para o aluno (em `disciplinas_alocadas_ao_aluno[]`). Antes de iniciar, zera as alocações e cargas de todos os professores para o processamento atual. A alocação ocorre em duas fases para cada disciplina:
            1.  **Fase 1 (Prioridade `horarios.txt`):** Verifica se o arquivo `horarios.txt` especifica um professor para algum dos horários da disciplina. Se sim, tenta alocar esse professor específico, contanto que ele: a) possa lecionar a disciplina (conforme `professor_pode_lecionar`), b) tenha carga horária total disponível (conforme `professores[].carga_horaria` vs `professores[].carga_alocada`), e c) não tenha conflitos de horário com outras disciplinas já alocadas a ele pelo sistema. A correspondência de nome entre `horarios.txt` e `professores.txt` é feita por `strcmp` (correspondência exata).
            2.  **Fase 2 (Fallback):** Se a Fase 1 não resultar em alocação (ex: professor não especificado no `horarios.txt`, nome não encontrado, ou o professor especificado não está apto/disponível), o sistema procura entre todos os professores por um que possa lecionar a disciplina, tenha carga horária e horário disponíveis, priorizando aquele com a menor `carga_alocada` no momento.
        * **Parâmetros:** `Professor professores[]`, `int qtd_professores`, `DisciplinaAlocada disciplinas_alocadas_ao_aluno[]`, `int qtd_disciplinas_aluno`, `Horario *todos_os_horarios`, `int qtd_total_horarios`.
    * **`void imprimir_alocacao_professores(Professor professores[], int qtd_professores, DisciplinaAlocada disciplinas_alocadas_para_aluno[], int qtd_total_disciplinas_aluno, Horario *todos_os_horarios, int qtd_total_de_horarios)`**
        * **Propósito:** Exibe a alocação de professores. A função lista **todos** os professores cadastrados. Para cada professor, imprime seu nome e suas especializações/formação. Em seguida, lista **apenas** as disciplinas que foram alocadas a ele pelo sistema E que, no planejamento do aluno (`disciplinas_alocadas_para_aluno`), possuem um `periodo_alocado >= 5`. Para cada uma dessas disciplinas filtradas, são exibidos seus detalhes completos de horário (local, dias/horário interpretado, horas específicas), utilizando `imprimir_horarios_disciplina_detalhado` internamente ou uma lógica similar. Mensagens apropriadas são exibidas se um professor não tiver disciplinas alocadas ou se nenhuma de suas disciplinas alocadas atender ao critério do 5º período. Por fim, lista quaisquer disciplinas que foram consideradas para alocação ao aluno mas não puderam ser atribuídas a nenhum professor.
        * **Parâmetros:** `Professor professores[]`, `int qtd_professores`, `DisciplinaAlocada disciplinas_alocadas_para_aluno[]`, `int qtd_total_disciplinas_aluno`, `Horario *todos_os_horarios`, `int qtd_total_de_horarios`.

---

### 11. `main.c`

* **Propósito:** É o arquivo principal que coordena a execução de todo o projeto. Ele gerencia o fluxo de leitura de dados, processamento, resolução de conflitos, alocação de professores e a apresentação dos resultados ao usuário.
* **Cabeçalhos Incluídos:** `stdio.h`, `stdlib.h`, `string.h`, `stdbool.h`, `leitura.h`, `verificacao.h`, `conflitos.h`, `horario.h`, `professor.h`.
* **Lógica Central/Fluxo de Trabalho:**
    1.  **Leitura de Dados:** Invoca as funções de leitura para carregar informações de `Disciplinas.txt`, `horarios.txt` (com fallback), `professores.txt` e `historico.txt`.
    2.  **Informações do Aluno:** Exibe nome, matrícula e período atual do aluno.
    3.  **Disciplinas Obrigatórias Pendentes:** Chama `disciplinas_pendentes` para identificar as disciplinas obrigatórias que o aluno ainda precisa cursar e as exibe.
    4.  **Resolução de Conflitos (Obrigatórias):** Prepara as disciplinas pendentes para a função `resolver_conflitos`, que atribui um `periodo_alocado` a cada uma, tentando evitar choques de horário e respeitando pré-requisitos.
    5.  **Exibição da Grade Sugerida (Obrigatórias):** Imprime um plano semestral para as disciplinas obrigatórias pendentes, mostrando os detalhes de horário de cada disciplina alocada em seu respectivo período, utilizando `imprimir_horarios_disciplina_detalhado`.
    6.  **Seleção de Ênfase (Eletivas):**
        * Extrai e exibe uma lista de ênfases únicas disponíveis a partir das disciplinas eletivas carregadas.
        * Solicita que o usuário escolha uma ênfase.
        * Filtra as disciplinas eletivas com base na ênfase escolhida e que o aluno ainda não tenha sido aprovado.
        * Exibe as disciplinas eletivas da ênfase selecionada, mostrando seus detalhes de horário disponíveis (usando `imprimir_horarios_disciplina_detalhado`), carga horária e pré-requisitos. Esta seção não realiza um agendamento complexo das eletivas em períodos específicos, mas as apresenta como sugestões.
    7.  **Alocação de Professores:** Chama `alocar_professores` para atribuir professores às disciplinas obrigatórias que foram agendadas para o aluno.
    8.  **Exibição da Alocação de Professores:** Chama `imprimir_alocacao_professores` para mostrar quais professores foram alocados a quais disciplinas (com o filtro de exibir detalhes apenas para disciplinas do 5º período em diante sob cada professor).
    9.  **Liberação de Memória:** Libera a memória alocada dinamicamente (para o vetor `alocadas`).

---

### Arquivos de Dados Necessários

O programa requer os seguintes arquivos de texto no mesmo diretório do executável (ou com caminhos corretamente especificados no código, se alterado):

* `Disciplinas.txt`: Catálogo de disciplinas, suas cargas horárias, períodos ideais, tipos, ênfases (para eletivas) e pré-requisitos.
* `horarios.txt`: Grade de horários das turmas ofertadas, com dias, turnos, aulas, local, horários específicos e professores designados.
* `historico.txt`: Histórico acadêmico do aluno, contendo dados pessoais e disciplinas cursadas com notas e status.
* `professores.txt`: Lista de professores, suas cargas horárias totais, formação/especializações e as disciplinas que estão habilitados a lecionar.

---

### Compilação e Execução

* **Compilação:** Todos os arquivos `.c` (`leitura.c`, `verificacao.c`, `horario.c`, `conflitos.c`, `professor.c`, `main.c`) devem ser compilados e linkados juntos. Um compilador C (como GCC) é necessário.
    ```bash
    gcc main.c leitura.c verificacao.c horario.c conflitos.c professor.c -o programa
    ```
* **Execução:**
    * Execute o programa compilado a partir do terminal: `./programa`
    * Certifique-se de que todos os arquivos de dados `.txt` necessários estão presentes no mesmo diretório do executável.
