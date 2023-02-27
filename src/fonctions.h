#pragma once

#ifndef _FONCTIONS_
#define _FONCTIONS_

/**
 * @file fonctions.h
 * @brief Ent�te du composant de fonctions
 */

 /**
 * @brief Les constantes
 */
enum {
	MIN_JOUEURS = 2,
	MAX = 28,
};

/**
* @brief Structure de donn�es de type Dico
*/
struct Dico {
	char** mots;
	unsigned int nbMot;
};
/**
* @brief Structure de donn�es de type Joueur
*/
struct Joueur {
	char type; // H ou R
	int score;
};
/**
* @brief Structure de donn�es de type Partie
*/
struct Partie {
	Joueur* joueurs;
	unsigned int nbJoueurs;
	char* motTap; // Lettres annonc�es pendant la partie
	unsigned int tailleMot;
	unsigned int posLettre; // Position o� on doit ins�rer la lettre dans le mot
	unsigned int tourActuel; // Indice du joueur dans le tableau joueurs
	char* motTapVerif;
	Dico d;
};

/**
* @brief Verification du nombre de joueur
* @param[in] argv : L'argument utilis� pour compter le nombre de joueur
* @return retourne un bool�an
* @pre argv valide
*/
bool verifNbJoueur(const char* argv[]);
/**
* @brief V�rifie le type de joueur
* @param[in] argv : L'argument utilis� pour v�rifier le type de joueur
* @return Retourne un bool�en
* @pre argv doit �tre valide
*/
bool verifJoueur(const char* argv[]);
/**
* @brief Charge le dictionnaire de mots
* @param[in,out] p: La partie � laquelle le dictionnaire est associ�
* @pre p est valide
*/
void initialiserDico(Partie& p);
/**
* @brief Initialise une partie
* @param[in,out] p: La partie � initialiser
* @param[in] argv : Les arguments de la commande utilis�s pour initialiser la partie
* @pre argv doit �tre valide
* @see initialiserDico
*/
void initialiserPartie(Partie& p, const char* argv[]);
/**
* @brief V�rifie si le mot tap� par le joueur est vide
* @param[in] p: La partie en cours
* @return Vrai si le mot tap� est vide, faux sinon
*/
bool estZeroLettre(Partie& p);
/**
* @brief V�rifie si le mot tap� par le joueur n'a qu'une seule lettre
* @param[in] p: La partie en cours
* @return Vrai si le mot tap� n'a qu'une seule lettre, faux sinon
*/
bool estPremiereLettre(Partie& p);
/**
* @brief V�rifie si le mot tap� par le joueur n'a que deux lettres ou qu'une lettre suivie d'un '?'
* @param[in] p: La partie en cours
* @return Vrai si le mot tap� n'a que deux lettres ou qu'une lettre suivie d'un '?', faux sinon
*/
bool estDeuxLettre(Partie& p);
/**
* @brief V�rifie si un mot est valide (si il est pr�sent dans le dictionnaire de la partie)
* @param[in] p: La partie en cours
* @param[in] mot: Le mot � v�rifier
* @return Vrai si le mot est valide, faux sinon
*/
bool estMotValide(Partie& p, char* mot);
/**
* @brief Agrandi la taille de la cha�ne de caract�re du mot tap�
* @param[in,out] p: La partie en cours
*/
void agrandir(Partie& p);
/**
* @brief Ajoute une lettre au mot tap� par le joueur
* @param[in,out] p: La partie en cours
* @param[in] c: La lettre � ajouter
*/
void ajoutLettre(Partie& p, char c);
/**
* @brief Demande � l'utilisateur de saisir une lettre et l'ajoute au mot tap� par le joueur
* @param[in,out] p: La partie en cours
*/
void saisiHumain(Partie& p);
/**
* @brief V�rifie si un caract�re est une voyelle
* @param[in] c: Le caract�re � v�rifier
* @return Vrai si c est une voyelle, faux sinon
*/
bool estVoyelle(const char c);
/**
* @brief V�rifie si un caract�re est une consonne
* @param[in] c: Le caract�re � v�rifier
* @return Vrai si c est une consonne, faux sinon
* @see estVoyelle
*/
bool estConsonne(const char c);
/**
* @brief V�rifie si le dernier caract�re tap� par le joueur est un point d'interrogation
* @param[in,out] p: La partie en cours
* @return Vrai si le dernier caract�re est un point d'interrogation, faux sinon
*/
bool ptInterrogation(Partie& p);
/**
* @brief Le robot tape un mot dans le cas o� l'humain a tap� un point d'interrogation
* @param[in,out] p: La partie en cours
*/
void casPtInterroR(Partie& p);
/**
* @brief Cas du robot o� il tape une lettre
* @param[in,out] p: La partie en cours
*/
void casNormalSaisiR(Partie& p);
/**
* @brief G�n�re une lettre al�atoire pour le robot
* @param[in,out] p: La partie en cours
*/
void saisiRobot(Partie& p);
/**
* @brief Fait jouer le joueur en cours
* @param[in,out] p: La partie en cours
*/
void jouer(Partie& p);
/**
* @brief Affiche le mot tap� par le joueur courant dans la partie en cours
* @param[in] p: La partie en cours
*/
void motSaisi(Partie& p);
/**
* @brief Affiche l'�tat de la partie en cours
* @param[in,out] p: La partie en cours
*/
void afficher(Partie& p);
/**
* @brief Fonction permettant de mettre � jour le score d'un joueur
* @param[in] p: La partie en cours
* @param[in] i: L'indice du joueur dont on veut mettre � jour le score
*/
void score(Partie& p, unsigned int i);
/**
* @brief Affiche les scores des joueurs
* @param[in] p: La partie en cours
*/
void afficheScore(Partie& p);
/**
* @brief Ajoute un point au score du joueur en cours
* @param[in,out] p: La partie en cours
*/
void ajouteScore(Partie& p);
/**
* @brief R�initialiser la manche en cours
* @param[in,out] p : La partie en cours
*/
void resetManche(Partie& p);
/**
* @brief Message si le mot tap� par le joueur existe dans le dictionnaire
* @param[in,out] p : La partie en cours
*/
void motExiste(Partie& p);
/**
* @brief Message si le mot tap� par le joueur n'est pas le m�me que le mot tap�
* @param[in,out] p : La partie en cours
*/
void lettresDifferentes(Partie& p);
/**
* @brief Message si le mot tap� par le joueur existe
* @param[in,out] p : La partie en cours
*/
void motExisteVerif(Partie& p);
/**
* @brief Message si le mot tap� par le joueur n'existe pas
* @param[in,out] p : La partie en cours
*/
void motExistePas(Partie& p);
/**
* @brief Saisir un mot pour verifier s'il existe
* @param[in,out] p : La partie en cours
*/
void saisirMotTapVerif(Partie& p);
/**
* @brief Message si le joueur tape un point d'exclamation
* @param[in,out] p : La partie en cours
*/
void exclamation(Partie& p);
/**
* @brief Verifie si le caract�re tap� est un point d'exclamation
* @param[in] p : La partie en cours
* @return Vrai si le dernier caract�re est un point d'exclamation, faux sinon
*/
bool ptExclamation(Partie& p);
/**
* @brief Verifie si les caract�re tap� sont les m�mes que le mot tap�
* @param[in] p : La partie en cours
* @return retourne un bool�en
*/
bool verifLettres(Partie& p);
/**
* @brief Cas o� c'est un point d'interrogation, on fais les v�rifications
* @param[in,out] p : La partie en cours
*/
void verifInterrogation(Partie& p);
/**
* @brief Cas o� c'est un point d'exclamation, on fait les v�rifications
* @param[in,out] p : La partie en cours
*/
void verifExclamation(Partie& p);
/**
* @brief Cas normal, on regarde si le mot existe
* @param[in,out] p : La partie en cours
*/
void verifNormal(Partie& p);
/**
* @brief Fonction reunissant toutes les verifications
* @param[in,out] p : La partie en cours
*/
void verification(Partie& p);
/**
* @brief Le jeu
* @param[in, out] p : La partie en cours
*/
void jouerPartie(Partie& p);
/**
* @brief D�truit une partie et lib�re les ressources associ�es
* @param[in,out] p: La partie � d�truire
*/
void detruirePartie(Partie& p);


#endif // !_FONCTIONS_