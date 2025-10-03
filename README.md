# Estação de Monitoramento Ambiental com ESP32

## Visão Geral do Projeto

Este projeto consiste em uma estação de monitoramento ambiental autônoma, construída com base na plataforma ESP32. O dispositivo coleta dados de temperatura, umidade e luminosidade do ambiente, exibindo-os em tempo real através de múltiplas interfaces: um display OLED local, um painel de indicadores LED e uma página web acessível via Wi-Fi.

O sistema opera em modo Access Point, criando sua própria rede sem fio para permitir o monitoramento remoto a partir de qualquer dispositivo com navegador, sem depender de uma infraestrutura de rede externa.

## Funcionalidades Principais

* **Monitoramento em Tempo Real:** Leitura contínua dos sensores de temperatura, umidade (DHT11) e luminosidade (LDR).
* **Interface de Dados Dupla:** As informações são exibidas simultaneamente em um display OLED local e em uma página web responsiva.
* **Painel de Status Visual:**
    * **Climômetro:** Um conjunto de LEDs (azul, verde, vermelho) que indica a faixa de temperatura atual (Frio, Ideal, Quente) com base nas recomendações da OMS.
    * **Indicador de Conforto:** Um módulo de semáforo que sinaliza a qualidade da umidade do ar (Seco, Ideal, Úmido).
    * **LED de Status:** Um LED branco indica que o sistema está totalmente operacional e a rede Wi-Fi está ativa.
* **Operação Autônoma:** O dispositivo funciona com qualquer fonte de alimentação USB (5V) e cria seu próprio Ponto de Acesso (Access Point) Wi-Fi, tornando-o totalmente portátil.

## Componentes de Hardware

* 1x Placa de Desenvolvimento ESP32
* 1x Sensor de Temperatura e Umidade DHT11
* 1x Sensor de Luminosidade (LDR)
* 1x Resistor de 10kΩ (para o LDR)
* 1x Display OLED 128x64 I2C
* 1x LED Azul (5mm)
* 1x LED Verde (5mm)
* 1x LED Vermelho (5mm)
* 1x LED Branco (5mm)
* 4x Resistores de 220Ω (para os LEDs)
* 1x Módulo de Semáforo (LEDs Vermelho, Amarelo, Verde)
* 1x Protoboard
* Jumpers (macho-macho)

## Bibliotecas de Software

As seguintes bibliotecas precisam ser instaladas na IDE Arduino através do "Gerenciador de Bibliotecas":

* `DHT sensor library` by Adafruit
* `Adafruit Unified Sensor` by Adafruit
* `Adafruit GFX Library` by Adafruit
* `Adafruit SSD1306` by Adafruit

## Instruções de Uso

1.  **Montagem:** Monte o circuito conforme especificado no arquivo `esquematicos/guia_de_conexoes.txt`.
2.  **Alimentação:** Energize o ESP32 utilizando uma fonte USB de 5V (carregador de celular, power bank, etc.). O LED branco de status acenderá quando o sistema estiver pronto.
3.  **Conexão:** Em um smartphone ou computador, procure as redes Wi-Fi disponíveis e conecte-se à rede com o nome (SSID) **`Guardiao_do_Lar_AP`**. A senha é **`12345678`**.
4.  **Acesso:** Após conectar-se à rede, abra um navegador de internet e acesse o endereço: **`http://192.168.4.1`**.
5.  **Monitoramento:** A página web exibirá os dados dos sensores, que são atualizados automaticamente.
