# BSN

**B**ody **S**ensor **N**etwork é o protótipo de sistema de software para uma rede de sensores para monitoramento de sinais vitais do corpo humano com resposta em tempo real e autônomo.

### Pré-Requisitos

É necessário ter a versão v4.16.0 do OpenDaVINCI instalada para executar o 
protótipo. O *framework* pode ser encontrado no repositório: 
https://github.com/se-research/OpenDaVINCI, onde há instruções para 
instalação para diversos sistemas operacionais. Link para o pacote 
pré-compilado: https://github.com/se-research/OpenDaVINCI/blob/master/docs/installation.pre-compiled.rst

Para execução em tempo real (*realtime*) o sistema operacional de execução do protótipo deve permitir esse tipo de operação. 

É usada também a biblioteca 'cxx test' para testar o programa. Para 
baixá-la basta digitar o seguinte comando:

```
sudo apt-get install cxxtest
```

### Instalação

Um passo-a-passo com exemplos descrevem como instalar o BSN (testado em Linux Ubuntu 16.04).

Clone o galho específico do repositório

```
git clone https://github.com/leooleo/bsn
```

## Compilação

Apenas rode o script com:

```
bash script.sh
```

A instalção será feita de forma automática.

### Execução

Para executar o protótipo deve-se executar o escalonador dos módulos do próprio OpenDaVINCI, o odsupercomponent, para então executá-los:

Antes de executar, o arquivo configuration.txt do diretorio *configs* deve ser colocado em */opt/od/bin/*

```
sudo cp /configs/configuration /opt/od/bin/
```

Exemplo de execução do odsupercomponent(Na pasta configs):

```
sudo odsupercomponent --cid=111 --freq=10 --realtime=20 --managed=simulation_rt
```

E o módulo bodyhub (na pasta bodyhub/build):
```
./bodyhub --cid=111
```
E o módulo sensornode (na pasta sensornode/build):
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

* **Ricardo D. Caldas** - https://github.com/rdinizcal
* **Gabriel Levi** - https://github.com/gabrielevi10
* **Léo Moraes** - https://github.com/leooleo 
