# VIGÉSIMO QUINTO EXEMPLO (IoT COM SERVIDOR MQTT, ESP32 E ADC)
# DANIEL RODRIGUES DE SOUSA 17/05/2024
# PROJETO DISPONÍVEL EM: https://wokwi.com/projects/481683117975459841

import time                               # Importa a biblioteca para manipulação de tempo
import random                             # Importa a biblioteca para geração de números pseudoaleatórios
import logging                            # Importa a biblioteca padrão de registro de eventos (logs)
from collections import deque             # Importa a estrutura de fila dupla com tamanho máximo fixo

import matplotlib.pyplot as plt           # Importa o módulo pyplot do Matplotlib para geração de gráficos
import matplotlib.animation as animation  # Importa o módulo de animação para atualização dinâmica de gráficos
import paho.mqtt.client as mqtt           # Importa o cliente MQTT da biblioteca Paho MQTT

# Configuração do sistema de log estruturado
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
)
log = logging.getLogger(__name__)         # Instancia o logger específico deste módulo

# Configurações do broker MQTT
# OBS: Caso broker.hivemq.com falhe (gaierror 11004), utilize "test.mosquitto.org" ou "mqtt.eclipseprojects.io".
# Esse erro indica bloqueio da porta 1883 por firewall/antivírus ou falha de resolução DNS.
BROKER    = "test.mosquitto.org"          # Define o endereço do broker MQTT público
PORT      = 1883                          # Define a porta TCP padrão do protocolo MQTT
TOPIC     = "esp32/adc"                   # Define o tópico MQTT onde o ESP32 publica as leituras do ADC
KEEPALIVE = 60                            # Define o intervalo máximo (em segundos) de keep-alive da sessão MQTT

random.seed(time.time())                  # Inicializa a semente do gerador pseudoaleatório com o relógio do sistema
CLIENT_ID = f"clientId-{random.randint(1000, 10000)}"  # Gera um identificador único para o cliente MQTT

# Buffer circular: mantém automaticamente no máximo 50 amostras recentes em memória
MAX_SAMPLES = 50                          # Define a capacidade máxima de amostras exibidas na janela do gráfico
data_buffer: deque[float] = deque(maxlen=MAX_SAMPLES)  # Instancia o buffer circular tipado

# Funções de callback MQTT — API v2
def on_connect(client, userdata, flags, reason_code, properties):  # Callback acionado após tentativa de conexão ao broker
    if(reason_code == 0):                 # Verifica se o código de retorno indica conexão bem-sucedida
        log.info("Conectado ao broker MQTT. Inscrevendo em '%s'…", TOPIC)  # Registra sucesso e tópico alvo
        client.subscribe(TOPIC)           # Realiza a inscrição no tópico de telemetria do ADC
    else:                                 # Caso o broker recuse a conexão
        log.error("Falha na conexão — código: %s", reason_code)  # Registra o código de erro retornado


def on_message(client, userdata, message):  # Callback acionado a cada nova mensagem recebida no tópico inscrito
    try:
        value = float(message.payload.decode("ascii"))  # Decodifica o payload ASCII e converte para ponto flutuante
        data_buffer.append(value)         # Insere o valor convertido no final do buffer circular
        log.debug("Valor recebido: %.2f", value)  # Registra o valor recebido em nível de depuração
    except ValueError as exc:             # Trata payloads malformados ou não numéricos
        log.warning("Payload inválido ignorado: %s (%s)", message.payload, exc)  # Registra aviso sem interromper execução


def on_disconnect(client, userdata, flags, reason_code, properties):  # Callback acionado ao encerrar ou perder a conexão
    if(reason_code != 0):                 # Verifica se a desconexão ocorreu de forma inesperada
        log.warning("Desconexão inesperada — código: %s", reason_code)  # Registra alerta de perda de conexão

# Instanciação e configuração do cliente MQTT (API v2)
client = mqtt.Client(
    mqtt.CallbackAPIVersion.VERSION2,
    client_id=CLIENT_ID,
)
client.on_connect    = on_connect         # Vincula o callback de conexão estabelecida
client.on_message    = on_message         # Vincula o callback de recepção de mensagens
client.on_disconnect = on_disconnect      # Vincula o callback de desconexão

try:
    client.connect(BROKER, PORT, KEEPALIVE)  # Estabelece a conexão TCP/MQTT com o broker configurado
except OSError as exc:                    # Trata erros de rede, DNS (gaierror) ou bloqueio de porta
    log.error(
        "Não foi possível conectar ao broker '%s:%s'.\n"
        "  → Verifique sua conexão, firewall e se a porta %s está liberada.\n"
        "  → Tente trocar BROKER por 'test.mosquitto.org'.\n"
        "  Erro original: %s",
        BROKER, PORT, PORT, exc,
    )
    raise SystemExit(1)                   # Encerra o script com código de erro 1

client.loop_start()                       # Inicia a thread em segundo plano para processamento de rede MQTT

# Configuração da janela e da animação do Matplotlib
fig, ax = plt.subplots()                  # Cria a figura e o conjunto de eixos do gráfico


def animate(_frame):                      # Atualiza o quadro da animação com as amostras armazenadas no buffer
    ax.clear()                            # Limpa os eixos anteriores antes de redesenhar a curva
    ax.plot(list(data_buffer))            # Converte o buffer circular em lista e plota os pontos
    ax.set_ylim(0, 4200)                  # Fixa os limites verticais para a resolução de 12 bits do ADC (0 a 4095)
    ax.set_xlim(0, MAX_SAMPLES)           # Fixa o eixo horizontal ao número máximo de amostras do buffer
    ax.set_title("Dados do ESP32 ADC")    # Define o título superior do gráfico
    ax.set_xlabel("Amostras recentes")    # Define o rótulo do eixo horizontal (X)
    ax.set_ylabel("Valor ADC")            # Define o rótulo do eixo vertical (Y)
    ax.grid(True, linestyle="--", alpha=0.5)  # Habilita a grade tracejada semitransparente de fundo


ani = animation.FuncAnimation(fig, animate, interval=100, cache_frame_data=False)  # Atualiza o gráfico a cada 100 ms

log.info("Gráfico aberto. Feche a janela para encerrar.")  # Informa ao usuário como encerrar a aplicação
plt.show()                                # Exibe a janela gráfica e bloqueia a thread principal até o fechamento

# Rotina de limpeza ao encerrar a janela gráfica
client.loop_stop()                        # Encerra a thread de comunicação em segundo plano do cliente MQTT
client.disconnect()                       # Envia pacote DISCONNECT e fecha o socket com o broker MQTT
log.info("Cliente MQTT desconectado. Programa encerrado.")  # Registra o encerramento limpo da aplicação
