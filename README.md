Simulador de Redes
==================

Simulador de redes criado pelos alunos Silvana Trindade e Maurício André Cinelli para a disciplina de redes.

O objetivo do software é dado uma topologia de rede e uma matriz de tráfego, encontrar os caminhos que proporcionem um melhor balanço das demandas entre os nós.

#### Como compilar

O software for desenvolvido na linguagem C++, no ambiente Linux.
Para compilar, é necessário ter o programa `gcc` e `g++` com suporte ao padrão C++11.

Tendo isso em mente, a compilação se dá pelo comando:

```
g++ *.cpp -o main -std=c++11 -Wall
```

E para executar o código, a estrutura de comando abaixo deve ser utilizada:

```
./main <caminho_arquivo_topologia> <caminho_arquivo_trafego>
```

Exemplo utilizando um dos testes incluídos com o simulador:

```
./main testes/topologia testes/trafego
```