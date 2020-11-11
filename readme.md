# Coelho esperto 1

Este jogo foi feito para JujubA, minha esposa, como presente de aniversário.

Ela gosta de puzzles e portanto fiz ele razoavelmente difícil. Caso muitas pessoas peçam posso vir a fazer um tutorial de como passar das fases mais complicadas.
Não tenho intenção de gastar muito mais tempo em código neste pois já estou trabalhando em outro projeto (o tal jogo 2) que vai ser melhor (espero).

Para rodar ele utilizei a GBDK, uma lib de desenvolvimento em c para gameboy (mais especificadamente a: `lcc.c,v 1.6 2001/10/28 18:38:13 michaelh Exp $`)

## O jogo
O arquivo coelho_esperto_1.3.gb é a rom valida que roda em gameboy e outros emuladores.
https://github.com/nimbfire/jujuba-gb-game/raw/master/coelho_esperto_1.3.gb

Por uma questão de não rodei no gb as primeiras builds não percebi que os sprites eram minusculos. Para jogar recomendo um emulador que aumente a tela em 4 vezes, mas 2x já ajuda bastante. É possivel jogar em tela x1, mas não recomendo. 


## Desafios

Um dos grandes problemas que tive ao desenvovler para gameboy foi a falta de conhecimento do próprio. Me deparei com situações onde loops demoravam 100 vezes mais que outros por diferenças bobas como testar se o i == 0 ao invés de i == [outro valor].
Tive problemas de conversão de código 8 bits para 16 bits
Me perdi mais de uma vez no C (havia visto ele apenas superficialmente, de forma que foi um desafio mexer nele para o gb)
No fim a rom é uma rom de gbc mas com as cores do gb sendo utilizadas. 
Me quebrei por um tempo com os arrays dos mapas quebrando a build por não ter posto `const` na frente da declaração dos mesmos. Isto fazia com que cada mapa virasse um array em memoria e ocupasse um espaço gigantesco
Foi feito um sistema e fogo que espalha por arvores, apaga com o tempo, entre outros. Mas foi descartado para termos uma build final do game e poder trabalhar no próximo.

## Metodologia

GoHorse. Desculpe aos que entendem de código, mas sempre fiz com o objetivo de ter algo funcional. Cada vez que tentei refatorar e deixar direito alguma limitação do gameboy ou do meu conhecimento me atrapalhava. O próximo vai ser bem melhor.

## Itch.io

https://nimbfire.itch.io/coelho-esperto-1-a-gameboy-game
