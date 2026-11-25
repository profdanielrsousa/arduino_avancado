# VIGÉSIMO QUARTO EXEMPLO (IoT COM THINGSPEAK, ESP32 E LED RGB)
# DANIEL RODRIGUES DE SOUSA 17/10/2024
# PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481682063649828865

import time                           # Importa a biblioteca para manipulação de tempo
import random                         # Importa a biblioteca para geração de números pseudoaleatórios
import requests                       # Importa a biblioteca para execução de requisições HTTP
from tkinter import Tk, Button, Frame # Importa os componentes gráficos da biblioteca padrão Tkinter

# Configurações do canal no ThingSpeak
channel_id    = 'SEU_CHANNEL_ID'      # Define o identificador numérico do canal (Channel ID) no ThingSpeak
write_api_key = 'SEU_WRITE_API_KEY'   # Define a chave de escrita (Write API Key) do canal no ThingSpeak

button_size = {'width': 20, 'height': 3}  # Define as dimensões padrão (largura e altura) dos botões da interface

# Monta a URL base da API REST do ThingSpeak para atualização de campos do canal
url = f'https://api.thingspeak.com/update?api_key={write_api_key}'

def send_message(field, mensagem):    # Envia o comando de acionamento (1 ou 0) ao campo especificado no ThingSpeak
    try:
        response = requests.get(url, params={'field' + str(field): mensagem})  # Executa a requisição HTTP GET com o parâmetro do campo
        if(response.status_code == 200):  # Verifica se a requisição HTTP foi aceita pelo servidor ThingSpeak
            print(f"Mensagem '{mensagem}' enviada com sucesso para o Field {field}.")  # Exibe confirmação de envio no console
        else:                         # Caso o servidor retorne código de status diferente de 200
            print(f"Falha ao enviar mensagem '{mensagem}' para o Field {field}. Código de status: {response.status_code}")  # Exibe o código de falha
    except requests.RequestException as e:  # Trata falhas de conexão, timeout ou erro HTTP na requisição
        print(f"Erro ao enviar mensagem: {e}")  # Exibe a descrição da exceção de rede no console

def liga(field):                      # Callback acionado pelo botão para ligar o LED associado ao campo
    send_message(field, '1')          # Envia o valor lógico '1' ao ThingSpeak

def desliga(field):                   # Callback acionado pelo botão para desligar o LED associado ao campo
    send_message(field, '0')          # Envia o valor lógico '0' ao ThingSpeak

root = Tk()                           # Instancia a janela principal da interface gráfica Tkinter
root.title("ThingSpeak (LED RGB)")    # Define o título exibido na barra superior da janela

def criar_botoes_led(cor, field):     # Cria e posiciona o par de botões (Liga/Desliga) para um canal de cor do LED RGB
    frame = Frame(root)               # Cria um contêiner horizontal para agrupar os dois botões da cor
    frame.pack()                      # Empacota o contêiner na janela principal
    Button(frame, text=f"Liga LED {cor}", command=lambda: liga(field), width=button_size['width'], height=button_size['height']).pack(side='left')     # Cria o botão de acionamento alinhado à esquerda
    Button(frame, text=f"Desliga LED {cor}", command=lambda: desliga(field), width=button_size['width'], height=button_size['height']).pack(side='right')  # Cria o botão de desligamento alinhado à direita

criar_botoes_led("Vermelho", 1)       # Instancia os controles do LED vermelho associados ao Field 1
criar_botoes_led("Verde", 2)          # Instancia os controles do LED verde associados ao Field 2
criar_botoes_led("Azul", 3)           # Instancia os controles do LED azul associados ao Field 3

root.mainloop()                       # Inicia o loop principal de eventos da interface gráfica Tkinter
