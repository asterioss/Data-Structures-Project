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
    Species_root=NULL;
    Homo_Sapiens_root=NULL;
    continents[0]=NULL;
    continents[1]=NULL;
    continents[2]=NULL;
    continents[3]=NULL;
    continents[4]=NULL;
	return 1;
}

//print Species tree
void printPostorder(struct Species* node) {
    if (node == NULL) return; 
  
    // first recur on left subtree 
    printPostorder(node->lc); 
  
    // then recur on right subtree 
    printPostorder(node->rc); 
  
    // now deal with the node 
    printf("<%d>", node->sid); 
}

//print Population tree
void printInorder(struct Population* node) { 
    if (node == NULL) return; 
  
    /* first recur on left child */
    printInorder(node->lc); 
  
    /* then print the data of node */
    printf("<%d>", node->gid);   
  
    /* now recur on right child */
    printInorder(node->rc); 
}

//search node in Species tree
struct Species* searchNode(struct Species* node, int key) {
    if(node==NULL) return NULL;
    else if(node->sid==key) return node;
    else {
        struct Species* x = searchNode(node->lc, key);
        if(x) return x;
        return searchNode(node->rc, key);
    }  
}

//S - insert species
void insertSpecies(struct Species* node, int sid, struct Species* new) {
    while(node!=NULL) {
        if(sid>node->sid && node==Species_root->lc) node=Species_root->rc;
        else if(sid<node->sid) {
            if(node->lc==NULL) {
               node->lc=new;
               break; 
            }
            else {
                struct Species* prev=NULL;
                while(node!=NULL && sid<node->sid) {
                    prev=node;
                    node=node->lc;
                }
                if(prev->lc==NULL) {
                    prev->lc=new;
                    break;
                }
                else if(prev->lc!=NULL) {
                    if(sid>prev->lc->sid) {
                        if(prev->rc==NULL) {
                            prev->rc=new;
                            break;
                        }
                        else node=prev->rc;
                    }
                }
            }
        }
        else if(sid>node->sid) {
            new->lc=node->lc;
            new->sid=node->sid;
            if(node->rc!=NULL && node->sid>node->rc->sid) {
                new->rc=node->rc;
                node->rc=NULL;
            }
            node->sid=sid;
            node->lc=new;
            break;
        }
    }
}

//L - find lowest common ancestor
struct Population *lca(struct Population* root, int n1, int n2) {
    if(root == NULL) return NULL; 

    if (n1<root->gid && n2<root->gid) return lca(root->lc, n1, n2); 
  
    if (n1>root->gid && n2>root->gid) return lca(root->rc, n1, n2); 
  
    return root; 
}

//M (merge) - insert to new tree
void inserttoTree(struct Population** root, struct Population* ekei) { 
    if (*root == NULL) {
        struct Population* node = (struct Population*)malloc(sizeof(struct Population)); 
        node=ekei;
        node->parent=NULL;
        node->lc = node->rc = NULL;  
        *root=node;
        return;
    }
    else {
        struct Population* r=*root;  //deikths sthn arxh ths ypolistas
        struct Population* prev;
        while(r!=NULL) {
            prev=r;
            if(ekei->gid<r->gid) r=r->lc;
            else if(ekei->gid>r->gid) r=r->rc;
        }
        if(r==NULL) {
            struct Population* node = (struct Population*)malloc(sizeof(struct Population)); 
            node=ekei;
            node->parent=prev;
            node->lc = node->rc = NULL;
            r=node;
            if(ekei->gid<prev->gid) prev->lc=r;
            else if(ekei->gid>prev->gid) prev->rc=r;
        }
    }
}

//return size of tree
int sizeofTree(struct Population* root)  
{  
    if (root == NULL) return 0;  
    int leftsize = sizeofTree(root->lc);
    int rightsize = sizeofTree(root->rc);
    return leftsize + rightsize +1;  
}

//M (merge) - tree to array
void TreetoArray(struct Population *root, struct Population* a[], int *i) {
    if(root==NULL) return;
    TreetoArray(root->lc, a, i); 
    a[*i] = root;
    (*i)++;
    TreetoArray(root->rc, a, i); 
}

//M (merge) - array to tree
void ArraytoTree(struct Population** root, int start, int end, struct Population* a[]) {
    int mid;
    if(start>end) return;
    mid = (start+end)/2;

    inserttoTree(&(*root), a[mid]); 
    ArraytoTree(&(*root)->lc, start, mid-1, a);
    ArraytoTree(&(*root)->rc, mid+1, end, a);
}

//delete node from Species tree
void deletefromSpecies(struct Species** node, int key, struct Species* root) {
    if (*node == NULL) return; 
    
    deletefromSpecies(&(*node)->lc, key, root);
    deletefromSpecies(&(*node)->rc, key, root);
    if((*node)->sid==key) {
        if(*node==root) {
            *node = (*node)->rc;
            return;
        }
        if((*node)->lc==NULL && (*node)->rc==NULL) {
            *node=NULL;
            return;
        }
        else if((*node)->lc==NULL && (*node)->rc!=NULL) {
            *node = (*node)->rc; 
            return;
        } 
        else if((*node)->lc!=NULL && (*node)->rc==NULL) {
            *node = (*node)->lc; 
            return; 
        }
        //node has two children
        else {
            struct Species* temp = (*node)->lc;
            *node = (*node)->rc;
            (*node)->lc=temp; 
        }
    }

}

//K - search delete node
struct Population* searchDeleteNode(struct Population* node, int key) {
    if(node==NULL) return NULL;
    else if(node->gid==key) return node;
    else {
        struct Population* x = searchDeleteNode(node->lc, key);
        if(x) return x;
        return searchDeleteNode(node->rc, key);
    }  
}

//K - find minimum in right subtree for Continents
struct ContinentPopulation* minimumKeyContinents(struct ContinentPopulation* curr) {
	while(curr->lc != NULL) {
		curr = curr->lc;
	}
	return curr;
}

//K - delete from Continents
void deletefromContinents(struct ContinentPopulation** node, int key) {
    if (*node == NULL) return; 

    if (key<(*node)->gid) deletefromContinents(&(*node)->lc, key); 
    else if (key > (*node)->gid) deletefromContinents(&(*node)->rc, key); 
    else {
        if((*node)->lc==NULL && (*node)->rc==NULL) {
            *node=NULL;
            //return;
        }
        else if((*node)->lc==NULL && (*node)->rc!=NULL) {
            *node = (*node)->rc; 
            //return;
        } 
        else if((*node)->lc!=NULL && (*node)->rc==NULL) {
            *node = (*node)->lc; 
            //return; 
        }
        //node has two children
        else {
            struct ContinentPopulation* temp = *node;
            temp = minimumKeyContinents(temp->rc);
            (*node)->gid = temp->gid;
            deletefromContinents(&(*node)->rc, temp->gid);
        }
    }
    return;
}

//K - find minimum in right subtree for Population deletion
struct Population* minimumKey(struct Population* curr) {
	while(curr->lc != NULL) {
		curr = curr->lc;
	}
	return curr;
}

//K - delete from Population tree
void deletePopulation(struct Population** node, int key) {
    if (*node == NULL) return; 

    if (key<(*node)->gid) deletePopulation(&(*node)->lc, key); 
    else if (key>(*node)->gid) deletePopulation(&(*node)->rc, key); 
    else {
        if((*node)->lc==NULL && (*node)->rc==NULL) {
            *node=NULL;
            //return;
        }
        else if((*node)->lc==NULL && (*node)->rc!=NULL) {
            *node = (*node)->rc; 
            //return;
        } 
        else if((*node)->lc!=NULL && (*node)->rc==NULL) {
            *node = (*node)->lc; 
            //return; 
        }
        //node has two children
        else {
            struct Population *temp = *node;
            temp = minimumKey(temp->rc);
            (*node)->gid = temp->gid;
            deletePopulation(&(*node)->rc, temp->gid);
        }
    }
    return;
}

//F - print all sids and all gids for each sid
void printFSpecies(struct Species* node) {
    if (node == NULL) return; 
  
    // first recur on left subtree 
    printFSpecies(node->lc); 
  
    // then recur on right subtree 
    printFSpecies(node->rc); 
  
    // now deal with the node 
    printf("\n <%d>\n  ", node->sid);
    if(node->population_root!=NULL) {
        struct Population* root = node->population_root;
        printInorder(root);
    }
}

//F - delete Population tree
void deleteTree(struct Population* node) {
    if (node == NULL) return;  
  
    deleteTree(node->lc);  
    deleteTree(node->rc);  
      
    if(continents[node->cid]!=NULL) {     //delete node from continents
        if(continents[node->cid]->population_root!=NULL) {
            struct ContinentPopulation* skr = continents[node->cid]->population_root;
            deletefromContinents(&skr, node->gid);
        }        
    }

    node=NULL;    //delete node from Population
}

//F - find min sid and return node
struct Species* minValue(struct Species* node) {  
   struct Species* current = node;  
  
   while (current->lc != NULL)  {  
       current = current->lc;  
   }
   return current;
}

//E - print the nodes
static int b=0;
void printHomo(struct Population* node) { 
     if (node == NULL) return; 
  
     /* first recur on left child */
     printHomo(node->lc); 
  
     /* then print the data of node */
     printf("<%d,%d>", node->gid, node->sid);   
  
     /* now recur on right child */
     printHomo(node->rc); 
}

//E - inorder HomoSapiens tree
void InorderSapiens(struct HomoSapiens* node) {
     if (node == NULL) return; 
  
     /* first recur on left child */
     InorderSapiens(node->lc); 

     if(node->lc==NULL && node->rc==NULL && b==0) {
        struct HomoSapiens* current = node; 
        while(current->next!=NULL) {
           if(current->population_root!=NULL) {
               printf("[");
               printHomo(current->population_root);
               printf("]");
           }
           else {
              printf("[");
              printf("<%d>", current->sid);
              printf("]"); 
           }
           current=current->next;     
        }
        b=1;
     } 
  
     /* now recur on right child */
     InorderSapiens(node->rc); 
}

//E - set next pointers to HomoSapiens tree
void setNext(struct HomoSapiens* node, struct HomoSapiens** prev) {
     if(node == NULL) return; 
  
     setNext(node->lc, prev); 
  
     if(node->lc==NULL && node->rc==NULL) {
         if(b!=0) {
            (*prev)->next=node;
         }
         *prev=node;
         b++;
     }
  
     setNext(node->rc, prev); 
}

//E - insert to homo sapiens
static int a=0;
void insertHomoSapiens(struct HomoSapiens** hs, struct Species *node) {
     if(*hs == NULL && Homo_Sapiens_root!=NULL) return;
  
     if(Homo_Sapiens_root==NULL) {
         struct HomoSapiens *new=(struct HomoSapiens*) malloc(sizeof(struct HomoSapiens));
         new->sid=node->sid;
         new->population_root=node->population_root;
         new->lc=NULL;
         new->next=NULL;
         Homo_Sapiens_root=new;

         struct HomoSapiens *r=(struct HomoSapiens*) malloc(sizeof(struct HomoSapiens));
         r->sid=node->sid;
         r->population_root=node->population_root;
         r->next=NULL;
         r->rc=r->lc=NULL;
         Homo_Sapiens_root->rc=r;
         return;
     }
     else {
         if(node->sid<(*hs)->sid) {
             if((*hs)->lc==NULL && a==0) {
                struct HomoSapiens *new=(struct HomoSapiens*) malloc(sizeof(struct HomoSapiens));
                
                new->sid=node->sid;
                new->population_root=node->population_root;
                new->lc=new->rc=NULL;
                new->next=NULL;
                (*hs)->lc=new;
                
                a=1;
                return;
             }
         }
         else if(node->sid>=(*hs)->sid) {
             if((*hs)->rc==NULL && a==0) {
                struct HomoSapiens *new=(struct HomoSapiens*) malloc(sizeof(struct HomoSapiens));

                struct HomoSapiens *l=(struct HomoSapiens*) malloc(sizeof(struct HomoSapiens));
                l->sid=(*hs)->sid;
                l->population_root=(*hs)->population_root;
                l->rc=l->lc=NULL;
                l->next=NULL;
                new->lc=l;

                struct HomoSapiens *r=(struct HomoSapiens*) malloc(sizeof(struct HomoSapiens));
                r->sid=node->sid;
                r->population_root=node->population_root;
                r->rc=r->lc=NULL;
                r->next=NULL;
                new->rc=r;
                
                new->sid=node->sid;
                new->population_root=node->population_root;
                new->next=NULL;
                (*hs)->rc=new;   

                a=1;
                return;     
             }
         }
     }

     if(node->sid<(*hs)->sid) insertHomoSapiens(&(*hs)->lc, node); 
     else if(node->sid>=(*hs)->sid) insertHomoSapiens(&(*hs)->rc, node); 
}

//N - print sids of HomoSapiens
void printHomoSapiens(struct HomoSapiens* node) {  
    if (node == NULL) return; 
  
     printHomoSapiens(node->lc); 
  
     if(node->lc==NULL && node->rc==NULL && b==0) {
        struct HomoSapiens* current = node; 
        printf("N\n ");
        printf("Homo Sapiens: ");
        int i=0;
        while(current->next!=NULL) {
           printf("<%d>,", current->sid);
           i++;
           current=current->next;     
        }
        printf("<%d>", current->sid);
        i++;
        printf("\n Homo Sapiens species: %d", i);
        printf("\nDONE\n");
        b=1;
     }
  
     printHomoSapiens(node->rc);
  
}

//J - find number of nodes in population
void populationNumber(struct Population* node, int *i) { 
     if (node == NULL) return; 
  
     /* first recur on left child */
     populationNumber(node->lc, i);  
  
     (*i)++;
  
     /* now recur on right child */
     populationNumber(node->rc, i);
}

//J - search sid and print statistics
void searchSapiens(struct HomoSapiens* node, int key) { 
     if (node == NULL) return; 
  
     searchSapiens(node->lc, key);

     if(node->sid==key && b==0) {
         printf("J<%d>\n ", key);
         int i=0;
         populationNumber(node->population_root, &i);
         printf("Homo Sapiens populations: %d", i);
         printf("\nDONE\n");
         b=1;
     }
  
     searchSapiens(node->rc, key); 
}

//D - print ContinentPopulation list
void ContinentsInorder(struct ContinentPopulation* node) {
    if(node == NULL) return; 
  
    /* first recur on left child */
    ContinentsInorder(node->lc); 
  
    /* then print the data of node */
    if(node->gid!=-1) printf("%d ", node->gid);
        
  
    /* now recur on right child */
    ContinentsInorder(node->rc); 
}

//D - traverse Population list with pre-order and make continents array
int i;   //for each continent (from 0 to 4)
void PopulationPreorder(struct Population* node) {
    if (node == NULL) return; 

    if(node->cid==0) {
        i=0;
        if(continents[i]==NULL) {
            continents[i]=(struct ContinentTree*) malloc(sizeof(struct ContinentTree));
            continents[i]->population_root = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
            continents[i]->population_root=NULL;
        }
    }
    else if(node->cid==1) {
        i=1;
        if(continents[i]==NULL) {
            continents[i]=(struct ContinentTree*) malloc(sizeof(struct ContinentTree));
            continents[i]->population_root = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
            continents[i]->population_root=NULL;
        }
    }
    else if(node->cid==2) {
        i=2;
        if(continents[i]==NULL) {
            continents[i]=(struct ContinentTree*) malloc(sizeof(struct ContinentTree));
            continents[i]->population_root = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
            continents[i]->population_root=NULL;
        }
    }
    else if(node->cid==3) {
        i=3;
        if(continents[i]==NULL) {
            continents[i]=(struct ContinentTree*) malloc(sizeof(struct ContinentTree));
            continents[i]->population_root = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
            continents[i]->population_root=NULL;
        }
    }
    else if(node->cid==4) {
        i=4;
        if(continents[i]==NULL) {
            continents[i]=(struct ContinentTree*) malloc(sizeof(struct ContinentTree));
            continents[i]->population_root = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
            continents[i]->population_root=NULL;
        }
    }

    if(continents[i]->population_root==NULL) {
        struct ContinentPopulation* new = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
        continents[i]->sentinel=(struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
        continents[i]->sentinel->lc=NULL;
        continents[i]->sentinel->rc=NULL;
        continents[i]->sentinel->gid=-1;
        new->gid=node->gid;
        new->lc=new->rc=continents[i]->sentinel;
        continents[i]->population_root=new;
    }
    else {
        struct ContinentPopulation* ekei = continents[i]->population_root;
        struct ContinentPopulation* prev;
        while(ekei!=continents[i]->sentinel) {
            prev=ekei;
            if(node->gid<ekei->gid) ekei=ekei->lc;
            else if(node->gid>ekei->gid) ekei=ekei->rc;
        }
        if(ekei==continents[i]->sentinel) {
            struct ContinentPopulation* new = (struct ContinentPopulation*) malloc(sizeof(struct ContinentPopulation));
            new->gid=node->gid;
            new->lc=new->rc=continents[i]->sentinel;
            ekei=new;
            if(node->gid<prev->gid) prev->lc=ekei;
            else if(node->gid>prev->gid) prev->rc=ekei;
        }    
    }

    /* then recur on left sutree */
    PopulationPreorder(node->lc);   
  
    /* now recur on right subtree */
    PopulationPreorder(node->rc); 
}

//D - traverse Species list with pre-order
void SpeciesPreorder(struct Species* node, char c) {
    if(node == NULL) return; 
  
    if(c=='D') {      //if function that calls SpeciesPreorder is D
      if(node->population_root!=NULL) {
        struct Population *root = node->population_root;
        PopulationPreorder(root);
      }
    }
    else if(c=='E') {     //if function that calls SpeciesPreorder is E
       struct HomoSapiens *hs=Homo_Sapiens_root;
       insertHomoSapiens(&hs, node);
       a=0;
    }
  
    /* then recur on left sutree */
    SpeciesPreorder(node->lc, c);   
  
    /* now recur on right subtree */
    SpeciesPreorder(node->rc, c); 
}



/**
 * @brief insert new species in Species' list with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_species(int sid){
	struct Species *p;
    struct Species* new = (struct Species*) malloc(sizeof(struct Species));
    new->sid=sid;
    new->lc=new->rc=NULL;
    new->population_root=NULL;

    if(Species_root==NULL) {
        Species_root=new;
    }
    else {
        p=Species_root;
        
        if(p->lc==NULL && sid<Species_root->sid) {
            p->lc=new;
        }
        else if(p->lc==NULL && sid>Species_root->sid) {
            Species_root=new;
            Species_root->lc=p;
        }
        else if(p->lc!=NULL && sid>Species_root->sid) {
            new->sid=p->sid;
            new->population_root=p->population_root;
            Species_root->sid=sid;
            new->lc=Species_root->rc;
            Species_root->rc=new;
        }
        else if(p->lc!=NULL && sid>p->lc->sid && p->rc==NULL) {
            p->rc=new;
        }
        else {
            insertSpecies(p->lc, sid, new);
        }
    }

    printf("S<%d>\n ", sid);
    printPostorder(Species_root);
    printf("\nDONE");
    printf("\n");
	return 1;
}

/**
 * @brief insert new population with ID <gid> in Species' with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int insert_population(int gid, int sid, int cid){
    struct Species* p;
    struct Population* new = (struct Population*) malloc(sizeof(struct Population));
    new->gid=gid;
    new->sid=sid;
    new->cid=cid;
    new->lc=new->rc=NULL;
    if(Species_root==NULL) {
        return 0;
    }
    else {
        p=searchNode(Species_root, sid);    //anazhthsh komvou sid
        if(p==NULL) return 0;
        else {
            if(p->population_root==NULL) {
                new->parent=NULL;
                p->population_root=new;
            }
            else {
                struct Population* r=p->population_root;  //deikths sthn arxh ths ypolistas
                struct Population* prev;
                while(r!=NULL) {
                    prev=r;
                    if(gid<r->gid) r=r->lc;
                    else if(gid>r->gid) r=r->rc;
                }
                if(r==NULL) {
                    new->parent=prev;
                    r=new;
                    if(gid<prev->gid) prev->lc=r;
                    else if(gid>prev->gid) prev->rc=r;
                }
            }
        }
    }

    printf("G<%d><%d><%d>\n ", gid, sid, cid);
    printf("<%d>\n  ", sid);
    printInorder(p->population_root);
    printf("\nDONE");
    printf("\n");
	return 1;
}

/**
 * @brief find the lowest (earliest) common ancestor of populations with ID <gid1> and <gid2> of species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int lowest_common_ancestor(int sid, int gid1, int gid2){
    struct Species* p;
    struct Population *g;        //lca node
    if(Species_root==NULL) {
        return 0;
    }
    else {
        p=searchNode(Species_root, sid);
        struct Population* r;
        if(p->population_root!=NULL) {
            r=p->population_root;
            g=lca(r,gid1,gid2);
        }
    }

    printf("L<%d><%d><%d>\n ", sid, gid1, gid2);
    printf("Lowest Common Ancestor: <%d>", g->gid);
    printf("\nDONE");
    printf("\n");
	return 1;
}

/**
 * @brief merge species with IDs <sid1> and <sid2> into new species with ID <sid3>
 *
 * @return 1 on success
 *         0 on failure
 */
int merge_species(int sid1, int sid2, int sid3){
    int i=0, j=0, k, x;
    struct Species *p1,*p2,*p3;

    if(Species_root==NULL) {
        return 0;
    }
    else {
        insert_species(sid3);
        p1=searchNode(Species_root,sid1);    //node with sid1
        p2=searchNode(Species_root,sid2);    //node with sid2
        p3=searchNode(Species_root,sid3);    //node with sid3
        if(p1==NULL || p2==NULL) return 0;
        else if((p1!=NULL && p1->population_root==NULL) && (p2!=NULL && p2->population_root==NULL)) return 0;
        else if(p1->population_root==NULL && p2->population_root!=NULL) { 
           int size=sizeofTree(p2->population_root);
           struct Population **a;
           a = (struct Population **) calloc(size, sizeof(struct Population*));
           TreetoArray(p2->population_root, a, &i);    //tree to array
           i=0, j=0, k=0;
           ArraytoTree(&p3->population_root, 0, size-1, a);     //array to tree
           deletefromSpecies(&Species_root, sid2, Species_root);
        }
        else if (p1->population_root!=NULL && p2->population_root==NULL) { 
           int size=sizeofTree(p1->population_root);
           struct Population **a;
           a = (struct Population **) calloc(size, sizeof(struct Population*));
           TreetoArray(p1->population_root, a, &i);    //tree to array
           i=0, j=0, k=0;
           ArraytoTree(&p3->population_root, 0, size-1, a);     //array to tree
           deletefromSpecies(&Species_root, sid1, Species_root);
        }
        else if(p1->population_root!=NULL && p2->population_root!=NULL) {
            struct Population* r1=p1->population_root;
            struct Population* r2=p2->population_root;

            int size1=sizeofTree(r1);
            int size2=sizeofTree(r2);
            struct Population **a;
            struct Population **b;
            a = (struct Population **) calloc(size1, sizeof(struct Population*));
            b = (struct Population **) calloc(size2, sizeof(struct Population*));
            int size3 = size1+size2;
            struct Population **c;
            c = (struct Population **) calloc(size3, sizeof(struct Population*));
            TreetoArray(r1, a, &i);    //tree to array
            TreetoArray(r2, b, &j);    //tree to array

            i=0, j=0, k=0;
            for(x=0; x<size3; x++) {          //merge the sorted arrays
               if(i!=size1 && j!=size2) {
                  if(a[i]->gid<b[j]->gid) {
                      c[k++] = a[i++];
                  }
                  else {
                      c[k++] = b[j++];
                  }
               }
               if(i==size1 && j!=size2) {
                   c[k++]=b[j++];
               }
               if(j==size2 && i!=size1) {
                   c[k++]=a[i++];
               }
            }

            ArraytoTree(&p3->population_root, 0, size3-1, c);     //array to tree

            deletefromSpecies(&Species_root, sid1, Species_root);
            deletefromSpecies(&Species_root, sid2, Species_root);
        }    
    }

    printf("M<%d><%d><%d>", sid1,sid2,sid3);
    printFSpecies(Species_root);
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
    struct Species* p;
    if(Species_root==NULL) {
        return 0;
    }
    else {
        char c = 'D';
        p=Species_root;
        SpeciesPreorder(p, c);
    }

    printf("D");
    int i;
    for(i=0; i<5; i++) {
       if(continents[i]!=NULL) {
          if(continents[i]->population_root!=NULL) {
             printf("\nContinent %d: ",i);
             struct ContinentPopulation* start = continents[i]->population_root;  
             ContinentsInorder(start);
          }
       }
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief delete population with ID <gid> from species with ID <sid>
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_population(int gid, int sid){
    struct Species* p;
    struct Population* r;   //pointer to Population
    struct Population* deletenode;   //pointer to delete node
    if(Species_root==NULL) {
        return 0;
    }
    else {
        p=searchNode(Species_root, sid);
        if(p->population_root!=NULL) {
            r=p->population_root;
            deletenode = searchDeleteNode(r, gid);    //search delete node
            if(deletenode==NULL) return 0;

            deletePopulation(&r, gid);    //delete node from population list

            if(continents[deletenode->cid]!=NULL) {
                if(continents[deletenode->cid]->population_root!=NULL) {
                   struct ContinentPopulation* skr = continents[deletenode->cid]->population_root;
                   deletefromContinents(&skr, gid);     //delete node from continents
                }    
            }
        }
    }

    printf("K<%d><%d>\n",gid,sid);
    printf("SPECIES\n ");
    printf("<%d>\n",sid);
    printf("  ");
    printInorder(r);
    if(continents[deletenode->cid]!=NULL) {
        if(continents[deletenode->cid]->population_root!=NULL) {
            printf("\nCONTINENTS\n ");
            printf("Conintent<%d>: ", deletenode->cid);
            struct ContinentPopulation* skr = continents[deletenode->cid]->population_root;
            ContinentsInorder(skr);
        }         
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief delete the species with lowest id and its populations
 *
 * @return 1 on success
 *         0 on failure
 */
int delete_species(){
    struct Species* p;
    if(Species_root==NULL) {
        return 0;
    }
    else {
        p = minValue(Species_root);    //find minimum sid
        if(p->population_root!=NULL) {
           struct Population* r=p->population_root;
           deleteTree(r);   //delete population tree
           r=NULL;

           deletefromSpecies(&Species_root, p->sid, Species_root);
        }
    }

    printf("F\n");
    printf("SPECIES");
    printFSpecies(Species_root);
    int i;
    printf("\nCONINENTS");
    for(i=0; i<5; i++) {
       if(continents[i]!=NULL) {
          if(continents[i]->population_root!=NULL) {
             printf("\nContinent %d: ",i);
             struct ContinentPopulation* start = continents[i]->population_root;  
             ContinentsInorder(start);
          }
       }
    }
    printf("\nDONE");
    printf("\n");
	return 1;
}

/**
 * @brief Remaining species evolve into homo sapiens.
 *
 * @return 1 on success
 *         0 on failure
 */
int evolution(){
    struct Species* p;
    if(Species_root==NULL) {
        return 0;
    }
    else {
        char c = 'E';
        p=Species_root;
        SpeciesPreorder(p, c);
        struct HomoSapiens *prev=NULL;
        setNext(Homo_Sapiens_root, &prev);    //set the next pointers
        b=0;

        printf("E\n ");
        printf("Homo Sapiens: ");
        InorderSapiens(Homo_Sapiens_root);
        b=0;
        printf("\nDONE\n");
    }
	return 1;
}

/**
 * @brief Gather statistics on species from homo_sapiens tree
 *
 * @return 1 on success
 *         0 on failure
 */
int species_statistics(){
    struct HomoSapiens *hs;
    if(Homo_Sapiens_root==NULL) {
        return 0;
    }
    else {
        hs=Homo_Sapiens_root;
        printHomoSapiens(hs);
        b=0;
    }
	return 1;
}

/**
 * @brief Gather statistics on population belonging to species with ID <sid> from homo_sapiens tree
 *
 * @return 1 on success
 *         0 on failure
 */
int population_statistics(int sid){
    struct HomoSapiens *hs;
    if(Homo_Sapiens_root==NULL) {
        return 0;
    }
    else {
        hs=Homo_Sapiens_root;
        searchSapiens(hs, sid);
        b=0;
    }
	return 1;
}

/**
 * @brief Print species' leaves list
 *
 * @return 1 on success
 *         0 on failure
 */
int print_species(){
    struct Species* p;
    if(Species_root==NULL) {
        return 0;
    }
    else {
        p=Species_root;
        printf("P\n ");
        printPostorder(Species_root);
        printf("\nDONE\n");
    }
	return 1;
}

/**
 * @brief Print species' tree with population trees
 *
 * @return 1 on success
 *         0 on failure
 */
int print_populations(){
    struct Species* p;
    if(Species_root==NULL) {
        return 0;
    }
    else {
        p=Species_root;
        printf("X");
        printFSpecies(p);
        printf("\nDONE\n");
    }
	return 1;
}

/**
 * @brief Print continents array with each continent's population tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_continents(){
    int i;
    for(i=0; i<5; i++) {
       if(continents[i]!=NULL) {
          if(continents[i]->population_root!=NULL) {
             if(i==0) printf("Continent %d: ",i);
             else printf("\nContinent %d: ",i);
             struct ContinentPopulation* start = continents[i]->population_root;  
             ContinentsInorder(start);
          }
       }
    }
    printf("\nDONE\n");
	return 1;
}

/**
 * @brief Print homo_sapiens tree
 *
 * @return 1 on success
 *         0 on failure
 */
int print_homo_sapiens(){
    struct HomoSapiens *hs;
    if(Homo_Sapiens_root==NULL) {
        return 0;
    }
    else {
        hs=Homo_Sapiens_root;
        printf("W\n ");
        printf("Homo Sapiens: ");
        InorderSapiens(hs);
        b=0;
        printf("\nDONE\n");
    }
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

				/* Lowest Common Ancestor
				 * L <sid> <gid1> <gid2> */
			 case 'L':
				{
					int sid, gid1, gid2;

					sscanf(buff, "%c %d %d %d", &event, &sid, &gid1, &gid2);
					DPRINT("%c %d %d %d\n", event, sid, gid1, gid2);

					if (lowest_common_ancestor(sid, gid1, gid2)) {
						DPRINT("%c %d %d %d succeeded\n", event, sid, gid1, gid2);
					} else {
						fprintf(stderr, "%c %d %d %d failed\n", event, sid, gid1, gid2);
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
				 * F */
			case 'F':
				{
					DPRINT("%c \n", event);

					if (delete_species()) {
						DPRINT("%c succeeded\n", event);
					} else {
						fprintf(stderr, "%c failed\n", event);
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
