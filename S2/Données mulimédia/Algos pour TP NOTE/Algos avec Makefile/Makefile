########################################
#~ définitions
########################################

BIN=bin/blur bin/difference bin/dilatation bin/dilatation_grey bin/dilatation_rgb bin/erosion bin/erosion_grey bin/erosion_rgb 
bin/fermeture bin/filtre_flou1 bin/filtre_flou2 bin/filtre_flou_rgb bin/filtre_gauss bin/filtre_moyenne 
bin/histo bin/histo_rgb bin/hysteresis bin/inverse bin/norme_gradient bin/ouverture bin/profil bin/seuil_grey bin/test_couleur bin/test_couleur bin/test_grey bin/test_grey_3 bin/test_grey_4 


# liste des fichiers sources 
SRCS0=blur.cpp
SRCS1=difference.cpp
SRCS2=dilatation.cpp
SRCS3=dilatation_grey.cpp
SRCS4=dilatation_rgb.cpp
SRCS5=erosion.cpp
SRCS6=erosion_grey.cpp
SRCS7=erosion_rgb.cpp
SRCS8=fermeture.cpp
SRCS9=filtre_flou1.cpp
SRCS10=filtre_flou2.cpp
SRCS11=filtre_flou_rgb.cpp
SRCS12=filtre_gauss.cpp
SRCS13=filtre_moyenne.cpp
SRCS14=histo.cpp
SRCS15=histo_rgb.cpp
SRCS16=hysteresis.cpp
SRCS17=inverse.cpp
SRCS18=norme_gradient.cpp
SRCS19=ouverture.cpp
SRCS20=profil.cpp
SRCS21=seuil_grey.cpp
SRCS22=test_couleur.cpp
SRCS23=test_grey.cpp
SRCS24=test_grey_3.cpp
SRCS25=test_grey_4.cpp

default: $(BIN)

########################################
#~ regles pour l'executable
########################################

obj/%.o: %.cpp
	gcc -Wall -Iinclude -c $< -o $@

bin/serveur: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

bin/blur: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/difference: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/dilatation: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/dilatation_grey: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/dilatation_rgb: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/erosion: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/erosion_grey: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/erosion_rgb: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/fermeture: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/filtre_flou1: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+ 
bin/filtre_flou2: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/filtre_flou_rgb: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/filtre_gauss: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/filtre_moyenne: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/histo: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/histo_rgb: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/hysteresis: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/inverse: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/norme_gradient: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/ouverture: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/profil: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/seuil_grey: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/test_couleur: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/test_couleur: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/test_grey: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/test_grey_3: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+
bin/test_grey_4: $(SRCS1:%.c=obj/%.o)
	gcc -o $@ $+

clean:
	rm -f $(BIN) obj/*.o *~
