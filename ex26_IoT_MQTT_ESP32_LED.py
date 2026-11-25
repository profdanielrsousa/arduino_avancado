# VIGÉSIMO SEXTO EXEMPLO (IoT COM SERVIDOR MQTT, ESP32 E LED)
# DANIEL RODRIGUES DE SOUSA 10/05/2024
# PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481683474430575617

import time                       # Importa a biblioteca para manipulação de tempo
import random                     # Importa a biblioteca para geração de números pseudoaleatórios
import paho.mqtt.client as mqtt   # Importa o cliente MQTT da biblioteca Paho MQTT
from tkinter import Tk, Button    # Importa os componentes gráficos da biblioteca padrão Tkinter

random.seed(time.time())          # Inicializa a semente do gerador pseudoaleatório com o relógio do sistema

numero_aleatorio = random.randint(1000, 10000)  # Gera um número inteiro aleatório entre 1000 e 10000

# Configurações do cliente e do broker MQTT
broker    = 'test.mosquitto.org'  # Define o endereço do broker MQTT público
port      = 1883                  # Define a porta TCP padrão do protocolo MQTT
topic     = 'topicName/led'       # Define o tópico MQTT de publicação dos comandos do LED
client_id = 'clientId-' + str(numero_aleatorio)  # Monta a string de identificação única do cliente MQTT

button_size = {'width': 10, 'height': 3}  # Define as dimensões padrão (largura e altura) dos botões da interface

def send_message(mensagem):       # Conecta ao broker MQTT, publica a mensagem no tópico e encerra a sessão
    client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1, client_id)  # Instancia o cliente MQTT com o Client ID gerado
    client.connect(broker, port)  # Estabelece a conexão TCP/MQTT com o broker configurado
    client.publish(topic, mensagem)  # Publica a carga útil ("on" ou "off") no tópico de controle
    client.disconnect()           # Encerra a sessão e fecha a conexão com o broker MQTT

def liga():                       # Callback acionado pelo botão para enviar o comando de ligar o LED
    send_message('on')            # Publica a string 'on' no tópico MQTT

def desliga():                    # Callback acionado pelo botão para enviar o comando de desligar o LED
    send_message('off')           # Publica a string 'off' no tópico MQTT

root = Tk()                       # Instancia a janela principal da interface gráfica Tkinter
root.title("Controle MQTT")       # Define o título exibido na barra superior da janela

Button(root, text="Liga", command=liga, width=button_size['width'], height=button_size['height']).pack(side='left')        # Cria e posiciona o botão de acionamento à esquerda
Button(root, text="Desliga", command=desliga, width=button_size['width'], height=button_size['height']).pack(side='right')  # Cria e posiciona o botão de desligamento à direita

root.mainloop()                   # Inicia o loop principal de eventos da interface gráfica Tkinter
