# BSN

**B**ody **S**ensor **N**etwork é o protótipo de sistema de software para uma rede de sensores para monitoramento de sinais vitais do corpo humano com resposta em tempo real e autônomo.

### TO-DO
Refatorar código dos sensores/leaf-tasks
	Refatorar sensor ECG
		Collect
		Filter
		Transfer
	Refatorar sensor Oxímetro
		Collect
		Filter
		Transfer
	Refatorar sensor Esfigmomanômetro
		Collect
		Filter
		Transfer
	Refatorar sensor Termômetro
		Collect
		Filter
		Transfer
Implementar Oráculo para cada um dos sensores (usar )
Implementar Erro para cada um dos sensores
Implementar serviço de mensagens entre módulos de cada sensor
	Alterar libbsn/msg/data/ para permitir mensagens entre tasks
Pensar em outros tipos de erro além de um range aleatório
	Ex.: Não filtrar os dados, envio de tipo de dados errados, falhas e desvios do serviço esperado

### Branch de Fault Tolerance

Esta branch foi feita para experimentos relacionados a métricas de reliability e confiability, implementando uma simulação de falha de tarefas/leaf-tasks específicas ao longo da simulação, com a possibilidade de identificar e quantificar o número de falhas que ocorreram ao longo da execução do programa.

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
O comando 

```
sudo odsupercomponent --cid=111 --freq=10 --realtime=20 --managed=simulation_rt
```
é utilizado para iniciar a execução de uma conferência. Cada conferência é definida pelo parâmetro --cid. Para iniciar a execução do CentralHub, é necessário estar dentro da pasta configs:

```
cd odv/centralhub/configs
```
e então, iniciar o odsupercomponent, com um cid à sua escolha. Após isso, em um outro terminal, utilize
```
cd odv/centralhub/listener && ./tcp_listenerApp --cid=
```
e 
```
cd odv/centralhub/processor && ./ProcessorApp --cid= 
```
com o mesmo cid do odsupercomponent iniciado nas configurações do CentralHub. 

Para execução do SensorNode, é necessário iniciar um odsupercomponent para cada tipo de sensor. Atualmente, temos 4 tipos de sensores: termômetro, oxímetro, eletrocardíografo e pressão arterial. Cada sensor possui seu arquivo de configuração próprio, encontrado em odv/sensornode/configs e é possível utilizar mais de um sensor na mesma conferência, sendo necessário apenas mudanças nas configurações.

Para executar o SensorNode por completo, é necessário utilizar os seguintes comandos, após iniciar um odsupercomponent com cid própria:
```
cd odv/sensornode/collector/build && ./DataCollectorApp --cid=
```
```
cd odv/sensornode/filter/build && ./FilterModulleApp --cid=
```
```
cd odv/sensornode/sender/build && ./SenderApp --cid=
```
Cada comando deve ser utilizado em terminais distintos, estando na pasta bsn. A cid deve ser a mesma utilizada pelo odsupercomponent correspondente à conferência que deseja executar.

O modo de escalonamento do odsupercomponent será First Come Fist Served (FCFS) com sincronização dos módulos com a frequência de 10Hz.

## Configurações de teste

* Sistema Operacional - Linux Ubuntu 16.04
* Compilador -  gcc version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.5)

## Autores

* **Ricardo D. Caldas** - https://github.com/rdinizcal
* **Gabriel Levi** - https://github.com/gabrielevi10
* **Léo Moraes** - https://github.com/leooleo
* **Jorge G. L. Mendes** - https://github.com/luzmendesj 
