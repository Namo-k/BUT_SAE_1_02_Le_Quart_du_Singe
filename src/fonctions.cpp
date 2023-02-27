/**
 * @file fonctions.cpp
 * @brief Ent�te du composant de fonctions
 */

#include <iostream>
#include <fstream>
#include <iomanip>
#include <locale>
#include <cstring>

#include "fonctions.h"

#pragma warning(disable:4996,6385)

using namespace std;

/**
* @brief V�rification du nombre de joueur
* @param[in] argv : L'argument utilis� pour compter le nombre de joueur
* @return retourne un bool�an
* @pre argv valide
*/
bool verifNbJoueur(const char* argv[]) {
	// V�rifier que le tableau d'arguments contient au moins un �l�ment
	if (argv == nullptr || argv[0] == nullptr) {
		return false;
	}

	const char* param = argv[1];

	// V�rifier que la cha�ne de caract�res repr�sentant le nombre de joueurs est de longueur minimale MIN_JOUEURS
	if (param == nullptr || strlen(param) < MIN_JOUEURS) {
		return false;
	}

	return true;
}
/**
* @brief V�rifie le type de joueur
* @param[in] argv : L'argument utilis� pour v�rifier le type de joueur
* @return Retourne un bool�en
* @pre argv doit �tre valide
*/
bool verifJoueur(const char* argv[]) {
	// V�rifier que le tableau d'arguments contient au moins un �l�ment
	if (argv == nullptr || argv[0] == nullptr) {
		return false;
	}

	const char* param = argv[1];
	char type;

	// V�rifier que la cha�ne de caract�res repr�sentant le type de joueurs n'est pas vide
	if (param == nullptr || strlen(param) == 0) {
		return false;
	}

	// V�rifier que chaque caract�re de la cha�ne est soit "H" soit "R"
	for (unsigned int i = 0; i < strlen(param); ++i) {
		type = toupper(param[i]);
		if (type != 'H' && type != 'R') {
			return true;
		}
	}

	return false;
}
/**
* @brief Charge le dictionnaire de mots
* @param[in,out] p: La partie � laquelle le dico est associ�
* @pre p est valide
*/
void initialiserDico(Partie& p) {
	p.d.nbMot = 0;

	//Ouverture du dico
	ifstream dico("./ods4.txt");
	if (!dico.good()) cout << "Dico pas ouvert";

	//Se placer au d�but du fichier
	dico.clear();
	dico.seekg(0, ios::beg);

	//Compte le nombre de mots dans le dico
	char* mot = new char[MAX];
	mot[0] = '\0';
	p.d.nbMot = 0;
	while (dico >> setw(MAX) >> mot) {
		p.d.nbMot++;
	}
	delete[] mot;

	//Se placer au d�but du fichier
	dico.clear();
	dico.seekg(0, ios::beg);

	// Allouer un tableau de mots
	char* motTemp;
	motTemp = new char[MAX];
	p.d.mots = new char* [p.d.nbMot];
	for (unsigned int i = 0; i < p.d.nbMot ; ++i) {
		dico >> setw(MAX) >> motTemp;
		p.d.mots[i] = new char[strlen(motTemp)+1];
		strcpy(p.d.mots[i],motTemp);
	}
	delete[] motTemp;

	// Fermer le fichier
	dico.close();
}
/**
* @brief Initialise une partie
* @param[in,out] p: La partie � initialiser
* @param[in] argv : Les arguments de la commande utilis�s pour initialiser la partie
* @pre argv doit �tre valide
* @see initialiserDico
*/
void initialiserPartie(Partie& p, const char* argv[]) {

	const char* param = argv[1];
	p.nbJoueurs = (unsigned int)strlen(param);

	p.joueurs = new Joueur[p.nbJoueurs];

	//Initialisation des types des joueurs
	for (unsigned int i = 0; i < p.nbJoueurs; ++i) {
		p.joueurs[i].type = toupper(param[i]);
		p.joueurs[i].score = 0;
	}

	p.tourActuel = 0;
	p.tailleMot = 2;
	
	
	p.motTap = new char[p.tailleMot + 1];
	p.motTap[0] = '\0';
	p.posLettre = 0;
	p.motTapVerif = new char[MAX];
	p.motTapVerif[0] = '\0';

	//Allocation du dico en m�moire
	initialiserDico(p);
}
/**
* @brief V�rifie si le mot tap� par le joueur est vide
* @param[in] p: La partie en cours
* @return Vrai si le mot tap� est vide, faux sinon
*/
bool estZeroLettre(Partie& p) {
	return (p.motTap[0] == '\0');
}
/**
* @brief V�rifie si le mot tap� par le joueur n'a qu'une seule lettre
* @param[in] p: La partie en cours
* @return Vrai si le mot tap� n'a qu'une seule lettre, faux sinon
*/
bool estPremiereLettre(Partie& p) {
	return (p.motTap[0] == '\0' || p.motTap[1] == '\0');
}
/**
* @brief V�rifie si le mot tap� par le joueur n'a que deux lettres ou qu'une lettre suivie d'un '?'
* @param[in] p: La partie en cours
* @return Vrai si le mot tap� n'a que deux lettres ou qu'une lettre suivie d'un '?', faux sinon
*/
bool estDeuxLettre(Partie& p) {
	return (p.motTap[0] == '\0' || p.motTap[1] == '\0' || p.motTap[2] == '\0' || p.motTap[2] == '?');
}
/**
* @brief V�rifie si un mot est valide (si il est pr�sent dans le dictionnaire de la partie)
* @param[in] p: La partie en cours
* @param[in] mot: Le mot � v�rifier
* @return Vrai si le mot est valide, faux sinon
*/
bool estMotValide(Partie& p, char* mot) {
	
	int min = 0;
	int max = p.d.nbMot - 1;
	int milieu;
	while (min <= max){
		milieu = (min + max) / 2;
		if (strcmp(p.d.mots[milieu], mot) == 0) return true;
		else if (strcmp(p.d.mots[milieu], mot) < 0) min = milieu + 1;
		else max = milieu - 1;
	}
	return false;
	
}
/**
* @brief Agrandi la taille de la cha�ne de caract�re du mot tap�
* @param[in,out] p: La partie en cours
*/
void agrandir(Partie& p) {

	unsigned int newMax = p.tailleMot + 1;
	
	char* newMot = new char[newMax+1];
	strcpy(newMot, p.motTap);
	
	delete[] p.motTap;
	p.motTap = new char[newMax + 1];
	strcpy(p.motTap, newMot);
	
	p.tailleMot = newMax;
	
	delete[] newMot;
	
}

/**
* @brief Ajoute une lettre au mot tap� par le joueur
* @param[in,out] p: La partie en cours
* @param[in] c: La lettre � ajouter
*/
void ajoutLettre(Partie& p, char c) {
	
	if (p.posLettre >= p.tailleMot) {
		agrandir(p);
	}
	
	p.motTap[p.posLettre] = toupper(c); // Majuscule
	p.motTap[p.posLettre + 1] = '\0';
	++p.posLettre;
}
/**
* @brief Demande � l'utilisateur de saisir une lettre et l'ajoute au mot tap� par le joueur
* @param[in,out] p: La partie en cours
*/
void saisiHumain(Partie& p) {
	char c;
	cin >> c;
	ajoutLettre(p, c);
	cin.ignore(INT_MAX, '\n');
}
/**
* @brief V�rifie si un caract�re est une voyelle
* @param[in] c: Le caract�re � v�rifier
* @return Vrai si c est une voyelle, faux sinon
*/
bool estVoyelle(const char c) {
	const char voyelle[6] = { 'a', 'e', 'i', 'o', 'u', 'y' };
	for (unsigned int i = 0; i < 6; ++i) {
		if (c == toupper(voyelle[i])) return true;
	}
	return false;
}
/**
* @brief V�rifie si un caract�re est une consonne
* @param[in] c: Le caract�re � v�rifier
* @return Vrai si c est une consonne, faux sinon
* @see estVoyelle
*/
bool estConsonne(const char c) {
	return !estVoyelle(c);
}
/**
* @brief V�rifie si le dernier caract�re tap� par le joueur est un point d'interrogation
* @param[in,out] p: La partie en cours
* @return Vrai si le dernier caract�re est un point d'interrogation, faux sinon
*/
bool ptInterrogation(Partie& p) {
	if (estZeroLettre(p)) {
		if (p.motTap[p.posLettre] == '?') return true;
		else return false;
	}
	else {
		if (p.motTap[p.posLettre - 1] == '?') return true;
		else return false;
	}
}
/**
* @brief Le robot tape un mot dans le cas o� le joueur a tap� un point d'interrogation
* @param[in,out] p: La partie en cours
*/
void casPtInterroR(Partie& p) {
	char c = '\0';
	if (estPremiereLettre(p)) {
		char motRobot[] = { "ABRUTI" };
		for (unsigned int i = 0; i < strlen(motRobot); ++i) {
			p.motTapVerif[i] = toupper(motRobot[i]);
		}
		p.motTapVerif[strlen(motRobot)] = '\0';
		cout << p.motTapVerif << endl;
	}
	else {
		
		// On regarde les premi�res lettres du mot tap�
		char* prefixe = new char[MAX];
		for (unsigned int i = 0; i < p.posLettre - 1; i++) {
			prefixe[i] = p.motTap[i];
		}
		prefixe[p.posLettre - 1] = '\0';
		unsigned int lenghPref = strlen(prefixe);

		// On compte le nombre de mots qui commencent par ce pr�fixe
		unsigned int nbMots = 0;
		for (unsigned int i = 0; i < p.d.nbMot; i++) {
			if (strncmp(p.d.mots[i], prefixe, lenghPref) == 0) {
				nbMots++;
			}
		}

		if (nbMots > 0) {
			unsigned int motChoisi = rand() % nbMots;
			unsigned int motCompte = 0;

			do {
				for (unsigned int i = 0; i < p.d.nbMot; i++) {
					if (strncmp(p.d.mots[i], prefixe, lenghPref) == 0) { // Trouve d'abord la partie du dico o� commence ce prefixe
						if (motCompte == motChoisi) { // V�rifie si c'est le mot choisi
							prefixe = p.d.mots[i];
							break;
						}
						motCompte++;
					}
				}

			} while (!estMotValide(p, prefixe));

			unsigned int longueur = strlen(prefixe);
			for (unsigned int i = 0; i < longueur; ++i) {
				p.motTapVerif[i] = prefixe[i];
			}
			p.motTapVerif[strlen(prefixe)] = '\0';
			cout << p.motTapVerif << endl;
		}
		else {
			c = '!';
			cout << c << endl;
			p.motTapVerif[0] = c;
			p.motTapVerif[1] = '\0';
		}

	}
}
/**
* @brief Cas du robot o� il tape une lettre
* @param[in,out] p: La partie en cours
*/
void casNormalSaisiR(Partie& p) {
	char c = '\0';
	if (estZeroLettre(p)) {
		char lettres[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		unsigned int index = rand() % 26;
		c = lettres[index];
		cout << c << endl;
		ajoutLettre(p, c);
	}
	else {

		unsigned int nbTest = 0;

		// On regarde les premi�res lettres du mot tap�
		char* prefixe = new char[MAX];
		for (unsigned int i = 0; i < p.posLettre; i++) {
			prefixe[i] = p.motTap[i];
		}
		prefixe[p.posLettre] = '\0';
		unsigned int lenghPref = strlen(prefixe);

		// On compte le nombre de mots qui commencent par ce pr�fixe
		unsigned int nbMots = 0;
		for (unsigned int i = 0; i < p.d.nbMot; i++) {
			if (strncmp(p.d.mots[i], prefixe, lenghPref) == 0) {
				nbMots++;
			}
		}

		// Si il y a au moins un mot qui commence par ce pr�fixe, on choisit une lettre al�atoirement parmi les mots qui commencent par ce pr�fixe
		if (nbMots > 0) {
			unsigned int motChoisi = rand() % nbMots;
			unsigned int motCompte = 0;

			do {
				for (unsigned int i = 0; i < p.d.nbMot; i++) {
					if (strncmp(p.d.mots[i], prefixe, lenghPref) == 0) { // Trouve d'abord la partie du dico o� commence ce prefixe
						if (motCompte == motChoisi) { // V�rifie si c'est le mot choisi
							c = p.d.mots[i][p.posLettre]; // R�cup�re la lettre suivante du mot choisi
							break;
						}
						motCompte++;
					}
				}


				prefixe[p.posLettre] = toupper(c); // Majuscule
				prefixe[p.posLettre + 1] = '\0';
				if (nbMots <= 3 && estMotValide(p, prefixe)) {
					c = '?';
					break;
				}
				else {
					motChoisi = rand() % nbMots;
					motCompte = 0;
				}

				if (estPremiereLettre(p)) break;
			
				++nbTest;

				if (nbMots > 2000) {
					if (nbTest > nbMots / 4) {
						if (estConsonne(prefixe[p.posLettre - 1])) {
							char voyelle[6] = { 'a', 'e', 'i', 'o', 'u', 'y' };
							unsigned int index = rand() % 6;
							c = voyelle[index];
							break;
						}
						else {
							char consonnes[21] = { 'b', 'c', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', 'n', 'p', 'q', 'r', 's', 't', 'v', 'w', 'x', 'y', 'z' };
							unsigned int index = rand() % 21;
							c = consonnes[index];
							break;
						}
						break;
					}
					
				}
				else {
					if (nbTest > nbMots / 2) {
						c = '?';
						break;
					}
				}
				
			} while (estMotValide(p, prefixe));

			cout << c << endl;
			ajoutLettre(p, c);
		}
		else {
			c = '?';
			cout << c << endl;
			ajoutLettre(p, c);
		}
		delete[] prefixe;
	}
}
/**
* @brief G�n�re une lettre al�atoire pour le robot
* @param[in,out] p: La partie en cours
*/
void saisiRobot(Partie& p) {
	char c = '\0';

	if (ptInterrogation(p)) {
		casPtInterroR(p);
	}
	else {
		casNormalSaisiR(p);
	}

}
/**
* @brief Fait jouer le joueur en cours
* @param[in,out] p: La partie en cours
*/
void jouer(Partie& p) {

	if (p.joueurs[p.tourActuel].type == 'H') {
		saisiHumain(p);
	}
	else {
		saisiRobot(p);
	}

}
/**
* @brief Affiche le mot tap� par le joueur courant dans la partie en cours
* @param[in] p: La partie en cours
*/
void motSaisi(Partie& p) {
	unsigned int longueur = strlen(p.motTap);
	for (unsigned int i = 0; i < longueur; ++i) {
		if (p.motTap[i] == 0) {
			break;
		}
		else {
			cout << p.motTap[i];
		}
	}
}
/**
* @brief Affiche l'�tat de la partie en cours
* @param[in,out] p: La partie en cours
*/
void afficher(Partie& p) {
	cout << p.tourActuel + 1 << p.joueurs[p.tourActuel].type << ", " << "(";
	motSaisi(p);
	cout << ")" << " > ";
}
/**
* @brief Fonction permettant de mettre � jour le score d'un joueur
* @param[in] p: La partie en cours
* @param[in] i: L'indice du joueur dont on veut mettre � jour le score
*/
void score(Partie& p, unsigned int i) {
	switch (p.joueurs[i].score) {
	case 1:
		cout << "0.25";
		break;
	case 2:
		cout << "0.5";
		break;
	case 3:
		cout << "0.75";
		break;
	case 4:
		cout << "1";
		break;
	default:
		cout << "0";
		break;
	}
}
/**
* @brief Affiche les scores des joueurs
* @param[in] p: La partie en cours
*/
void afficheScore(Partie& p) {
	for (unsigned int i = 0; i < p.nbJoueurs; ++i) {
		if (i == p.nbJoueurs - 1) {
			cout << i + 1 << p.joueurs[i].type << " : ";
			score(p, i);
			cout << endl;
		}
		else {
			cout << i + 1 << p.joueurs[i].type << " : ";
			score(p, i);
			cout << "; ";
		}
	}
}
/**
* @brief Ajoute un point au score du joueur en cours
* @param[in,out] p: La partie en cours
*/
void ajouteScore(Partie& p) {
	++p.joueurs[p.tourActuel].score;
}
/**
* @brief R�initialiser la manche en cours
* @param[in,out] p : La partie en cours
*/
void resetManche(Partie& p) {
	if (p.joueurs[p.tourActuel].score == 4);
	else {
		p.motTap[0] = '\0';
		p.motTapVerif[0] = '\0';
	}
	--p.tourActuel;
	p.posLettre = 0;
	return;
}
/**
* @brief Message si le mot tap� par le joueur existe dans le dictionnaire
* @param[in,out] p : La partie en cours
*/
void motExiste(Partie& p) {
	cout << "le mot "; motSaisi(p); cout << " existe, le joueur ";
	cout << p.tourActuel + 1 << p.joueurs[p.tourActuel].type << " prend un quart de singe" << endl;
	ajouteScore(p);
	afficheScore(p);
	resetManche(p);

}
/**
* @brief Message si le mot tap� par le joueur n'est pas le m�me que le mot tap�
* @param[in,out] p : La partie en cours
*/
void lettresDifferentes(Partie& p) {
	cout << "le mot " << p.motTapVerif << " ne commence pas par les lettres attendues, le joueur " << p.tourActuel << p.joueurs[p.tourActuel - 1].type << " prend un quart de singe" << endl;
	--p.tourActuel; // Recule d'un tour donc d'un joueur
	ajouteScore(p);
	afficheScore(p);
	resetManche(p);
}
/**
* @brief Message si le mot tap� par le joueur existe
* @param[in,out] p : La partie en cours
*/
void motExisteVerif(Partie& p) {
	cout << "le mot " << p.motTapVerif << " existe, le joueur ";
	if (p.tourActuel == p.nbJoueurs) {
		p.tourActuel = 0;
		cout << p.tourActuel + 1 << p.joueurs[p.tourActuel].type << " prend un quart de singe" << endl; // Cas particulier du dernier/premier joueur
	}
	else
		cout << p.tourActuel + 1 << p.joueurs[p.tourActuel].type << " prend un quart de singe" << endl;
	ajouteScore(p);
	afficheScore(p);
	resetManche(p);
}
/**
* @brief Message si le mot tap� par le joueur n'existe pas
* @param[in,out] p : La partie en cours
*/
void motExistePas(Partie& p) {
	cout << "le mot " << p.motTapVerif << " n'existe pas, " << p.tourActuel << p.joueurs[p.tourActuel - 1].type << " prend un quart de singe" << endl;
	if (p.tourActuel == 0) {
		p.tourActuel = p.nbJoueurs;
	}
	else --p.tourActuel; // Recule d'un tour donc d'un joueur
	ajouteScore(p);
	afficheScore(p);
	resetManche(p);
}
/**
* @brief Saisir un mot pour verifier s'il existe
* @param[in,out] p : La partie en cours
*/
void saisirMotTapVerif(Partie& p) {

	if (p.joueurs[p.tourActuel - 1].type == 'H') {
		cin >> setw(MAX) >> p.motTapVerif;
		cin.ignore(INT_MAX, '\n');
		unsigned int longueur = strlen(p.motTapVerif);
		for (unsigned int i = 0; i < longueur; ++i) {
			p.motTapVerif[i] = toupper(p.motTapVerif[i]); // Majuscule
		}
	}
	else {
		saisiRobot(p);
	}

}
/**
* @brief Message si le joueur tape un point d'exclamation
* @param[in,out] p : La partie en cours
*/
void exclamation(Partie& p) {
	cout << "le joueur " << p.tourActuel + 1 << p.joueurs[p.tourActuel].type << " abandonne la manche et prend un quart de singe" << endl;
	ajouteScore(p);
	afficheScore(p);
	resetManche(p);
}
/**
* @brief Verifie si le caract�re tap� est un point d'exclamation
* @param[in] p : La partie en cours
* @return Vrai si le dernier caract�re est un point d'exclamation, faux sinon
*/
bool ptExclamation(Partie& p) {
	if (p.posLettre == 0) return false;
	if (p.motTap[p.posLettre - 1] == '!') return true;
	else return false;
}
/**
* @brief Verifie si les caract�res tap�s sont les m�mes que le mot tap�
* @param[in] p : La partie en cours
* @return retourne un bool�en
*/
bool verifLettres(Partie& p) {
	unsigned int longueur = strlen(p.motTap);
	for (unsigned int i = 0; i < longueur -1; ++i) {
		if (p.motTapVerif[i] != p.motTap[i])
			return false;
	}
	return true;
}
/**
* @brief Cas o� c'est un point d'interrogation, on fait les v�rifications
* @param[in,out] p : La partie en cours
*/
void verifInterrogation(Partie& p) {
	if (p.tourActuel == 0) { // Cas particulier du dernier/premier joueur
		p.tourActuel = p.nbJoueurs;
	}
	cout << p.tourActuel << p.joueurs[p.tourActuel - 1].type << ", saisir le mot > ";
	saisirMotTapVerif(p);

	if (!verifLettres(p)) {
		lettresDifferentes(p);
	}
	else if (!estDeuxLettre(p)) {
		if (estMotValide(p, p.motTapVerif)) {
			motExisteVerif(p);
		}
		else {
			motExistePas(p);
		}
	}
	else if (estPremiereLettre(p)) {
		if (estMotValide(p, p.motTapVerif)) {
			motExisteVerif(p);
		}
		else {
			motExistePas(p);
		}
	}
	else {
		motExistePas(p);
	}
}
/**
* @brief Cas o� c'est un point d'exclamation, on fait les v�rifications
* @param[in,out] p : La partie en cours
*/
void verifExclamation(Partie& p) {
	exclamation(p);
}
/**
* @brief Cas normal, on regarde si le mot existe
* @param[in,out] p : La partie en cours
*/
void verifNormal(Partie& p) {
	if (!estMotValide(p, p.motTap)) { // V�rification de la validit� du mot
		return;
	}
	else { // Si le mot existe dans le dictionnaire
		motExiste(p);
		return;
	}
}
/**
* @brief Fonction reunissant toutes les verifications
* @param[in,out] p : La partie en cours
*/
void verification(Partie& p) {
	if (ptInterrogation(p)) { // Si le joueur a tap� ?
		if (estPremiereLettre(p)) {
			cout << "Aucun mot n'a �t� saisi, ";
			cout << p.tourActuel + 1 << p.joueurs[p.tourActuel].type << " prend un quart de singe" << endl;
			ajouteScore(p);
			afficheScore(p);
			resetManche(p);
		}
		else {
			verifInterrogation(p); // Verification des cas possibles
		}
		
	}
	else if (ptExclamation(p)) { // Si le joueur a tap� !
		verifExclamation(p); // Verification des cas possibles
	}
	else {
		if (!estDeuxLettre(p)) {
			verifNormal(p); //Sinon verifications normales
		}
		else; //Si c'est la premi�re lettre, on ne fait rien
	}
}
/**
* @brief Le jeu
* @param[in, out] p : La partie en cours
*/
void jouerPartie(Partie& p) {
	while (p.joueurs[p.tourActuel].score != 4) {

		afficher(p);
		jouer(p);
		verification(p);


		if (p.tourActuel == p.nbJoueurs - 1) p.tourActuel = 0;
		else ++p.tourActuel;
	}
	cout << "La partie est finie" << endl;
}
/**
* @brief D�truit une partie et lib�re les ressources associ�es
* @param[in,out] p: La partie � d�truire
*/
void detruirePartie(Partie& p) {
	delete[] p.joueurs;
	p.joueurs = nullptr;
	delete[] p.motTap;
	p.motTap = nullptr;

	for (unsigned int i = 0; i < p.d.nbMot; ++i) {
		delete[] p.d.mots[i];
	}
	delete[] p.d.mots;

	p.d.nbMot = NULL;
	p.d.mots = nullptr;

	delete[] p.motTapVerif;
	p.motTapVerif = nullptr;
}