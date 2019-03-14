# Arduino et timers


L'initialisation du timer est fournie.

À chaque cycle de l'horloge, un compteur est incrémenté et il est
comparé à la valeur se trouvant dans un registre (registre
OCR1A dans notre cas). Si ils sont égaux, une interruption est
générée. Le programme est alors dérouté pour exécuter la fonction
correspondante et le compteur est réinitialisé.

Un exemple http://http://www.avrbeginners.net/architecture/timers/timers.html



# Interruptions en arduino?

Les fonctions d'interruptions, dans le cas d'un micro-contrôleur AVR
et de l'utilisation de avr-gcc sont nommées ISR et prennent en
paramètre le vecteur d'interruption correspondant (dans notre cas, il
s'agit de TIMER1_COMPA_vect). Le timer est initialisé avec un
prescaler de 1024, cela signifie que le compteur est incrémenté tous
les 1024 cycles. L'arduino est cadencé à 16 MHz.

Pour activer les interruptions
vous devez appeler la fonction \verb!sei()!. Pour désactiver les
interruptions, la fonction est \verb!cli()!. 

Doc : http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html

