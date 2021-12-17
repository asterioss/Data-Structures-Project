#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Darwin.h"

#define BUFFER_SIZE 1024  /* Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


/**
 * @brief Optional function to initialize data structures that 
 *        need initialization
 *
 * @return 1 on success
 *         0 on failure
 */
int initialize (void)
{
    Species_head=NULL;
    Species_tail=NULL;
    hs_L=NULL;
    continents[0]=NULL;
    continents[1]=NULL;
    continents[2]=NULL;
    continents[3]=NULL;
    continents[4]=NULL;

	return 1;
}

/**
 * @brief insert new species in Species' list with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_species (int sid)
{
	struct Species* new = (struct Species*) malloc(sizeof(struct Species));
    struct Species* node;
    new->sid=sid;
    new->next=NULL;
    new->Population_head=NULL;
    new->Population_tail=NULL;
    
    if(Species_head==NULL) {
        new->prev=NULL;
        Species_head=new;
        Species_tail=Species_head;
    }
    else {
        node=Species_head;
        while(node->next!=NULL) {
            node=node->next;
        }
        node->next=new;
        new->prev=node;
        Species_tail=node->next;
    }

    node=Species_head;
    struct Species* curr;
    while(node->next!=NULL) {
        curr=node->next;
        while(curr!=NULL) {
            if(curr->sid<node->sid) {
                int temp = node->sid;  
                node->sid = curr->sid;  
                curr->sid = temp;
                if(node->prev==NULL) Species_head=node;
            }
            curr=curr->next;
        }
        node=node->next;
    }

    node=Species_head;
    printf("S<%d>", sid);
    while(node!=NULL) {
        if(node->next==NULL) Species_tail=node;
        struct Population* curr;
        curr=node->Population_head;
        printf("\n <%d>", node->sid);
        if(curr!=NULL) printf("\n  ");
        while(curr!=NULL) {
            printf("<%d>", curr->gid);
            curr=curr->next;
        }
        node=node->next;
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief insert new population with ID <gid> in Species' with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_population(int gid, int sid, int cid){
	struct Population* new = (struct Population*) malloc(sizeof(struct Population));
    struct Species* node;
    new->gid=gid;
    new->sid=sid;
    new->cid=cid;
    new->next=NULL;
    new->next_species=NULL;

    if(Species_head==NULL) {
        return 0;
    }
    else {
        node=Species_head;
        while(node!=NULL) {
            if(node->sid==sid) {
                struct Population* curr;
                if(node->Population_head==NULL) {
                    node->Population_head=new;
                    node->Population_tail=node->Population_head;
                }
                else {
                    curr=node->Population_head;
                    while(curr->next!=NULL) {
                        curr=curr->next;
                    }
                    curr->next=new;
                    node->Population_tail=curr->next;
                }

                curr=node->Population_head;
                struct Population* ekei;
                while(curr->next!=NULL) {
                    ekei=curr->next;
                    while(ekei!=NULL) {
                        if(ekei->gid<curr->gid) {
                            int temp = curr->gid;  
                            curr->gid = ekei->gid;  
                            ekei->gid = temp;
                            int temp1 = curr->sid;  
                            curr->sid = ekei->sid;  
                            ekei->sid = temp1;
                            int temp2 = curr->cid;  
                            curr->cid = ekei->cid;  
                            ekei->cid = temp2;
                        }
                        ekei=ekei->next;
                    }
                    curr=curr->next;
                }
            }
            node=node->next;
        }
    }

    node=Species_head;
    printf("G<%d><%d><%d>", gid, sid, cid);
    while(node!=NULL) {
        struct Population* curr;
        curr=node->Population_head;
        printf("\n <%d>", node->sid);
        if(curr!=NULL) printf("\n  ");
        while(curr!=NULL) {
            printf("<%d,%d>", curr->gid, curr->cid);
            curr=curr->next;
        }
        node=node->next;
    } 
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief merge species with IDs <sid1> and <sid2> into new species with ID <sid3>
 *
 * @return 1 on success
 *         0 on failure
 */
int merge_species(int sid1, int sid2, int sid3){
	struct Species* node;
    if(Species_head==NULL) {
        return 0;
    }
    else {
        insert_species(sid3);
        struct Species* ekei;
        struct Species* skr;
        node=Species_head;
        while(node!=NULL) {
            if(node->sid==sid1) ekei=node;
            if(node->sid==sid2) skr=node;
            node=node->next;
        }
        node=Species_head;
        while(node!=NULL) {
            if(node->sid==sid3) break;
            node=node->next;
        }
        struct Population* gang = (struct Population*) malloc(sizeof(struct Population));
        struct Population* new = (struct Population*) malloc(sizeof(struct Population));
        struct Population* curr;
        if(ekei->Population_head!=NULL) {
            curr=ekei->Population_head;
            while(curr!=NULL) {
                if(node->Population_head==NULL) {
                    new->gid=curr->gid;
                    new->sid=curr->sid;
                    new->cid=curr->cid;
                    new->next=NULL;
                    new->next_species=NULL;
                    node->Population_head=new;
                    gang=node->Population_head;
                    node->Population_tail=new;
                }
                else { 
                    gang->next= (struct Population*) malloc(sizeof(struct Population));
                    gang->next->gid=curr->gid;
                    gang->next->sid=curr->sid;
                    gang->next->cid=curr->cid;
                    gang->next->next=NULL;
                    gang->next->next_species=NULL;
                    node->Population_tail=gang->next;
                    gang=gang->next;   
                }
                curr=curr->next;
            }

            struct Species* del = ekei;
            if(Species_head==del) {
                Species_head=ekei->next;
                Species_head->prev=NULL;
            }
            else {
                if (del->next != NULL) {
                    del->next->prev = del->prev;
                    del->prev->next = del->next;
                }
                else {
                    del->prev->next=NULL;
                }
            }
        }
        if(skr->Population_head!=NULL) {
            curr=skr->Population_head;
            while(curr!=NULL) {
                if(node->Population_head==NULL) {
                    new->gid=curr->gid;
                    new->sid=curr->sid;
                    new->cid=curr->cid;
                    new->next=NULL;
                    new->next_species=NULL;
                    node->Population_head=new;
                    node->Population_tail=new;
                    gang=node->Population_head;
                }
                else {
                    gang->next= (struct Population*) malloc(sizeof(struct Population));
                    gang->next->gid=curr->gid;
                    gang->next->sid=curr->sid;
                    gang->next->cid=curr->cid;
                    gang->next->next=NULL;
                    gang->next->next_species=NULL;
                    node->Population_tail=gang->next;
                    gang=gang->next;  
                }
                curr=curr->next;
            }

            struct Species* del = skr;
            if(Species_head==del) {
                Species_head=skr->next;
                Species_head->prev=NULL;
            }
            else {
                if (del->next != NULL) {
                    del->next->prev = del->prev;
                    del->prev->next = del->next;
                }
                else {
                    del->prev->next=NULL;
                }
            }
        }

        curr=node->Population_head;
        struct Population* curr1;
        while(curr->next!=NULL) {
            curr1=curr->next;
            while(curr1!=NULL) {
                if(curr1->gid<curr->gid) {
                    int temp = curr->gid;  
                    curr->gid = curr1->gid;  
                    curr1->gid = temp;
                    int temp1 = curr->sid;  
                    curr->sid = curr1->sid;  
                    curr1->sid = temp1;
                    int temp2 = curr->cid;  
                    curr->cid = curr1->cid;  
                    curr1->cid = temp2;
                }
                curr1=curr1->next;
            }
            curr=curr->next;
        }
    }

    node=Species_head;
    printf("M<%d><%d><%d>", sid1, sid2, sid3);
    while(node!=NULL) {
        struct Population* curr;
        curr=node->Population_head;
        printf("\n <%d>", node->sid);
        if(curr!=NULL) printf("\n  ");
        while(curr!=NULL) {
            printf("<%d>", curr->gid);
            curr=curr->next;
        }
        node=node->next;
    } 
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Distribute species' population in continents array
 *
 * @return 1 on success
 *         0 on failure
 */
int distribute(){
	struct Species* node;
    if(Species_head==NULL) {
        return 0;
    }
    else {
        node=Species_head;
        int i;
        struct ContinentPopulation* skere[5];
        while(node!=NULL) {
            struct Population* curr;
            if(node->Population_head!=NULL) {
                curr=node->Population_head;
                while(curr!=NULL) {
                    if(curr->cid==1) {
                        i=0;
                        if(continents[i]==NULL) {
                            continents[i]=(struct ContinentList*) malloc(sizeof(struct ContinentList));
                            continents[i]->populationList = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                            continents[i]->populationList=NULL;
                        }
                    }
                    else if(curr->cid==2) {
                        i=1;
                        if(continents[i]==NULL) {
                            continents[i]=(struct ContinentList*) malloc(sizeof(struct ContinentList));
                            continents[i]->populationList = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                            continents[i]->populationList=NULL;
                        }
                    }
                    else if(curr->cid==3) {
                        i=2;
                        if(continents[i]==NULL) {
                            continents[i]=(struct ContinentList*) malloc(sizeof(struct ContinentList));
                            continents[i]->populationList = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                            continents[i]->populationList=NULL;
                        }
                    }
                    else if(curr->cid==4) {
                        i=3;
                        if(continents[i]==NULL) {
                            continents[i]=(struct ContinentList*) malloc(sizeof(struct ContinentList));
                            continents[i]->populationList = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                            continents[i]->populationList=NULL;
                        }
                    }
                    else if(curr->cid==5) {
                        i=4;
                        if(continents[i]==NULL) {
                            continents[i]=(struct ContinentList*) malloc(sizeof(struct ContinentList));
                            continents[i]->populationList = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                            continents[i]->populationList=NULL;
                        }
                    }

                    if(continents[i]->populationList==NULL) {
                        struct ContinentPopulation* new=(struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                        continents[i]->sentinel=(struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                        continents[i]->sentinel->next=NULL;
                        continents[i]->sentinel->gid=-1;
                        new->gid=curr->gid;
                        new->next=continents[i]->sentinel;
                        continents[i]->populationList=new;
                        skere[i]=(struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                        skere[i]=continents[i]->populationList;
                    }
                    else {
                        skere[i]->next = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
                        skere[i]->next->gid=curr->gid;
                        skere[i]->next->next=continents[i]->sentinel;
                        skere[i]=skere[i]->next;
                    }
                    curr=curr->next;
                }
            }
            node=node->next;
        }
    }

    int j;
    printf("D\n");
    struct ContinentList* skr;
    struct ContinentPopulation* skere;
    for(j=0; j<5; j++) {
        skr=continents[j];
        printf("Continent %d: ", j);
        if(skr!=NULL) {
            skere=skr->populationList;
            while(skere!=skr->sentinel) {
                printf("<%d>", skere->gid);
                skere=skere->next;
            }
        }
        printf("\n");
    } 
    printf("DONE\n");
	return 1;
}

/**
 * @brief delete population with ID <gid> from species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_population(int gid, int sid){
	struct Species* node;
    struct Species* ekei;
    struct Population *skere;
    struct Population* curr1;
    if(Species_head==NULL) {
        return 0;
    }
    else {
        node=Species_head;
        while(node!=NULL) {
            if(node->sid==sid) {
                ekei=node;
                struct Population* curr;
                if(node->Population_head!=NULL) {
                    curr=node->Population_head;
                    struct Population* temp = NULL;
                    if(curr->gid==gid) {
                       node->Population_head = curr->next;
                       curr1=curr;
                    }
                    else {     
                        while(curr!=NULL && curr->gid!=gid) {
                            temp=curr;
                            curr=curr->next;    
                        }
                        if(curr!=NULL) {
                            temp->next=curr->next;
                            curr1=curr;
                        }
                    }
                    
                    if(continents[curr1->cid]!=NULL) {
                        if(continents[curr1->cid]->populationList!=NULL) {
                            skere=curr1;
                            struct ContinentList* temp = continents[curr1->cid];
                            struct ContinentPopulation *temp1=temp->populationList;
                            struct ContinentPopulation *skr=NULL;
                            if(temp1->gid==gid) {
                                continents[curr1->cid]->populationList=temp1->next;
                            }
                            else {
                                temp1=continents[curr1->cid]->populationList->next;
                                skr=continents[curr1->cid]->populationList;
                                while(temp1!=temp->sentinel) {
                                    if(temp1->gid==gid) {
                                        skr->next=temp1->next;
                                        break;
                                    }
                                    skr=temp1;
                                    temp1=temp1->next;
                                }
                            }
                        }
                    }
                }

            }
            node=node->next;
        }
    }

    printf("K<%d><%d>\n", gid, sid);
    printf("SPECIES");
    struct Population* curr;
    curr=ekei->Population_head;
    printf("\n<%d>", ekei->sid);
    if(curr!=NULL) printf("\n  ");
    while(curr!=NULL) {
        printf("<%d>", curr->gid);
        curr=curr->next;
    }
    if(continents[curr1->cid]!=NULL) {
        printf("\nCONTINENTS\n");
        printf("Continent<%d>:", skere->cid);
        struct ContinentList* new;
        struct ContinentPopulation* new1;
        new=continents[skere->cid];
        if(new!=NULL) {
            new1=new->populationList;
            while(new1!=new->sentinel) {
                printf("<%d>", new1->gid);
                new1=new1->next;
            }
        }
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief delete species with ID <sid> and its populations
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_species(int sid){
	struct Species* node;
    if(Species_head==NULL) {
        return 0;
    }
    else {
        node = Species_tail;
        while(node!=NULL) {
            if(node->sid==sid) {
                struct Population* curr;
                if(node->Population_head!=NULL) {
                    curr=node->Population_head;
                    struct Population* temp;
                    struct ContinentPopulation *new;
                    
                    while(curr!=NULL) {
                        if(continents[curr->cid]!=NULL) {
                            if(continents[curr->cid]->populationList!=NULL) {
                                struct ContinentList* temp=continents[curr->cid];
                                new=temp->populationList;
                                struct ContinentPopulation *skr;
                                if(new->gid==curr->gid) {
                                    continents[curr->cid]->populationList=new->next;
                                }
                                else {
                                    while(new!=temp->sentinel && new->gid!=curr->gid) {
                                        skr=new;
                                        new=new->next;
                                    }
                                    if(new!=temp->sentinel) {
                                        skr->next=new->next;
                                    }
                                }
                            }
                        }

                        temp=curr->next;
                        curr=temp;
                    }
                    node->Population_head=NULL;
                    node->Population_tail=NULL;
                }
                struct Species* del = node;
                if(Species_head==del) {
                    Species_head=node->next;
                    Species_head->prev=NULL;
                }
                else {
                    if (del->next != NULL) {
                        del->next->prev = del->prev;
                        del->prev->next = del->next;
                    }
                    else {
                        del->prev->next=NULL;
                    }
                }
            }
            node=node->prev;
        }
    }

    node=Species_head;
    printf("F<%d>\n", sid);
    printf("SPECIES");
    while(node!=NULL) {
        struct Population* curr;
        curr=node->Population_head;
        printf("\n<%d>", node->sid);
        if(curr!=NULL) printf("\n  ");
        while(curr!=NULL) {
            printf("<%d>", curr->gid);
            curr=curr->next;
        }
        node=node->next;
    }
    printf("\n");
    int j;
    struct ContinentList* skr;
    struct ContinentPopulation* skere;
    for(j=0; j<5; j++) {
        skr=continents[j];
        printf("Continent %d: ", j);
        if(skr!=NULL) {
            skere=skr->populationList;
            while(skere!=skr->sentinel) {
                printf("<%d>", skere->gid);
                skere=skere->next;
            }
        }
        printf("\n");
    } 
    printf("DONE\n");
	return 1;
}

/**
 * @brief Remaining species evolve into homo sapiens.
 *
 * @return 1 on success
 *         0 on failure
 */
int evolution(){
	struct Species* node;

    if(Species_head==NULL) {
        return 0;
    }
    else {
        node=Species_head;
        struct Population* new = (struct Population*) malloc(sizeof(struct Population));
        struct Population* ekei;
        while(node!=NULL) {
            if(node->Population_head!=NULL) {
                new=node->Population_head;
                if(node==Species_tail) new->next_species=NULL;
                else new->next_species=node->next->Population_head; 

                if(hs_L==NULL) hs_L=new;
                ekei = (struct Population*) malloc(sizeof(struct Population));
                ekei=node->Population_tail;
                if(node==Species_tail) ekei->next=NULL;
                else {
                    new=node->next->Population_head;
                    ekei->next=new;
                }      
            }
            node = node->next;   
        }
    }

    struct Population* curr;
    struct Population* curr1;
    curr=hs_L;
    printf("E");
    printf("\n  ");
    printf("Homo Sapiens:");
    while(curr!=NULL) {
        printf("[");
        curr1=curr;
        if(curr->next_species!=NULL) {
            while(curr1->next!=NULL) {
                if(curr1->sid!=curr1->next->sid) {
                    printf("<%d,%d>", curr1->gid, curr1->sid);
                    break;
                }
                printf("<%d,%d>,", curr1->gid, curr1->sid);
                curr1=curr1->next;
            }
            printf("]");
        }
        else {
           while(curr1->next!=NULL) {
               printf("<%d,%d>,", curr1->gid, curr1->sid);
               curr1=curr1->next;
           }
           printf("<%d,%d>", curr1->gid, curr1->sid);
           printf("]"); 
        }
        curr1->next=NULL;
        curr=curr->next_species;
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Gather statistics on species from homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int species_statistics(){
	struct Population* node;
    int num = 0;

    if(hs_L==NULL) {
        return 0;
    }
    else {
        node=hs_L;
        while(node!=NULL) {
            num++;
            node=node->next_species;
        }
    }

    node=hs_L;
    printf("N");
    printf("\n  ");
    printf("Homo Sapiens species:<%d>", num);
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Gather statistics on population belonging to species with ID <sid> from homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int population_statistics(int sid){
	struct Population* node;
    int num=0;

    if(hs_L==NULL) {
        return 0;
    }
    else {
        node=hs_L;
        while(node!=NULL) {
            if(node->sid==sid) {
                struct Population* curr=node;
                while(curr->next!=NULL) {
                    if(node->next_species!=NULL) {
                        if(curr->sid!=curr->next->sid) break;
                        num++;
                    }
                    else {
                        num++;
                    }
                    curr=curr->next;
                }
                if(node->next_species!=NULL) num++;  //gia to teleutaio stoixeio
            }
            node=node->next_species;
        }
    }

    node=hs_L;
    printf("J<%d>", sid);
    printf("\n  ");
    printf("Homo Sapiens populations:<%d>", num);
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print species' list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_species(){
	struct Species* node=Species_head;
    printf("P");
    printf("\n ");
    while(node!=NULL) {
        printf("<%d>", node->sid);
        node=node->next;
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print species' list with population lists
 *
 * @return 1 on success
 *         0 on failure
 */
int print_populations(){
	struct Species* node=Species_head;
    printf("X");
    while(node!=NULL) {
        struct Population* curr;
        curr=node->Population_head;
        printf("\n <%d>", node->sid);
        if(curr!=NULL) printf("\n  ");
        while(curr!=NULL) {
            printf("<%d,%d>", curr->gid, curr->cid);
            curr=curr->next;
        }
        node=node->next;
    } 
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print continents array with each continent's population list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_continents(){
	int j;
    struct ContinentList* skr;
    struct ContinentPopulation* skere;
    for(j=0; j<5; j++) {
        skr=continents[j];
        printf("Continent %d: ", j);
        if(skr!=NULL) {
            skere=skr->populationList;
            while(skere!=skr->sentinel) {
                printf("<%d>", skere->gid);
                skere=skere->next;
            }
        }
        printf("\n");
    } 
    printf("DONE\n");
	return 1;
}

/**
 * @brief Print homo_sapiens list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_homo_sapiens(){
	struct Population* curr=hs_L;
    printf("W");
    printf("\n  ");
    printf("Homo Sapiens:");
    while(curr!=NULL) {
        printf("[");
        while(curr->next!=NULL) {
            printf("<%d,%d>", curr->gid, curr->sid);
            if(curr->sid!=curr->next->sid) break;
            curr=curr->next;
        }
        if(curr->next==NULL) printf("<%d,%d>", curr->gid, curr->sid);
        printf("]");
        curr=curr->next_species;
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Free resources
 *
 * @return 1 on success
 *         0 on failure
 */
int free_all(void)
{
	return 1;
}


/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
	FILE *fin = NULL;
	char buff[BUFFER_SIZE], event;

	/* Check command buff arguments */
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Open input file */
	if (( fin = fopen(argv[1], "r") ) == NULL ) {
		fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
		perror("Opening test file\n");
		return EXIT_FAILURE;
	}

	/* Initializations */
	initialize();

	/* Read input file buff-by-buff and handle the events */
	while (fgets(buff, BUFFER_SIZE, fin)) {

		DPRINT("\n>>> Event: %s", buff);

		switch(buff[0]) {

			/* Comment */
			case '#':
				break;

				/* Insert Species
				 * S <sid> */
			case 'S':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (insert_species(sid)) {
						DPRINT("%c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, sid);
					}

					break;
				}

				/* Insert population
				 * G <gid> <sid> <cid> */
			case 'G':
				{
					int gid, sid, cid;

					sscanf(buff, "%c %d %d %d", &event, &gid, &sid, &cid);
					DPRINT("%c %d %d %d\n", event, gid, sid, cid);

					if (insert_population(gid, sid, cid)) {
						DPRINT("%c %d %d %d succeeded\n", event, gid, sid, cid);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, gid, sid, cid);
					}

					break;
				}

				/* Merge species
				 * M <sid1> <sid2> <sid3> */
			case 'M':
				{
					int sid1, sid2, sid3;

					sscanf(buff, "%c %d %d %d", &event, &sid1, &sid2, &sid3);
					DPRINT("%c %d %d %d\n", event, sid1, sid2, sid3);

					if (merge_species(sid1, sid2, sid3)) {
						DPRINT("%c %d %d %d succeeded\n", event, sid1, sid2, sid3);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, sid1, sid2, sid3);
					}

					break;
				}

				/* Distribute species
				 * D */
			case 'D':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (distribute()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Delete population
				 * K <gid> <sid> */
			case 'K':
				{
					int gid, sid;

					sscanf(buff, "%c %d %d", &event, &gid, &sid);
					DPRINT("%c %d %d\n", event, gid, sid);

					if (delete_population(gid, sid)) {
						DPRINT("%c %d %d succeeded\n", event, gid, sid);
					} else {
						fprintf(stderr, "%c %d %d failed\n", event, gid, sid);
					}

					break;
				}

				/* Delete species
				 * F <sid> */
			case 'F':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (delete_species(sid)) {
						DPRINT("%c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, sid);
					}

					break;
				}

				/* Evolution to homo sapiens
				 * E */
			case 'E':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (evolution()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Species' statistics
				 * N */
			case 'N':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (species_statistics()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Population statistics
				 * J <sid> */
			case 'J':
				{
					int sid;

					sscanf(buff, "%c %d", &event, &sid);
					DPRINT("%c %d\n", event, sid);

					if (population_statistics(sid)) {
						DPRINT("%c %d succeeded\n", event, sid);
					} else {
						fprintf(stderr, "%c %d failed\n", event, sid);
					}

					break;
				}

				/* Print species
				 * P */
			case 'P':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_species()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print populations
				 * X */
			case 'X':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_populations()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print continents
				 * C */
			case 'C':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_continents()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

			/* Print homo sapiens
				 * W */
			case 'W':
				{
					sscanf(buff, "%c", &event);
					DPRINT("%c\n", event);

					if (print_homo_sapiens()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
					}

					break;
				}

				/* Empty line */
			case '\n':
				break;

				/* Ignore everything else */
			default:
				DPRINT("Ignoring buff: %s \n", buff);

				break;
		}
	}

	free_all();
	return (EXIT_SUCCESS);
}
