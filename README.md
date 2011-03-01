  overo-maple-serial
=======

Test serial commmunication from a Gumstix Overo to a LeafLabs Maple
at TTL signal levels. Nothing special, just a starting point for a real 
project.

Uses a Sparkfun level converter for the 1.8 <-> 3.3 translation. 

The overo-maple-serial.png is a picture of the hookup.


  overo
-------

The overo directory has the C-code for the gumstix side. 

The Makefile is for a cross-compile using the standard OE tools. 

Export an OETMP before calling make if your OE TMPDIR is in a non-standard 
place.

Copy the serial_c executable to the Overo however is convenient.


  maple
-------

The maple pde is under maple/serial_echo. Build and deploy with the Maple IDE
as you would any other project.


  test
-------

root@overo:~# ls
serial_c

root@overo:~# ./serial_c hello
Sent: hello
Received: HELLO

root@overo:~# ./serial_c good-bye
Sent: good-bye
Received: GOOD-BYE

root@overo:~# ./serial_c 
Sent: maple-can-you-hear-me?
Received: MAPLE-CAN-YOU-HEAR-ME?


