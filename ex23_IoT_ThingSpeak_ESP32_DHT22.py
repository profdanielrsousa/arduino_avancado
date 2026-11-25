# VIGÉSIMO TERCEIRO EXEMPLO (IoT COM THINGSPEAK, ESP32 E SENSOR DHT22)
# DANIEL RODRIGUES DE SOUSA 17/10/2024
# PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481680148000447489

import time                           # Importa a biblioteca para manipulação de tempo
import random                         # Importa a biblioteca para geração de números pseudoaleatórios
import matplotlib.pyplot as plt       # Importa o módulo pyplot do Matplotlib para geração de gráficos
import matplotlib.animation as animation  # Importa o módulo de animação para atualização dinâmica de gráficos
import requests                       # Importa a biblioteca para execução de requisições HTTP

# Configurações do canal no ThingSpeak
channel_id = 'SEU_CHANNEL_ID'         # Define o identificador numérico do canal (Channel ID) no ThingSpeak
read_api_key = 'SEU_READ_API_KEY'     # Define a chave de leitura (Read API Key) do canal no ThingSpeak
field_number = 1                      # Define o número do campo (Field 1 = Temperatura) a ser consultado

# Monta a URL de consulta da API REST do ThingSpeak para obter os últimos 10 registros
url = f'https://api.thingspeak.com/channels/{channel_id}/fields/{field_number}.json?api_key={read_api_key}&results=10'

dataList = []                         # Inicializa a lista global que armazena as leituras extraídas do canal

def fetch_data():                     # Consulta a API REST do ThingSpeak e atualiza a lista global de dados
    global dataList
    try:
        response = requests.get(url)  # Executa a requisição HTTP GET na URL configurada do ThingSpeak
        response.raise_for_status()   # Verifica o código de status HTTP e lança exceção em caso de erro

        data = response.json()        # Converte o corpo da resposta HTTP de formato JSON para dicionário Python
        dataList = [float(entry['field1']) for entry in data['feeds'] if entry['field1'] is not None]  # Extrai os valores numéricos válidos do campo 1
    except requests.RequestException as e:  # Trata falhas de conexão, timeout ou erro HTTP na requisição
        print(f"Erro na requisição de dados: {e}")  # Exibe a descrição do erro de rede no console
        dataList = []                 # Limpa a lista de dados em caso de falha na comunicação
    except ValueError as e:           # Trata erros de conversão de tipo ou decodificação JSON
        print(f"Erro ao processar os dados recebidos: {e}")  # Exibe a descrição do erro de processamento no console
        dataList = []                 # Limpa a lista de dados em caso de falha na conversão

def animate(i):                       # Atualiza o quadro da animação do Matplotlib com os dados mais recentes
    fetch_data()                      # Busca os últimos registros armazenados no canal do ThingSpeak
    ax.clear()                        # Limpa os eixos do gráfico anterior antes da nova plotagem
    ax.plot(dataList)                 # Traça a curva com os valores de temperatura recuperados
    ax.set_ylim([0, 50])              # Fixa os limites do eixo vertical (Y) entre 0 °C e 50 °C
    ax.set_title("Dados do ThingSpeak (Temperatura)")  # Define o título superior do gráfico
    ax.set_ylabel("Temperatura (°C)") # Define o rótulo do eixo vertical (Y)

fig = plt.figure()                    # Cria a janela principal da figura do Matplotlib
ax  = fig.add_subplot(111)            # Adiciona uma grade única de eixos (1 linha, 1 coluna, índice 1)

ani = animation.FuncAnimation(fig, animate, interval=15000)  # Configura a atualização automática do gráfico a cada 15000 ms (15 s)

plt.show()                            # Exibe a janela do gráfico e mantém o loop de eventos ativo até o fechamento
