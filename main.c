#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/*
 * Controle da impressora Elgin (i8 / i9) em C, usando a DLL E1_Impressora01.dll.
 *
 * A DLL precisa estar na RAIZ do projeto (na mesma pasta de onde o programa e executado),
 * junto com os arquivos XMLSAT.xml e CANC_SAT.xml.
 *
 * IMPORTANTE: a DLL precisa ter a mesma arquitetura do compilador:
 *   - compilador de 32 bits (ex.: MinGW do Code::Blocks)  -> use a DLL da pasta x86
 *   - compilador de 64 bits (ex.: MSYS2 mingw64)          -> use a DLL da pasta x64
 */

/* ===================== Ligacao com a DLL (equivalente a interface ImpressoraDLL do Java) ===================== */

/* Cada typedef descreve a "forma" de um comando da DLL: o que devolve e quais parametros recebe */
typedef int (*FnAbreConexaoImpressora)(int tipo, const char *modelo, const char *conexao, int param);
typedef int (*FnFechaConexaoImpressora)(void);
typedef int (*FnImpressaoTexto)(const char *dados, int posicao, int estilo, int tamanho);
typedef int (*FnCorte)(int avanco);
typedef int (*FnImpressaoQRCode)(const char *dados, int tamanho, int nivelCorrecao);
typedef int (*FnImpressaoCodigoBarras)(int tipo, const char *dados, int altura, int largura, int HRI);
typedef int (*FnAvancaPapel)(int linhas);
typedef int (*FnStatusImpressora)(int param);
typedef int (*FnAbreGavetaElgin)(int pino, int ti, int tf);
typedef int (*FnAbreGaveta)(int pino, int ti, int tf);
typedef int (*FnSinalSonoro)(int qtd, int tempoInicio, int tempoFim);
typedef int (*FnModoPagina)(void);
typedef int (*FnLimpaBufferModoPagina)(void);
typedef int (*FnImprimeModoPagina)(void);
typedef int (*FnModoPadrao)(void);
typedef int (*FnPosicaoImpressaoHorizontal)(int posicao);
typedef int (*FnPosicaoImpressaoVertical)(int posicao);
typedef int (*FnImprimeXMLSAT)(const char *dados, int param);
typedef int (*FnImprimeXMLCancelamentoSAT)(const char *dados, const char *assQRCode, int param);

/* Um ponteiro para cada comando. Depois de carregarDLL(), e so chamar: AbreConexaoImpressora(...) */
static FnAbreConexaoImpressora AbreConexaoImpressora;
static FnFechaConexaoImpressora FechaConexaoImpressora;
static FnImpressaoTexto ImpressaoTexto;
static FnCorte Corte;
static FnImpressaoQRCode ImpressaoQRCode;
static FnImpressaoCodigoBarras ImpressaoCodigoBarras;
static FnAvancaPapel AvancaPapel;
static FnStatusImpressora StatusImpressora;
static FnAbreGavetaElgin AbreGavetaElgin;
static FnAbreGaveta AbreGaveta;
static FnSinalSonoro SinalSonoro;
static FnModoPagina ModoPagina;
static FnLimpaBufferModoPagina LimpaBufferModoPagina;
static FnImprimeModoPagina ImprimeModoPagina;
static FnModoPadrao ModoPadrao;
static FnPosicaoImpressaoHorizontal PosicaoImpressaoHorizontal;
static FnPosicaoImpressaoVertical PosicaoImpressaoVertical;
static FnImprimeXMLSAT ImprimeXMLSAT;
static FnImprimeXMLCancelamentoSAT ImprimeXMLCancelamentoSAT;

static HMODULE dll = NULL;

/* Procura um comando dentro da DLL. Devolve 1 se achou, 0 se nao achou. */
static int carregaFuncao(void *destino, const char *nome) {
    FARPROC f = GetProcAddress(dll, nome);
    if (f == NULL) {
        printf("Comando '%s' nao encontrado na DLL.\n", nome);
        return 0;
    }
    *(FARPROC *)destino = f;
    return 1;
}

/* Carrega a DLL da raiz do projeto e todos os comandos. Devolve 1 se deu certo. */
static int carregarDLL(void) {
    dll = LoadLibraryA("E1_Impressora01.dll");
    if (dll == NULL) {
        printf("Nao foi possivel carregar E1_Impressora01.dll (erro %lu).\n", GetLastError());
        printf("Confira: a DLL esta na raiz do projeto? Ela tem a mesma arquitetura (32/64 bits) do compilador?\n");
        return 0;
    }

    int ok = 1;
    ok &= carregaFuncao(&AbreConexaoImpressora, "AbreConexaoImpressora");
    ok &= carregaFuncao(&FechaConexaoImpressora, "FechaConexaoImpressora");
    ok &= carregaFuncao(&ImpressaoTexto, "ImpressaoTexto");
    ok &= carregaFuncao(&Corte, "Corte");
    ok &= carregaFuncao(&ImpressaoQRCode, "ImpressaoQRCode");
    ok &= carregaFuncao(&ImpressaoCodigoBarras, "ImpressaoCodigoBarras");
    ok &= carregaFuncao(&AvancaPapel, "AvancaPapel");
    ok &= carregaFuncao(&StatusImpressora, "StatusImpressora");
    ok &= carregaFuncao(&AbreGavetaElgin, "AbreGavetaElgin");
    ok &= carregaFuncao(&AbreGaveta, "AbreGaveta");
    ok &= carregaFuncao(&SinalSonoro, "SinalSonoro");
    ok &= carregaFuncao(&ModoPagina, "ModoPagina");
    ok &= carregaFuncao(&LimpaBufferModoPagina, "LimpaBufferModoPagina");
    ok &= carregaFuncao(&ImprimeModoPagina, "ImprimeModoPagina");
    ok &= carregaFuncao(&ModoPadrao, "ModoPadrao");
    ok &= carregaFuncao(&PosicaoImpressaoHorizontal, "PosicaoImpressaoHorizontal");
    ok &= carregaFuncao(&PosicaoImpressaoVertical, "PosicaoImpressaoVertical");
    ok &= carregaFuncao(&ImprimeXMLSAT, "ImprimeXMLSAT");
    ok &= carregaFuncao(&ImprimeXMLCancelamentoSAT, "ImprimeXMLCancelamentoSAT");
    return ok;
}

/* ===================== Variaveis do programa ===================== */

static int conexaoAberta = 0;       /* 0 = fechada, 1 = aberta */
static int tipo = 1;                /* 1 = USB, 2 = Serial, 3 = Rede... */
static char modelo[50] = "i9";      /* "i8" ou "i9" */
static char conexao[50] = "USB";
static int parametro = 0;

/* Chave do QR Code usada no cancelamento SAT */
static const char *ASS_QRCODE =
    "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZjbh3btsZhF+sjHqEMR159i4agru9x6KsepK/"
    "q0E2e5xlU5cv3m1woYfgHyOkWDNcSdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQEVd7u+"
    "TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+"
    "ZfO5b/+vkyAHS6CYVFCDtYR9Hi5qgdk31v23w==";

/* ===================== Funcoes auxiliares ===================== */

/* Mostra a mensagem e le uma linha de texto digitada (sem o ENTER no final) */
static void capturarEntrada(const char *mensagem, char *destino, int tamanho) {
    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\r\n")] = '\0';
}

/* Mostra a mensagem e le um numero inteiro. Se digitar letras, pergunta de novo. */
static int lerInteiro(const char *mensagem) {
    char linha[50];
    char *fim;
    while (1) {
        capturarEntrada(mensagem, linha, sizeof(linha));
        long valor = strtol(linha, &fim, 10);
        if (linha[0] != '\0' && *fim == '\0') {
            return (int)valor;
        }
        printf("Digite apenas numeros!\n");
    }
}

/* Le um arquivo inteiro para a memoria. Devolve NULL se nao achar. Quem chama deve dar free(). */
static char *lerArquivo(const char *caminho) {
    FILE *arq = fopen(caminho, "rb");
    if (arq == NULL) {
        return NULL;
    }
    fseek(arq, 0, SEEK_END);
    long tamanho = ftell(arq);
    rewind(arq);

    char *conteudo = (char *)malloc(tamanho + 1);
    if (conteudo == NULL) {
        fclose(arq);
        return NULL;
    }
    size_t lidos = fread(conteudo, 1, tamanho, arq);
    conteudo[lidos] = '\0';
    fclose(arq);
    return conteudo;
}

/* ===================== Funcoes da impressora ===================== */

void configurarConexao(void) {
    if (!conexaoAberta) {
        tipo = lerInteiro("Digite o tipo de conexao (1 para USB, 2 para Serial, etc.): ");
        capturarEntrada("Digite o modelo da impressora: ", modelo, sizeof(modelo));
        capturarEntrada("Digite a porta de conexao (ex: USB): ", conexao, sizeof(conexao));
        parametro = lerInteiro("Digite o parametro adicional (ex: 0 para padrao): ");
        printf("Parametros de conexao configurados com sucesso.\n");
    } else {
        printf("Conexao ja configurada. Pronta para uso.\n");
    }
}

void abrirConexao(void) {
    if (!conexaoAberta) {
        int retorno = AbreConexaoImpressora(tipo, modelo, conexao, parametro);
        if (retorno == 0) {
            conexaoAberta = 1;
            printf("Conexao aberta com sucesso.\n");
        } else {
            printf("Erro ao abrir conexao. Codigo de erro: %d\n", retorno);
        }
    } else {
        printf("Conexao ja esta aberta.\n");
    }
}

/* Fecha uma conexao aberta e atualiza seu estado somente se o comando funcionar. */
void fecharConexao(void) {
    if (!conexaoAberta) {
        printf("Conexao ja esta fechada.\n");
        return;
    }
    int retorno = FechaConexaoImpressora();
    if (retorno == 0) {
        conexaoAberta = 0;
        printf("Conexao fechada com sucesso.\n");
    } else {
        printf("Erro ao fechar conexao. Codigo: %d\n", retorno);
    }
}

/* Confere a conexao, le o texto e as copias, imprime em um laco e verifica o corte. */
void imprimirTexto(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    char texto[200];
    capturarEntrada("Texto a imprimir: ", texto, 199);
    if (texto[0] == '\0') {
        printf("Texto vazio. Impressao cancelada.\n");
        return;
    }
    int copias = lerInteiro("Quantas copias? ");
    if (copias < 1) {
        printf("A quantidade deve ser maior que zero.\n");
        return;
    }
    /* A quebra de linha envia cada copia do buffer para o papel. */
    int posicao = 0;
    while (texto[posicao] != '\0') {
        posicao++;
    }
    texto[posicao] = '\n';
    texto[posicao + 1] = '\0';
    for (int i = 0; i < copias; i++) {
        int retorno = ImpressaoTexto(texto, 1, 4, 0);
        if (retorno < 0) {
            printf("Erro ao imprimir copia %d. Codigo: %d\n", i + 1, retorno);
            return;
        }
        printf("Copia %d enviada com sucesso.\n", i + 1);
    }
    int retornoCorte = Corte(5);
    if (retornoCorte == 0) {
        printf("Papel cortado com sucesso.\n");
    } else {
        printf("Erro ao cortar papel. Codigo: %d\n", retornoCorte);
    }
}

/* Confere a conexao, le o conteudo, imprime o QR Code e verifica o corte. */
void imprimirQRCode(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    char dados[300];
    capturarEntrada("Conteudo do QR Code: ", dados, 300);
    if (dados[0] == '\0') {
        printf("Conteudo vazio. Impressao cancelada.\n");
        return;
    }
    int retorno = ImpressaoQRCode(dados, 6, 4);
    if (retorno != 0) {
        printf("Erro ao imprimir QR Code. Codigo: %d\n", retorno);
        return;
    }
    printf("QR Code enviado com sucesso.\n");
    int retornoCorte = Corte(5);
    if (retornoCorte == 0) {
        printf("Papel cortado com sucesso.\n");
    } else {
        printf("Erro ao cortar papel. Codigo: %d\n", retornoCorte);
    }
}

/* Confere a conexao, imprime o codigo de barras de exemplo e verifica o corte. */
void imprimirCodigoBarras(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    int retorno = ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3);
    if (retorno != 0) {
        printf("Erro ao imprimir codigo de barras. Codigo: %d\n", retorno);
        return;
    }
    printf("Codigo de barras enviado com sucesso.\n");
    int retornoCorte = Corte(5);
    if (retornoCorte == 0) {
        printf("Papel cortado com sucesso.\n");
    } else {
        printf("Erro ao cortar papel. Codigo: %d\n", retornoCorte);
    }
}

/* Le o XML SAT em um vetor, envia para a impressora e verifica o corte. */
void imprimirXMLSAT(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    char dadosXML[10000];
    FILE *arquivo = fopen("XMLSAT.xml", "rb");
    if (arquivo == NULL) {
        printf("Nao foi possivel ler o XMLSAT.xml.\n");
        return;
    }
    /* Se o vetor ficar cheio, falta espaco para marcar o fim do texto. */
    int quantidade = fread(dadosXML, 1, 10000, arquivo);
    fclose(arquivo);
    if (quantidade == 0) {
        printf("O arquivo XMLSAT.xml esta vazio.\n");
        return;
    }
    if (quantidade == 10000) {
        printf("O arquivo XMLSAT.xml ultrapassa 9999 bytes.\n");
        return;
    }
    dadosXML[quantidade] = '\0';
    int retorno = ImprimeXMLSAT(dadosXML, 0);
    if (retorno != 0) {
        printf("Erro ao imprimir XML SAT. Codigo: %d\n", retorno);
        return;
    }
    printf("XML SAT enviado com sucesso.\n");
    int retornoCorte = Corte(5);
    if (retornoCorte == 0) {
        printf("Papel cortado com sucesso.\n");
    } else {
        printf("Erro ao cortar papel. Codigo: %d\n", retornoCorte);
    }
}

/* Le o XML de cancelamento em um vetor e imprime com a assinatura fornecida. */
void imprimirXMLCancelamentoSAT(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    char dadosXML[10000];
    FILE *arquivo = fopen("CANC_SAT.xml", "rb");
    if (arquivo == NULL) {
        printf("Nao foi possivel ler o CANC_SAT.xml.\n");
        return;
    }
    /* Se o vetor ficar cheio, falta espaco para marcar o fim do texto. */
    int quantidade = fread(dadosXML, 1, 10000, arquivo);
    fclose(arquivo);
    if (quantidade == 0) {
        printf("O arquivo CANC_SAT.xml esta vazio.\n");
        return;
    }
    if (quantidade == 10000) {
        printf("O arquivo CANC_SAT.xml ultrapassa 9999 bytes.\n");
        return;
    }
    dadosXML[quantidade] = '\0';
    int retorno = ImprimeXMLCancelamentoSAT(dadosXML, ASS_QRCODE, 0);
    if (retorno == 0) {
        printf("XML de cancelamento SAT enviado com sucesso.\n");
    } else {
        printf("Erro ao imprimir cancelamento SAT. Codigo: %d\n", retorno);
    }
}

/* Confere a conexao e o retorno do comando de abertura da gaveta Elgin. */
void abrirGavetaElgin(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    int retorno = AbreGavetaElgin(1, 50, 50);
    if (retorno == 0) {
        printf("Gaveta Elgin aberta com sucesso.\n");
    } else {
        printf("Erro ao abrir gaveta Elgin. Codigo: %d\n", retorno);
    }
}

/* Confere a conexao e o retorno do comando alternativo de abertura da gaveta. */
void abrirGaveta(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    int retorno = AbreGaveta(1, 5, 10);
    if (retorno == 0) {
        printf("Gaveta aberta com sucesso.\n");
    } else {
        printf("Erro ao abrir gaveta. Codigo: %d\n", retorno);
    }
}

/* Confere a conexao e envia quatro sinais sonoros, mostrando sucesso ou erro. */
void emitirSinalSonoro(void) {
    if (!conexaoAberta) {
        printf("Conexao fechada. Abra a conexao antes (opcao 2).\n");
        return;
    }
    int retorno = SinalSonoro(4, 50, 5);
    if (retorno == 0) {
        printf("Sinal sonoro enviado com sucesso.\n");
    } else {
        printf("Erro ao emitir sinal sonoro. Codigo: %d\n", retorno);
    }
}

/* Mostra todas as opcoes do programa, incluindo a opcao zero para sair. */
void exibirMenu(void) {
    printf("\n*************************************************\n");
    printf("**************** MENU IMPRESSORA ****************\n");
    printf("*************************************************\n\n");

    printf("1  - Configurar Conexao\n");    
    printf("2  - Abrir Conexao\n");
    printf("3  - Fechar Conexao\n");
    printf("4  - Imprimir Texto\n");
    printf("5  - Imprimir QR Code\n");
    printf("6  - Imprimir Codigo de Barras\n");
    printf("7  - Imprimir XML SAT\n");
    printf("8  - Imprimir XML de Cancelamento SAT\n");
    printf("9  - Abrir Gaveta Elgin\n");
    printf("10 - Abrir Gaveta\n");
    printf("11 - Emitir Sinal Sonoro\n");
    printf("0  - Sair\n");
    printf("--------------------------------------\n");
}

/* ===================== Programa principal ===================== */

/* Pede usuario e senha, permite tres tentativas e mostra quantas restam. */
int fazerLogin(void) {
    char usuario[50], senha[50];
    for (int tentativa = 1; tentativa <= 3; tentativa++) {
        capturarEntrada("Usuario: ", usuario, 50);
        capturarEntrada("Senha: ", senha, 50);
        if (strcmp(usuario, "admin") == 0 && strcmp(senha, "1234") == 0) {
            printf("Login realizado com sucesso.\n");
            return 1;
        }
        printf("Usuario ou senha incorretos. Tentativas restantes: %d\n", 3 - tentativa);
    }
    return 0;
}

/* Carrega a DLL, verifica o login e repete o menu, chamando a funcao escolhida. */
int main(void) {
    if (!carregarDLL()) {
        printf("Programa encerrado.\n");
        return 1;
    }

    if (!fazerLogin()) {
        printf("Acesso negado.\n");
        FreeLibrary(dll);
        return 1;
    }

    int opcao = -1;
    while (opcao != 0) {
        exibirMenu();
        opcao = lerInteiro("\nDigite a opcao desejada: ");

        switch (opcao) {
            case 1:
                configurarConexao();
                break;
            case 2:
                abrirConexao();
                break;
            case 3:
                fecharConexao();
                break;
            case 4:
                imprimirTexto();
                break;
            case 5:
                imprimirQRCode();
                break;
            case 6:
                imprimirCodigoBarras();
                break;
            case 7:
                imprimirXMLSAT();
                break;
            case 8:
                imprimirXMLCancelamentoSAT();
                break;
            case 9:
                abrirGavetaElgin();
                break;
            case 10:
                abrirGaveta();
                break;
            case 11:
                emitirSinalSonoro();
                break;
            case 0:
                fecharConexao();
                printf("Programa encerrado.\n");
                break;
            default:
                printf("Opcao invalida.\n");
                break;
        }
    }

    FreeLibrary(dll);
    return 0;
}
