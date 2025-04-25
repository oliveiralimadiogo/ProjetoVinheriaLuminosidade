# 🍷 Sistema de Monitoramento de Luminosidade – Vinheria Agnello
## 📋 Descrição do Projeto
O objetivo é desenvolver um sistema de monitoramento para o ambiente de armazenamento dos vinhos. Segundo o proprietário da vinheria, a qualidade do vinho é fortemente impactada por fatores como temperatura, umidade e luminosidade, sendo fundamental manter essas condições sob controle

## 🎯 Objetivos
Medir a intensidade da luz ambiente com sensor LDR.

Sinalizar a situação com LEDs:

🟢 Verde: ambiente ideal (escuro);

🟡 Amarelo: nível de alerta (meia-luz);

🔴 Vermelho: nível crítico (muita luz).

Quando em alerta, um buzzer toca por 3 segundos, repete o aviso se o nível se mantiver.

## ⚙️ Componentes Utilizados
1x Arduino Uno

1x Sensor LDR

1x Resistor 1kΩ (divisor de tensão para o LDR)

3x LEDs (verde, amarelo e vermelho)

3x Resistores 220Ω (para os LEDs)

1x Buzzer

Protoboard e jumpers

## 🛠️ Como Reproduzir
✅ Montagem do circuito:
Monte o LDR com resistor de 10kΩ formando um divisor de tensão:

Um lado do LDR no 5V, outro lado no pino A0 e ligado ao resistor de 10kΩ que vai para o GND.

Conecte os LEDs nos pinos 2 (verde), 3 (amarelo) e 4 (vermelho).

Conecte o buzzer no pino 5.

Use resistores adequados para proteger os LEDs.

## 🕹️ TinkerCad

- [Projeto](https://www.tinkercad.com/things/iryyTOm2NdR-checkpoint-edge/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard&sharecode=YwfjBHHS2fJPUteCx_2BqlvWlVWSF4Dw36xt0A6M0LY)

## 💻 Upload do código:
Abra o Arduino IDE;

Cole o código-fonte (presente neste repositório);

Selecione a placa e porta corretas;

Faça o upload para o Arduino.

## 🧪 Teste:
Varie a luz incidente sobre o LDR e observe a mudança de LEDs e o acionamento do buzzer.

## ⏱ Lógica com millis() (sem delay)
O buzzer é ativado por 3 segundos, depois desativado por 3 segundos, em um ciclo contínuo enquanto o ambiente estiver em nível de alerta. Isso é feito usando a função millis(), que permite controle de tempo sem travar o loop principal, diferente do delay().

## Desenvolvedores do projeto :busts_in_silhouette:

- [Diogo Oliveira Lima](https://github.com/oliveiralimadiogo)
- [Eduarda de Castro Coutinho dos Santos](https://github.com/DudaFror)
- [Lucas dos Reis Aquino](https://github.com/LucassAquino)
- [Lucas Perez Bonato](https://github.com/LucasBonato)

