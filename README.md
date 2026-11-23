# 🖨️ Sistema de Controle de Impressora Elgin
### Controle de Impressora Elgin em C

---

## 📘 Sobre o Projeto

Este projeto foi desenvolvido para controlar uma impressora térmica **Elgin i8/i9**.
O programa possui login com limite de tentativas e um menu de operações da impressora.  

---

## 🧰 Tecnologias Utilizadas

- **Linguagem C** — variáveis, vetores, funções, `strcmp`, `if/else`, `for`, `while` e `switch/case`.
- **Windows x64** — ambiente utilizado na verificação.
- **Tiny C Compiler 0.9.27 (x64)** — compilador utilizado nos testes de preparação.
- **DLL E1_Impressora01** — biblioteca do SDK da Elgin fornecida pelo professor.
- **Git, GitHub e Git LFS** — controle de versões, colaboração e arquivos binários.
- **Impressora Elgin i8 ou i9** — modelos compatíveis com o projeto.

---

## 📁 Estrutura do Projeto

```text
main.c                         → Código-fonte: ligação com a DLL, login e menu
E1_Impressora01.dll             → Biblioteca Elgin de 64 bits
Bibliotecas E1 Impressora.zip   → Bibliotecas originais x64 e x86
XMLSAT.xml                     → Exemplo de cupom SAT
CANC_SAT.xml                   → Exemplo de cancelamento SAT
ASS_QRCODE Cancelamento Sat.txt → Assinatura de exemplo para cancelamento
README.md                      → Apresentação e instruções do projeto
.gitignore                     → Exclusão de executáveis e arquivos temporários
.gitattributes                 → Configuração do Git LFS
```

Todo o código do programa fica em **um único `main.c`**.

---

## ⚙️ Instalação, Compilação e Execução

### 1. Preparar o Windows

Use um PC com **Windows de 64 bits**, preferencialmente Windows 10 ou 11. Para baixar e compilar o projeto, instale Git, Git LFS e GCC x64 conforme os passos abaixo. Para impressão física, é necessário ter uma Elgin i8 ou i9 e o driver correspondente.

### 2. Instalar Git e Git LFS

1. Instale o [Git para Windows](https://git-scm.com/install/windows), mantendo a opção de usar Git pela linha de comando.
2. Feche e abra novamente o PowerShell e confira:

```powershell
git --version
git lfs version
```

3. Se o segundo comando não funcionar, instale o [Git LFS](https://git-lfs.com/) e abra novamente o terminal.
4. Ative o Git LFS:

```powershell
git lfs install
```

### 3. Instalar o compilador C de 64 bits

Escolha **uma** opção de compilação: Dev-C++ com compilador incluído (item **5A**) ou GCC pelo terminal. Para usar Dev-C++, pule a instalação do MSYS2 e baixe o projeto conforme o item 4. Se preferir o terminal e já tiver GCC x64, pode utilizá-lo. Caso contrário, instale **MSYS2 com GCC UCRT64**, seguindo as [orientações oficiais do MinGW-w64](https://www.mingw-w64.org/getting-started/msys2/):

1. Baixe o instalador x64 no [site do MSYS2](https://www.msys2.org/).
2. Instale no caminho padrão `C:\msys64`.
3. Abra **MSYS2 UCRT64** pelo menu Iniciar. Execute os comandos `pacman` nesse terminal, não no PowerShell.
4. Atualize os pacotes:

```bash
pacman -Syu
```

5. Se a atualização pedir para fechar o terminal, aceite, abra **MSYS2 UCRT64** novamente e execute `pacman -Syu` para concluir.
6. Instale o GCC:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc
```

7. Volte ao **PowerShell** e confira:

```powershell
& "C:\msys64\ucrt64\bin\gcc.exe" --version
& "C:\msys64\ucrt64\bin\gcc.exe" -dumpmachine
```

O segundo comando deve mostrar `x86_64-w64-mingw32`. Se instalou MSYS2 em outra pasta, ajuste os caminhos. Usando o caminho completo, não é necessário configurar o `PATH`.

### 4. Baixar o projeto

O repositório é **privado**. Sua conta GitHub precisa ter acesso e aceitar o convite de colaboração, quando houver.

1. Abra no Explorador de Arquivos a pasta em que deseja salvar o projeto.
2. Digite `powershell` na barra de endereço dessa pasta e pressione Enter.
3. Nesse PowerShell, execute:

```powershell
git clone https://github.com/AdrianFCL/linguagem-c-impressora.git
cd linguagem-c-impressora
git lfs pull
```

Se aparecer uma solicitação de autenticação, entre pelo navegador com a conta GitHub que recebeu acesso.

Se já tiver uma cópia atual do projeto, entre nela e confira `git status`. Com a pasta sem alterações locais, atualize:

```powershell
git pull --ff-only
git lfs pull
```

Se o Git informar histórico divergente, preserve suas alterações e faça um novo clone em outra pasta. Isso pode acontecer com cópias anteriores à reorganização do histórico.

Também é possível usar **Code → Download ZIP** e extrair o conteúdo. Confira se a DLL e o ZIP das bibliotecas vieram completos: um pequeno arquivo de texto começando com `version https://git-lfs.github.com/spec/v1` é somente um ponteiro Git LFS. Nesse caso, utilize `git clone` e `git lfs pull`.

### 5. Conferir os arquivos e compilar

O terminal deve estar na pasta que contém:

```text
main.c
E1_Impressora01.dll
XMLSAT.xml
CANC_SAT.xml
```

A DLL da raiz é **x64** e deve ser usada com um compilador de **64 bits**. O ZIP das bibliotecas inclui também uma DLL x86, mas a configuração de 32 bits não foi validada neste projeto.

No PowerShell, com MSYS2 instalado no caminho indicado:

```powershell
& "C:\msys64\ucrt64\bin\gcc.exe" -std=c99 -Wall -Wextra main.c -o impressora.exe
```

Se o seu GCC x64 já estiver no `PATH`, pode usar:

```powershell
gcc -std=c99 -Wall -Wextra main.c -o impressora.exe
```

Após uma compilação sem erros, aparecerá `impressora.exe` nessa pasta. Não é necessário instalar Java, JNA ou IntelliJ. As verificações de preparação utilizaram Tiny C Compiler 0.9.27 x64; os comandos GCC acima são uma alternativa de compilação para o computador do grupo.

### 5A. Alternativa: gerar o executável pelo Dev-C++

Esta opção usa somente os arquivos do projeto já publicados no GitHub. O Dev-C++ é a ferramenta instalada no PC; não é preciso acrescentar outro código-fonte, biblioteca Java ou arquivo de projeto.

1. Instale o [Embarcadero Dev-C++ com TDM-GCC incluído](https://www.embarcadero.com/free-tools/dev-cpp). Escolha o pacote com compilador e suporte a **64 bits**.
2. Baixe o repositório conforme o item 4. Confira se estes arquivos estão juntos e se a DLL foi baixada por completo:

```text
main.c
E1_Impressora01.dll
XMLSAT.xml
CANC_SAT.xml
```

3. Abra o Dev-C++ e use **File → Open** ou **Ctrl+O** para abrir o `main.c` dessa pasta. É possível compilar esse arquivo diretamente, sem criar um projeto novo.
4. Em **Tools → Compiler Options**, selecione uma configuração **TDM-GCC 64-bit**. Mantenha a extensão `.c` para que o arquivo seja compilado como C.
5. Pressione **F9** para compilar ou **F11 — Compile & Run** para compilar e executar. Esses atalhos e a compilação de um único fonte constam na [documentação oficial do Dev-C++](https://github.com/Embarcadero/Dev-Cpp/wiki/Compiling).
6. Confira o executável gerado, normalmente **`main.exe`** na pasta do fonte. Se configurar outro local de saída, mantenha a DLL ao lado do executável.
7. Para garantir a localização dos XMLs, abra o PowerShell na pasta que contém o executável, a DLL e os XMLs e execute:

```powershell
.\main.exe
```

Entre com usuário `admin` e senha `1234`. Os testes de login, menu e impressora estão nos itens 6 e 7.

Nos outros exemplos deste README, o nome `impressora.exe` corresponde ao arquivo gerado pelo comando GCC. Se compilou pelo Dev-C++ e obteve `main.exe`, use esse nome no lugar de `impressora.exe`, inclusive ao copiar para outro PC.

A DLL correta para essa configuração é a **x64 já presente na raiz**. O ZIP de bibliotecas e o arquivo de assinatura de exemplo podem permanecer na pasta; não precisam ser adicionados como arquivos-fonte ao Dev-C++. O executável gerado fica localmente e não deve ser enviado ao GitHub.

### 6. Executar e testar sem impressora

Ainda no PowerShell, na mesma pasta:

```powershell
.\impressora.exe
```

Entre com usuário `admin` e senha `1234`.

Sem impressora, confira:

- Login correto e acesso ao menu.
- Três tentativas incorretas, com contagem e acesso negado.
- Letras no lugar da opção numérica, seguidas de uma opção válida.
- Uma opção inexistente, como `99`, que deve mostrar “Opcao invalida”.
- Opções de `4` a `11` sem abrir conexão: devem avisar que a conexão está fechada, sem pedir dados.
- Opção `0`, que encerra o programa.

Para repetir um teste após o programa fechar, execute `.\impressora.exe` novamente. Encerre normalmente pela opção `0`.

A DLL real não simula uma impressora. Para exercitar impressão, corte, som e gavetas sem equipamento, é necessária uma DLL simulada separada; ela não está incluída no repositório. Os resultados de verificação estão descritos na seção “Testes Realizados”.

### 7. Usar a impressora por USB

1. Instale o driver correspondente ao equipamento conforme as orientações da Elgin.
2. Ligue a impressora, coloque papel e conecte o cabo USB.
3. Execute o programa e faça o login.
4. Na opção **1**, informe:

```text
Tipo de conexao: 1
Modelo: i9
Porta de conexao: USB
Parametro adicional: 0
```

Para uma i8, informe `i8` no lugar de `i9`.

5. Escolha **2** e confira a mensagem de conexão aberta.
6. Use **4** para imprimir um texto, por exemplo com duas cópias.
7. Teste QR Code, código de barras e XMLs pelas opções **5**, **6**, **7** e **8**.
8. A opção **11** emite o sinal sonoro. As opções **9** e **10** exigem uma gaveta compatível conectada.
9. A opção **3** fecha a conexão. A opção **0** fecha a conexão, se aberta, e encerra.

### 8. Executar em outro PC sem instalar o compilador

Após compilar, copie estes quatro arquivos para **a mesma pasta** no outro PC:

```text
impressora.exe
E1_Impressora01.dll
XMLSAT.xml
CANC_SAT.xml
```

No PC de destino, abra essa pasta no Explorador, digite `powershell` na barra de endereço e execute:

```powershell
.\impressora.exe
```

O PC de destino não precisa de GCC, Git ou MSYS2 apenas para executar o programa já compilado. Precisa de Windows compatível com o executável x64 e, para impressão física, dos drivers e dependências exigidos pelo SDK Elgin. Se o Windows informar outra dependência ausente, instale o componente correspondente recomendado pelo fornecedor.

`main.c` só é necessário para ler, modificar ou compilar o código. **Não envie o executável gerado ao GitHub**; ele já está excluído pela `.gitignore`.

### 9. Problemas comuns

| Problema | O que conferir |
| --- | --- |
| `git` ou `git lfs` não é reconhecido | Instale a ferramenta e abra novamente o PowerShell. |
| Acesso negado ou `Repository not found` ao clonar | Confira conta, URL e convite de acesso ao repositório privado. |
| `gcc` não é reconhecido | Use `& "C:\msys64\ucrt64\bin\gcc.exe"` no PowerShell. |
| `main.c` não foi encontrado | Confira a pasta do terminal com `Get-Location` e `dir`. |
| `impressora.exe` não existe | Confira se a compilação terminou sem erros. |
| Erro 126 ao carregar a DLL | Confira a DLL ao lado do executável, o download completo via Git LFS e as dependências do SDK. |
| Erro 193 ao carregar a DLL | Confira se DLL e executável são ambos de 64 bits. |
| Falha ao abrir conexão | Confira alimentação, cabo USB, driver, modelo e parâmetros da opção 1. |
| XML não encontrado | Execute a partir da pasta de `XMLSAT.xml` e `CANC_SAT.xml`. |

---

## 🔐 Login

```text
Usuário: admin
Senha: 1234
```

São permitidas **três tentativas**. A cada erro, o programa informa quantas restam. Depois do terceiro erro, o acesso é negado e o programa encerra. As credenciais são didáticas e fixas, conforme o enunciado.

---

## 📋 Funcionalidades do Menu

| Opção | O que faz |
| --- | --- |
| 1 | Configura tipo, modelo, porta e parâmetro da conexão. Já fornecida pelo professor. |
| 2 | Abre a conexão com a impressora. Já fornecida pelo professor. |
| 3 | Fechar a conexão. |
| 4 | Imprimir texto e escolher a quantidade de cópias. |
| 5 | Imprimir QR Code. |
| 6 | Imprimir código de barras. |
| 7 | Imprimir o XML SAT. |
| 8 | Imprimir o XML de cancelamento SAT. |
| 9 | Abrir gaveta pelo comando Elgin. |
| 10 | Abrir gaveta pelo comando alternativo. |
| 11 | Emitir sinal sonoro. |
| 0 | Fecha a conexão, se estiver aberta, e encerra o programa. |

O menu usa `switch/case` para chamar a função correspondente a cada opção.

A configuração inicial é tipo `1`, modelo `i9`, conexão `USB` e parâmetro `0`. Para usar uma i8, altere o modelo na opção 1 antes de abrir a conexão.

---

## 🧪 Testes Realizados

| Teste | Resultado |
| --- | --- |
| Compilação da versão completa em Windows x64 com Tiny C Compiler 0.9.27 | Aprovado. |
| Login com `admin` e `1234`, seguido de acesso ao menu | Aprovado com DLL simulada. |
| Três tentativas com usuário e senha incorretos | Acesso negado, com indicação das tentativas restantes. |
| Seleção das opções, fechamento da conexão e saída pelo menu | Aprovado com DLL simulada. |
| Texto, cópias, QR Code, código de barras e XMLs | Comandos e parâmetros conferidos com DLL simulada. |
| Corte de papel, sinal sonoro e gavetas | Comandos e retornos conferidos com DLL simulada. |

Os testes utilizaram uma **DLL simulada**, sem acionar uma impressora física. Também foi conferido o carregamento da DLL real de 64 bits, com login e saída. Para encerrar a execução interativa, utilize a opção `0` do menu.

---

## 📚 Documentação Adicional

- [Documentação do SDK Elgin — impressoras térmicas](https://elgindevelopercommunity.github.io/group___m1.html).
- Enunciado **Trabalho_C_Impressora.pdf**, disponibilizado pelo professor.
- Manual da impressora Elgin i8 ou i9 correspondente ao equipamento utilizado na demonstração.

---

## 👥 Autores

- Adrian Ferraz Cardoso Lopes
- Lucas Andrade Gonçalves Pereira
- Pedro Henrique Marques Trindade
