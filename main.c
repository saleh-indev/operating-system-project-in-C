#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAILLE_MAX 100

typedef struct  Processus Processus;
typedef struct Result Result;
struct Processus{
    int dateArrive,dateFin;
    int tempsExec;
    int tempsRest;
    int tempsAtt;
    int tempsRep;
    int dureeES;
};
struct Result{
    float tempsAttMoy;
    float tempsRestMoy;
    float tempsRepMoy;
    float tauxOccCPU;
};

//Prototypes
/**
 * @brief Menu permettant de choisir l'ALGORITHME d'ordonnancement
 * @return int
*/
int menuPrincipal();

/**
 * @brief Initialise les données
 * 
 * @param p 
 * @param n 
 */
void initData(Processus p[TAILLE_MAX], int n);

/**
 * @brief Lit les données a partir d'un fichier
 * Format reste a definir (voir documentation)
 * @param file 
 * @param p 
 * @return int 
 */
int lireFichier(FILE* file,Processus p[TAILLE_MAX]);

/**
 * @brief Permet de trier les processus par ordre d'arrivee
 * Utile pour FIFO
 * @param process 
 * @param n 
 */
void trierTempsArrive(Processus process[TAILLE_MAX], int n);

/**
 * @brief Permet de trier les processus par temps restant
 * Utile pour SJF
 * @param process 
 * @param n 
 */
void trierTempsRestant(Processus process[TAILLE_MAX], int n);

/**
 * @brief Fonction qui gere le tour de round R
 * 
 * @param process 
 * @param n 
 * @return int 
 */
int exist(Processus process[TAILLE_MAX], int n);

/**
 * @brief Fonction de verification utilisée dans RR
 * 
 * @param index 
 * @param t 
 * @param i 
 * @return int 
 */
int notin(int index[TAILLE_MAX],int t,int i);

/**
 * @brief Calcul le resultat de l'ordonnant
 * 
 * @param process 
 * @param n 
 * @param temps
 * @return Result 
 */
Result calculResultat(Processus process[TAILLE_MAX], int n,int temps);

/**
 * @brief Sauvegarde les resultats d'ordonnancement
 * @param Result
 * @param process 
 * @param n
 */
void enregister(Result resultat,Processus process[TAILLE_MAX], int n);


int main(int argc, char *argv[]){
    FILE* fichierTest = NULL;
    if(argc >= 2){
        //En cas de lecture 
        fichierTest = fopen(strcat(argv[1],".txt"), "r");
        if(fichierTest== NULL) {
            printf ("Impossible d'ouvrir le fichier : %s\n", argv[1]);
            exit(1);
        }
    }    
    //Lecture des processus par console lecture
    Processus processus[TAILLE_MAX];
    int nombre;
    if(argc < 2){
        //Si on donne les processus par la console
        printf("Donnez le nombre de processus : ");
        scanf("%d",&nombre);
        if(nombre<=0) exit(0);
        initData(processus,nombre);
        for(int i=0;i<nombre;i++){
            printf("Processus %d: \n",i+1);
            printf("Donnez le temps d'arrive : ");
            scanf("%d",&processus[i].dateArrive);
            printf("Donnez le temps d'execution CPU: ");
            scanf("%d",&processus[i].tempsExec);
            printf("Donnez le temps d'E/S: ");
            scanf("%d",&processus[i].dureeES);
        }
    }
    else
        nombre = lireFichier(fichierTest,processus);
    
    //initialisation data
    Result resultat;
    resultat.tauxOccCPU = 0.0;
    resultat.tempsAttMoy = 0;
    resultat.tempsRepMoy = 0;
    resultat.tempsRestMoy = 0;
    int quantum=100;
    int temps = 0;

    //Choix de l'algorithme
    switch (menuPrincipal()){
        case 1:
            //FIFO
            trierTempsArrive(processus,nombre);
            for (int i = 0; i < nombre; i++){
                if(i==0){
                    //Cas du premier processus
                    processus[i].dateFin = processus[i].dateArrive+ processus[i].tempsExec+ processus[i].dureeES;
                    temps = processus[i].tempsExec;
                    processus[i].tempsRep  = processus[i].dateArrive;
                    processus[i].tempsRest = processus[i].dateFin - processus[i].dateArrive;
                    processus[i].tempsAtt = processus[i].tempsRest - processus[i].tempsExec;
                } 
                else{
                    temps = processus[i-1].dateFin;
                    if(processus[i].dateArrive<=temps)
                        processus[i].tempsRep = temps;
                    else {
                        processus[i].tempsRep = processus[i].dateArrive;
                        temps= processus[i].dateArrive;
                    }
                    processus[i].dateFin = temps+ processus[i].tempsExec+processus[i].dureeES;
                    processus[i].tempsRest = processus[i].dateFin - processus[i].dateArrive;
                    processus[i].tempsAtt = processus[i].tempsRest - processus[i].tempsExec;
                }
            }
            temps= processus[nombre-1].dateFin;
        break;
        case 2:
            //SJF
            trierTempsRestant(processus,nombre);
            for (int i = 0; i < nombre; i++){
                if(i==0){
                    processus[i].dateFin = processus[i].dateArrive+ processus[i].tempsExec+ processus[i].dureeES;
                    temps = processus[i].tempsExec;
                    processus[i].tempsRep  = processus[i].dateArrive;
                    processus[i].tempsRest = processus[i].dateFin - processus[i].dateArrive;
                    processus[i].tempsAtt = processus[i].tempsRest - processus[i].tempsExec;
                } else{
                    temps = processus[i-1].dateFin;
                    if(processus[i].dateArrive<=temps)
                        processus[i].tempsRep = temps;
                    else {
                        processus[i].tempsRep = processus[i].dateArrive;
                        temps= processus[i].dateArrive;
                    }
                    processus[i].dateFin = temps+ processus[i].tempsExec+processus[i].dureeES;
                    processus[i].tempsRest = processus[i].dateFin - processus[i].dateArrive;
                    processus[i].tempsAtt = processus[i].tempsRest - processus[i].tempsExec;
                }
            }
            temps= processus[nombre-1].dateFin;
        break;
        default: 
            //RR
            quantum = 100; //par convention (.01 s)
            trierTempsArrive(processus,nombre);
            int index[TAILLE_MAX], t=0;
            int tr[TAILLE_MAX];
            int tex[TAILLE_MAX];
            for(int i = 0; i < nombre; i++){
                tex[i]=processus[i].tempsExec;
            }
            while(exist(processus,nombre)==1){
                for (int i = 0; i < nombre && notin(index,t,i); i++){
                    if(processus[i].tempsExec>quantum){
                        processus[i].tempsExec-=quantum;
                        processus[i].dateFin += quantum*nombre;
                    }
                    else{
                        processus[i].dateFin += processus[i].tempsExec;
                        tr[i]= temps+ processus[i].tempsExec;
                        index[t]=i;
                        t++;
                        processus[i].tempsExec=0;
                    }
                }
                temps+=quantum;
            }
            for(int i = 0; i < nombre; i++){
                processus[i].tempsExec=tex[i];
            }
            for(int i = 0; i < nombre; i++){
                processus[i].tempsRep = tr[i];
                processus[i].dateFin+=processus[i].dureeES;
                processus[i].tempsRest = processus[i].dateFin - processus[i].dateArrive;
                processus[i].tempsAtt = processus[i].tempsRest - processus[i].tempsExec;
                if( processus[i].tempsRest <=0)  processus[i].tempsRest =0;
                if( processus[i].tempsAtt<=0)  processus[i].tempsAtt=0;
            }
        break;
    }
    resultat = calculResultat(processus,nombre,temps);
    enregister(resultat, processus,nombre);
    printf("Fin de la simulation !\n");
    return 0;
}

//IMPLEMENTATION 
int menuPrincipal(){
    int choix=0;
    printf("\n\n\t\t\tLES POLITIQUES D'ORDONNANCEMENT\n\t\t\t\t\t     -------------\n\n");
    printf("\t\t\t1- FIFO\n\t\t\t2- SJF\n\t\t\t3- RR\n");
    do{
        printf("\t\t\t\tChoisir un algorithme: ");
        scanf("%d",&choix);
        if(choix<1 || choix>3)
            printf("\t\t\t\t -- OPERATION NON DISPOSIBLE... Reessayez !-- \n\n");
    }while(choix<1 || choix>3);
    return choix;
}
void initData(Processus p[TAILLE_MAX], int n){
    for(int i= 0; i < n; i++){
        p[i].dateArrive = 0;
        p[i].dateFin = 0;
        p[i].dureeES = 0;
        p[i].tempsAtt = 0;
        p[i].tempsExec = 0;
        p[i].tempsRep = 0;
        p[i].tempsRest = 0;
    }
}
void trierTempsArrive(Processus process[TAILLE_MAX], int n){
    int temp;
    for(int i= 0; i<n-1; i++){
        for(int j=i+1; j<n;j++){
            if(process[i].dateArrive>process[j].dateArrive){
                temp = process[i].dateArrive;
                process[i].dateArrive = process[j].dateArrive;
                process[j].dateArrive = temp;
            }
        }
    }
}
void trierTempsRestant(Processus process[TAILLE_MAX], int n){
    int temp;
    for(int i= 0; i<n-1; i++){
        for(int j=i+1; j<n;j++){
            if(process[i].tempsExec>process[j].tempsExec){
                temp = process[i].tempsExec;
                process[i].tempsExec = process[j].tempsExec;
                process[j].tempsExec = temp;
            }
        }
    }
}
int exist(Processus process[TAILLE_MAX], int n){
    for(int i=0; i< n; i++){
        if(process[i].tempsExec>0) return 1;
    }
    return 0;
}
int notin(int index[TAILLE_MAX],int t,int j){
    for(int i=0;i<t; i++){
        if(index[i]==j) return 0;
    }
    return 1;
}
Result calculResultat(Processus process[TAILLE_MAX], int n,int temps){
    Result r;
    r.tauxOccCPU=0.0;
    r.tempsAttMoy=0;
    r.tempsRepMoy= 0;
    r.tempsRestMoy= 0;
    for(int i= 0; i < n; i++){
        r.tempsAttMoy+=process[i].tempsAtt;
        r.tempsRestMoy+=process[i].tempsRest;
        r.tempsRepMoy+=process[i].tempsRep;
        r.tauxOccCPU+=process[i].tempsExec;
    }
    r.tempsAttMoy = r.tempsAttMoy/n;
    r.tempsRestMoy = r.tempsRestMoy/n;
    r.tempsRepMoy = r.tempsRepMoy/n;
    r.tauxOccCPU = r.tauxOccCPU/temps;
    return r;
}
int lireFichier(FILE* file,Processus p[TAILLE_MAX]){
    int nombre=0;
    if(file == NULL) {
		exit (1);
	}
    while(!feof(file)){
        fscanf(file, "%d\t%d\t%d\n",&p[nombre].dateArrive,&p[nombre].tempsExec,&p[nombre].dureeES);
        nombre++;
    }
    return nombre;
}
void enregister(Result resultat,Processus process[TAILLE_MAX], int n){
    FILE* f = fopen("resultat.xsl", "w");
	if(f == NULL) {
		exit (1);
	}
    fprintf(f, "CPU\tE/S\tAttente\tReponse\tRestitution\n");
    for(int i= 0; i < n; i++){
        fprintf(f, "%d\t%d\t%d\t%d\t%d\n",process[i].tempsExec,
        process[i].dureeES,
        process[i].tempsAtt,
        process[i].tempsRep,
        process[i].tempsRest     
        );
    }
    fprintf(f, "\nTAM\tTRepM\tTResM\tTaux\n");
    fprintf(f, "%.2f\t%.2f\t%.2f\t%.2f\n",resultat.tempsAttMoy,resultat.tempsRepMoy,resultat.tempsRestMoy,resultat.tauxOccCPU*100);
    fclose(f);
}