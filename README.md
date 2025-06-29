# 42-Minishell

si haces make y ejecutas ./minishell entraras ya en el programa y de momento lo que hace es leer un comando y lo divide en tokens y desps a partir ede esos tokens monta el arbol. Aun no hay comprobaciones de inputs incorrectos.
So pruebas por ejemplo: < infile ls -l | wc -l | grep al > outfile

Deberia devolverte:
                                 R─[8] grep al
                      R─[3] >
                                 L─[0] outfile

           R─[1] |
                      L─[8] wc -l
s─[1] |
                      R─[8] ls -l
           L─[2] <
                      L─[0] infile

que es una representacion grafica del arbol para que te sea mas facil entenderlo.

el primer nodo(el de la derecha), tiene una s de start que es donde empieza el arbol, en numero entre [] es el tipo del nodo, siendo:
0 ->TOKEN_WORD
1 -> TOKEN_PIPE
2 -> TOKEN_REDIRECT_IN,
3 -> TOKEN_REDIRECT_OUT,
4 -> TOKEN_HEREDOC,
5 -> TOKEN_SEMICOLON,
6 -> TOKEN_VARIABLE,
7 -> TOKEN_ENV_VAR,
8 -> TOKEN_COMMAND


Despues a la derecha del [] esta el contenido del nodo, el resto de nodos empiezan por R para los de la derecha (right) y L para los del la izquiera (left).

Cualquier cosa me dices :D
