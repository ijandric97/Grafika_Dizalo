# Grafika_Dizalo

Projekt za predmet Računalna Grafika. Napravljen korištenjem OpenGL 1.2, FreeGLUT
i GLM librarya.

Opis rješenja:
	
	Program inicijalizira prozor velicine 800x600 u gornjem lijevom kutu.

	Svjetlo je postavljeno pri samom vrhu, i malo ispred, te mu je boja
	ambient light-a postavljena na jako tamnu plavu (jer mi je izgledalo
	ljepse nego sivo).

	Kamera je postavljena na kruznicu oko centra te na visinu koja odgovara
	polovini maksimalnoga puta. Moze se kretati po kruznici (yaw) te zumirati
	(pitching i rolling nisu implementirani). U Pocetku je kamera dosta
	odzumirana jer je scena "velika", te je zakrenuta u lijevo (lijepse se vidi).

	Dizalo (elevator), uteg (weight) i prizemlje? (outside) se crtaju na isti
	nacin, znaci kao quadovi, i za svaki vertex quad-a izracunavamo normalu.
	Dizalo smo translatirali naprijed, a uteg nazad; te ih ovisno o animaciji
	translatiramo na odgovarajucu visinu. Prizemlje? (neznam kako to nazvat)
	se sastoji od tri iste plohe, samo je ona u sredini zute boje, dok su
	ostale dvije translatirane naprijed i nazad, zelene/plave, te su sve skupa
	translatirane malo u dolje tako da lift ne clippa u njih.

	Kabeli (cables) su nacrtani kao linije, te smo ih prije crtanja podebljali
	tako da se bolje vide. Imaju sivu boju, ali zbog ambijentalnog svijetla
	izgledaju plavo/sivo. Kod crtanja ih ne translatiram i skaliram, vec im
	mijenjam lokaciju vertex-a. (jer mi je tako bilo jednostavnije, i manje koda
	za napisat.)
	
	Na kraju na near plane nacrtamo text koji pokazuje sto koja tipka radi, te
	trenutnu poziciju kamere.

	Animacija je izvedena preko timer funkcije, te se u njoj mijenja trenutna
	visina dizala, a iz trenutne visine dizala racunamo njoj obrnutu visinu
	na koju postavljamo uteg. Ima dvije brzine, prvu koja je aktivna pri dnu
	i vrhu, i brzu (cetiri puta vecu) koja je aktivna sredinom puta (tako da se
	dobije iluzija ubrzanja).

	Meni, fullscreen su dodani jer sam htio isprobat kako funkcioniraju.

Tipke za upravljanje programom:
	
	←, →	Rotiranje kamere oko y osi
	↑, ↓	Zumiranje kamere
	SPACE	Pokretanje / Resetiranje animacije
	ESC	Izlaz iz programa
	F1	Fullscreen on/off

	Right Mouse Button	Mali meni koji sadrzi Pokretanje, Izlaz i Fullscreen

Command line argumenti:
	
	Nisam implementirao argumente pri pokretanju.

Reference:
	
	Laboratorijska Vjezba 6 (P1, P2)
	https://www.opengl.org/resources/libraries/glut/spec3/spec3.html
	http://www.glprogramming.com/red/
	http://www.glprogramming.com/blue/
	http://openinggl.blogspot.com/
	http://programmingexamples.net/wiki/OpenGL		

Testirano na:
	
	Xubuntu 18.04 x64
	cmake 3.10.2
	gcc (Ubuntu 7.3.0-16ubuntu3) 7.3.0
	g++ (Ubuntu 7.3.0-16ubuntu3) 7.3.0
	glm (0.9.9~a2-2) unstable
	freeglut (2.8.1-3) unstable
	3.0 Mesa 18.0.0-rc5
	Qt Creator 4.5.2
