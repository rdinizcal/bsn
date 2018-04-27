# OpenBASN

**Open** **B**ody **A**rea **S**ensor **N**etwork é o protótipo de rede de sensores para monitoramento de sinais vitais do corpo humano com resposta em tempo real e autônomo desenvolvido para o Trabalho de Graduação em Engenharia Mecatrônica na Universidade de Brasília de Ricardo Diniz Caldas.

## Introdução

Estas instruções descrevem o conteúdo do galho (tcc) do repositório OpenBASN e apresentam por meio de um passo-a-passo como instalar, compilar e executar o protótipo dem ambiente Linux.

## Descrição

No presente galho do repositório encontram-se o protótipo no OpenDaVINCI, e modelo do UPPAAL utilizados na apresentação do Trabalho de Graduação em Engenharia Mecatrônica na Universidade de Brasília do aluno Ricardo Diniz Caldas, na data de 08/12/2017, de título "Prototipação e Verificação Formal de Sistema Autônomo com Propriedades Tempo-Real: Um estudo de caso no Body Sensor Network".

### Pré-Requisitos

É necessário ter a versão v4.16.0 do OpenDaVINCI instalada para executar o protótipo. O *framework* pode ser encontrado no repositório: https://github.com/se-research/OpenDaVINCI, onde há instruções para instalação para diversos sistemas operacionais.

Para execução em tempo real (*realtime*) o sistema operacional de execução do protótipo deve permitir esse tipo de operação. 

### Instalação

Um passo-a-passo com exemplos descrevem como instalar o OpenBASN (testado em Linux Ubuntu 16.04).

Clone o galho específico do repositório

```
git clone -b tcc https://github.com/rdinizcal/OpenBASN.git
```

Entre na pasta da biblioteca libopenbasn

## Compilação

Apenas rode o script com:

```
bash script.sh
```

### Execução

Para executar o protótipo deve-se executar o escalonador dos módulos do próprio OpenDaVINCI, o odsupercomponent, para então emakxecutá-los:

Antes de executar, o arquivo configuration.txt do diretorio *configs* deve ser colocado em */opt/od/bin/*

```
sudo cp /configs/configuration /opt/od/bin/
```

Exemplo de execução do odsupercomponent(Na pasta configs):

```
sudo odsupercomponent --cid=111 --freq=10 --realtime=20 --managed=simulation_rt
```

E o módulo bodyhub(Na pasta bodyhub/build):
```
./bodyhub --cid=111
```
E o módulo sensornode(Na pasta sensornode/build):
```
./sensornode --cid=111 --id=0
```

E o módulo sensor(Na pasta sensor/build):

```
./sensor
```

Nesta configuração o protótipo será executado com uma plataforma central, três sensores (termômetro, eletrocardiógrafo e oxímetro). O modo de escalonamento do odsupercomponent será First Come Fist Served (FCFS) com sincronização dos módulos com a frequência de 10Hz.

## Configurações de teste

* Sistema Operacional - Linux Ubuntu 16.04
* Compilador -  gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.5)

## Autores

* **Ricardo D. Caldas** - (https://github.com/rdinizcal)