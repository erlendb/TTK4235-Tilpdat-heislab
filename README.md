Heisdriveren skal i src/driver.

Kompilatoren pleier å klage på at build/ og build/driver/ ikke finnes.

På sal: bruk elev.c/h, io.c/h, channels.h fra oppgavezipen. io.c må legges til Makefila.

For simulator: bruk elev.c/h fra https://github.com/erlendb/simulatorheis_tilpdat


Spørsmål til studass:
* Hvis et håpløst menneske står i samme etasje som heisen er i og bestiller heisen til den etasjen igjen og igjen, blir heisen stående der til evig tid. Kult/ukult?
* Nødstopp -> Kjør mot bestilling motsatt retning av retningen før nødstopp -> Nødstopp -> Bestilling i samme retning som forrige bestilling: heisen tror den er et annet sted og går feil vei. Kult/ukult? Kan løses ved å lagre siste retning før nødstopp i tillegg til lastDirection.
