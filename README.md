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
- **Impressora Elgin i8 ou i9** — equipamento previsto para a demonstração; ainda não houve teste físico.

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
| 3 | Fechar a conexão — implementação pendente. |
| 4 | Imprimir texto e escolher a quantidade de cópias — implementação pendente. |
| 5 | Imprimir QR Code — implementação pendente. |
| 6 | Imprimir código de barras — implementação pendente. |
| 7 | Imprimir o XML SAT — implementação pendente. |
| 8 | Imprimir o XML de cancelamento SAT — implementação pendente. |
| 9 | Abrir gaveta pelo comando Elgin — implementação pendente. |
| 10 | Abrir gaveta pelo comando alternativo — implementação pendente. |
| 11 | Emitir sinal sonoro — implementação pendente. |
| 0 | Encerra o menu e o programa. O fechamento explícito da conexão depende da próxima implementação. |

O menu completo já está disponível e usa `switch/case` para chamar cada função. As funções marcadas como pendentes ainda não executam essas operações.

A configuração inicial é tipo `1`, modelo `i9`, conexão `USB` e parâmetro `0`. Para usar uma i8, altere o modelo na opção 1 antes de abrir a conexão.

---

## 🧪 Testes Realizados

| Teste | Resultado |
| --- | --- |
| Compilação da primeira etapa em Windows x64 com Tiny C Compiler 0.9.27 | Aprovado. |
| Login com `admin` e `1234`, seguido de acesso ao menu | Aprovado com DLL simulada. |
| Três tentativas com usuário e senha incorretos | Acesso negado, com indicação das tentativas restantes. |
| Seleção das opções e saída pelo menu | Encaminhamento conferido com DLL simulada; as funções pendentes continuam vazias. |
| Impressão de texto, QR Code, código de barras e XMLs | Pendente de implementação nesta etapa e de teste físico. |
| Corte de papel, sinal sonoro e gavetas | Pendente de implementação nesta etapa e de teste físico. |

Os testes simulados **não comprovam funcionamento na impressora real**. O grupo ainda precisa concluir as funções restantes e realizar a demonstração na faculdade.

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
