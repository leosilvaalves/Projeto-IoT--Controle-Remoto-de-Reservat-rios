
# Controle Remoto de Reservatórios

## Descrição
<p> O projeto utiliza o NodeMCU V2 para obter um controle remoto de um sistema de reservatórios de água via protocolo MQTT e tem o objetivo principal de ser uma solução de baixo custo e acessibilidade para todas as pessoas.
O sistema será composto por dois reservatórios que poderão ser controlados via internet (TCP/IP), esse sistema identificará o seu estado através dos sensores de níveis de água e quando entrar em estado crítico enviará alertas. Com bases nessas informações os responsáveis conseguem analisar o que deve ser feito e caso seja necessário poderão acionar o abastecimento do sistema remotamente.</p>
<p> Este sistema é simples de ser utilizado, através de qualquer aplicativo que permita o envio de mensagens para broker MQTT é possível estar verificando o status do reservatório e acionar o abastecimento remotamente.</p>   

## Hardware Utilizado
- 1 Módulo Esp8266 Esp-12e
- 2 Sensores de nível de água com Boia Horizontal
- 1 Módulo relé 1 saída 10amp 5v
- 1 Mini Bomba de Água Submersível
- 1 Conversor de Nível Lógico RC
- 1 Mangueira de aquário 40cm
- 2 Recipientes para colocar os sensores de nível de água e a bomba de água

## Documentação
O protocolo utilizado é o MQTT que permite verificar o status do reservatório e acionar o seu abastecimento remotamente. 
Para visualizar o estado do reservatório e mandar comandos foi utilizado o aplicativo MQTT Dash que permite assinar e publicar em tópicos MQTT. O broker utilizado é o HiveMQ que já vem como broker padrão no stech de exemplo da biblioteca PubSubClient.
