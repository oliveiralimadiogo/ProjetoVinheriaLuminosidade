# 🍷 Sistema de Monitoramento de Luminosidade – Vinheria Agnello
## 📋 Descrição do Projeto

O objetivo é desenvolver um sistema de monitoramento para o ambiente de armazenamento dos vinhos. Segundo o proprietário da vinheria, a qualidade do vinho é fortemente impactada por fatores como temperatura, umidade e luminosidade, sendo fundamental manter essas condições sob controle

## 🎯 Objetivos
Medir a intensidade da luz ambiente com sensor LDR, Medir a temperatura e a umidade do ambiente e mostrar essas informações em um display.

Cores do led acessas:

🟢 Verde: 
ambiente ideal (escuro)  
10Cº < temperatura < 15Cº Display("Temperatura OK" + temperatura)  
50% < umidade < 70% Display("Umidade OK" + umidade)  


🟡 Amarelo: 
nível de alerta (meia-luz); Display(“Ambiente a meia luz” deve ser mostrado no Display;)  
Temperatura < 10Cº Display("Temp. Baixa" + Valor Temperatura) e buzzer ativo  
Temperatura > 15Cº Display("Temp. Alta" + Valor Temperatura) e buzzer ativo  


🔴 Vermelho: 
nível crítico (muita luz); Display(“Ambiente muito claro” deve ser mostrado no display) Buzzer ativo  
umidade < 50% Display("Umidade Baixa" + umidade) e buzzer ativo  
umidade > 70% Display("Umidade Alta" + umidade) e buzzer ativo  



Quando em alerta, um buzzer toca por 3 segundos, repete o aviso se o nível se mantiver.

## ⚙️ Componentes Utilizados
1x Arduino Uno

1x Sensor LDR

1x Resistor 1kΩ (divisor de tensão para o LDR)

3x LEDs (verde, amarelo e vermelho)

3x Resistores 220Ω (para os LEDs)

1x Buzzer

1X Sensor DHT11

1X Display LCD

Protoboard e jumpers

## 🕹️ TinkerCad

- [Projeto](https://www.tinkercad.com/things/iryyTOm2NdR-checkpoint-edge/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=YwfjBHHS2fJPUteCx_2BqlvWlVWSF4Dw36xt0A6M0LY)

## 💻 Upload do código:
Abra o Arduino IDE;

Cole o código-fonte (presente neste repositório);

Selecione a placa e porta corretas;

Faça o upload para o Arduino.

## 🧪 Teste:
Varie a luz incidente sobre o LDR e observe a mudança de LEDs e o acionamento do buzzer.  
Varie a temperatura e a umidade do ambiente.

## Desenvolvedores do projeto :busts_in_silhouette:

- [Diogo Oliveira Lima | RM: 562559](https://github.com/oliveiralimadiogo)
- [Eduarda de Castro Coutinho dos Santos | RM: 562184](https://github.com/DudaFror)
- [Lucas dos Reis Aquino | RM: 562414 ](https://github.com/LucassAquino)
- [Lucas Perez Bonato | RM: 565356](https://github.com/LucasBonato)
- [Leandro Simoneli | RM: 566539](https://github.com/Leo010906)

