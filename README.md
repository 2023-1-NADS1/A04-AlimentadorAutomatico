# FECAP - Fundação de Comércio Álvares Penteado

<p align="center">
<a href= "https://www.fecap.br/"><img src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRhZPrRa89Kma0ZZogxm0pi-tCn_TLKeHGVxywp-LXAFGR3B1DPouAJYHgKZGV0XTEf4AE&usqp=CAU" alt="FECAP - Fundação de Comércio Álvares Penteado" border="0"></a>
</p>

# A04-Alimentador Automático

## Integrantes: Cleiton Lima, Matheus Adaniya, Wilson Testoni, Gabriel Italo.

## Professores Orientadores: Adriano F. Valente e Victor Bruno.

## Descrição

### Objetivo
Fornecer alimentos seguros e saudáveis, fracionados regularmente para cachorros em situação de rua, tendo em vista a priorização do bem-estar dos animais. Identificar os cães através das fotos feitas pela câmera integrada do ESP32-CAM e fazer o envio das mesmas para ONGs, instituições e petshops parceiros que estejam próximos, assim podendo fazer uma avaliação prévia do estado dos cachorros para que eles possam ser resgatados e tratados.

### O que faz

* Identifica a proximidade do cachorro com o alimentador usando o Sensor de Distância Ultrassônico;<br>
* Uma foto é feita e enviada para a nuvem (Google Drive) pelo ESP32-CAM;<br>
* Libera uma porção de ração feita pelo alimentador, que é composto por um Motor de Passo;<br>
<br>

## 🛠 Estrutura das pastas

-Raiz<br>
|-->documentos<br> 
  &emsp;|-->Projeto-de-extensão-ADS1A-AlimentadorAutomatico3.pdf<br>
|-->imagens<br>
  &emsp;|-->Esquematica_do_trabalho_WOkwI.png<br>
|-->src<br>
  &emsp;  |-->Alimentador-automatico<br>
  &emsp; &emsp; |-->Código_do_Apps_Script_(GoogleDrive)<br>
  &emsp; &emsp; &emsp;|-->Código_Apps_Script<br>
  &emsp; &emsp; |-->alimentador-automatico.ino<br>
  &emsp; &emsp; |-->Base64.cpp<br>
  &emsp; &emsp; |-->Base64.h<br>
  &emsp;  |-->Wokwi<br>
  &emsp; &emsp; |-->Esquematica_do_trabalho_WOkwI.png<br>
  &emsp; &emsp; |-->Link_Wokwi.txt<br>
  
<b>README.MD</b>: Arquivo que serve como guia e explicação geral sobre o projeto.

<b>src</b>: Pasta que contém o código fonte e o link para o Wokwi.

<b>imagens</b>: Imagens da esquemetica do trabalho no Wokwi.
