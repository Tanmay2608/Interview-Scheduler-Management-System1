#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_QUEUE 100
#define MAX_ACTIVITY 10
#define TABLE_SIZE 100

/* =========================================================
   CANDIDATE STRUCTURE
   ========================================================= */

typedef struct Candidate {
    char id[20];
    char name[50];
    char email[60];
    char phone[20];
    char position[50];
    float experience;
    char status[30];

    struct Candidate *next;
} Candidate;


/* =========================================================
   1. QUEUE - INTERVIEW SCHEDULING
   ========================================================= */

typedef struct {
    char ids[MAX_QUEUE][20];
    int front;
    int rear;
} Queue;

void initQueue(Queue *q) {
    q->front = -1;
    q->rear = -1;
}

int isQueueEmpty(Queue *q) {
    return q->front == -1;
}

int isQueueFull(Queue *q) {
    return q->rear == MAX_QUEUE - 1;
}

void enqueue(Queue *q, char id[]) {

    if (isQueueFull(q)) {
        printf("\nQueue is full!\n");
        return;
    }

    if (q->front == -1)
        q->front = 0;

    q->rear++;

    strcpy(q->ids[q->rear], id);

    printf("\nCandidate %s added to interview queue.\n", id);
}

void dequeue(Queue *q) {

    if (isQueueEmpty(q)) {
        printf("\nInterview queue is empty.\n");
        return;
    }

    printf("\nInterview scheduled for Candidate ID: %s\n",
           q->ids[q->front]);

    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
}

void displayQueue(Queue *q) {

    if (isQueueEmpty(q)) {
        printf("\nInterview queue is empty.\n");
        return;
    }

    printf("\n========== INTERVIEW QUEUE ==========\n");

    for (int i = q->front; i <= q->rear; i++) {
        printf("%d. Candidate ID: %s\n",
               i - q->front + 1,
               q->ids[i]);
    }
}


/* =========================================================
   2. CIRCULAR QUEUE - RECENT ACTIVITIES
   ========================================================= */

typedef struct {
    char activities[MAX_ACTIVITY][100];
    int front;
    int rear;
    int count;
} CircularQueue;

void initCircularQueue(CircularQueue *cq) {
    cq->front = 0;
    cq->rear = -1;
    cq->count = 0;
}

void addActivity(CircularQueue *cq, char activity[]) {

    cq->rear = (cq->rear + 1) % MAX_ACTIVITY;

    strcpy(cq->activities[cq->rear], activity);

    if (cq->count < MAX_ACTIVITY) {
        cq->count++;
    } else {
        cq->front = (cq->front + 1) % MAX_ACTIVITY;
    }
}

void displayActivities(CircularQueue *cq) {

    if (cq->count == 0) {
        printf("\nNo recent activities.\n");
        return;
    }

    printf("\n========== RECENT ACTIVITIES ==========\n");

    for (int i = 0; i < cq->count; i++) {

        int index = (cq->front + i) % MAX_ACTIVITY;

        printf("%d. %s\n",
               i + 1,
               cq->activities[index]);
    }
}


/* =========================================================
   3. SINGLY LINKED LIST - CANDIDATE DATABASE
   ========================================================= */

Candidate *head = NULL;

Candidate *findCandidate(char id[]) {

    Candidate *temp = head;

    while (temp != NULL) {

        if (strcmp(temp->id, id) == 0)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

void insertCandidate(Candidate data[]) {

    if (findCandidate(data->id) != NULL) {

        printf("\nERROR: Candidate ID already exists!\n");
        return;
    }

    Candidate *newNode =
        (Candidate *)malloc(sizeof(Candidate));

    if (newNode == NULL) {
        printf("\nMemory allocation failed!\n");
        return;
    }

    *newNode = *data;

    newNode->next = NULL;

    if (head == NULL) {
        head = newNode;
    } else {

        Candidate *temp = head;

        while (temp->next != NULL)
            temp = temp->next;

        temp->next = newNode;
    }

    printf("\nCandidate registered successfully!\n");
}

void deleteCandidate(char id[]) {

    Candidate *temp = head;
    Candidate *prev = NULL;

    while (temp != NULL &&
           strcmp(temp->id, id) != 0) {

        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {

        printf("\nCandidate not found.\n");
        return;
    }

    if (prev == NULL)
        head = temp->next;
    else
        prev->next = temp->next;

    free(temp);

    printf("\nCandidate deleted successfully.\n");
}

void updateStatus(char id[], char status[]) {

    Candidate *candidate = findCandidate(id);

    if (candidate == NULL) {

        printf("\nCandidate not found.\n");
        return;
    }

    strcpy(candidate->status, status);

    printf("\nCandidate status updated successfully.\n");
}

void displayCandidate(Candidate *c) {

    printf("\n---------------------------------------\n");

    printf("Candidate ID     : %s\n", c->id);
    printf("Name             : %s\n", c->name);
    printf("Email            : %s\n", c->email);
    printf("Phone            : %s\n", c->phone);
    printf("Position         : %s\n", c->position);
    printf("Experience       : %.1f years\n", c->experience);
    printf("Interview Status : %s\n", c->status);

    printf("---------------------------------------\n");
}

void displayAllCandidates() {

    Candidate *temp = head;

    if (temp == NULL) {

        printf("\nCandidate database is empty.\n");
        return;
    }

    printf("\n========== CANDIDATE DATABASE ==========\n");

    while (temp != NULL) {

        displayCandidate(temp);

        temp = temp->next;
    }
}


/* =========================================================
   4. HASH TABLE - FAST CANDIDATE SEARCH
   ========================================================= */

Candidate *hashTable[TABLE_SIZE];

int hashFunction(char id[]) {

    int hash = 0;

    for (int i = 0; id[i] != '\0'; i++) {

        hash = (hash * 31 + id[i])
               % TABLE_SIZE;
    }

    return hash;
}

void initHashTable() {

    for (int i = 0; i < TABLE_SIZE; i++)
        hashTable[i] = NULL;
}

void hashInsert(Candidate *candidate) {

    int index = hashFunction(candidate->id);

    /*
       Collision handling using chaining
    */

    Candidate *newNode =
        (Candidate *)malloc(sizeof(Candidate));

    if (newNode == NULL) {
        printf("\nHash memory allocation failed!\n");
        return;
    }

    *newNode = *candidate;

    newNode->next = hashTable[index];

    hashTable[index] = newNode;
}

Candidate *hashSearch(char id[]) {

    int index = hashFunction(id);

    Candidate *temp = hashTable[index];

    while (temp != NULL) {

        if (strcmp(temp->id, id) == 0)
            return temp;

        temp = temp->next;
    }

    return NULL;
}

void searchCandidateByHash() {

    char id[20];

    printf("\nEnter Candidate ID to search: ");
    scanf("%19s", id);

    Candidate *result = hashSearch(id);

    if (result == NULL) {

        printf("\nCandidate not found!\n");

    } else {

        printf("\n========== CANDIDATE FOUND ==========\n");

        displayCandidate(result);
    }
}


/* =========================================================
   5. BINARY SEARCH TREE
   INTERVIEW ROUND HIERARCHY
   ========================================================= */

typedef struct BSTNode {

    int round;
    char roundName[50];

    struct BSTNode *left;
    struct BSTNode *right;

} BSTNode;


/*
   Create a new BST node
*/

BSTNode *createBSTNode(int round, char name[]) {

    BSTNode *newNode =
        (BSTNode *)malloc(sizeof(BSTNode));

    if (newNode == NULL) {

        printf("\nMemory allocation failed!\n");
        exit(1);
    }

    newNode->round = round;

    strcpy(newNode->roundName, name);

    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}


/*
   Insert interview round into BST
*/

BSTNode *insertRound(BSTNode *root,
                     int round,
                     char name[]) {

    if (root == NULL)
        return createBSTNode(round, name);

    if (round < root->round) {

        root->left =
            insertRound(root->left,
                        round,
                        name);

    } else if (round > root->round) {

        root->right =
            insertRound(root->right,
                        round,
                        name);
    }

    return root;
}


/*
   Search for interview round
*/

BSTNode *searchRound(BSTNode *root, int round) {

    if (root == NULL ||
        root->round == round)

        return root;

    if (round < root->round)
        return searchRound(root->left, round);

    return searchRound(root->right, round);
}


/*
   Inorder traversal
*/

void inorderRounds(BSTNode *root) {

    if (root == NULL)
        return;

    inorderRounds(root->left);

    printf("Round %d : %s\n",
           root->round,
           root->roundName);

    inorderRounds(root->right);
}


/*
   Preorder traversal
*/

void preorderRounds(BSTNode *root) {

    if (root == NULL)
        return;

    printf("Round %d : %s\n",
           root->round,
           root->roundName);

    preorderRounds(root->left);
    preorderRounds(root->right);
}


/*
   Postorder traversal
*/

void postorderRounds(BSTNode *root) {

    if (root == NULL)
        return;

    postorderRounds(root->left);
    postorderRounds(root->right);

    printf("Round %d : %s\n",
           root->round,
           root->roundName);
}


/* =========================================================
   REGISTER CANDIDATE
   ========================================================= */

void registerNewCandidate(
        Queue *q,
        CircularQueue *cq) {

    Candidate newCandidate;

    printf("\n========== CANDIDATE REGISTRATION ==========\n");

    printf("Enter Candidate ID: ");
    scanf("%19s", newCandidate.id);

    if (findCandidate(newCandidate.id) != NULL) {

        printf("\nERROR: Candidate ID already exists!\n");
        return;
    }

    printf("Enter Name: ");
    scanf(" %[^\n]", newCandidate.name);

    printf("Enter Email: ");
    scanf("%59s", newCandidate.email);

    printf("Enter Phone: ");
    scanf("%19s", newCandidate.phone);

    printf("Enter Position Applied For: ");
    scanf(" %[^\n]", newCandidate.position);

    printf("Enter Years of Experience: ");
    scanf("%f", &newCandidate.experience);

    strcpy(newCandidate.status, "Registered");

    newCandidate.next = NULL;

    /*
       Insert into Linked List
    */

    insertCandidate(&newCandidate);

    /*
       Insert into Hash Table
    */

    Candidate *candidate =
        findCandidate(newCandidate.id);

    if (candidate != NULL)
        hashInsert(candidate);

    /*
       Add to interview queue
    */

    enqueue(q, newCandidate.id);

    /*
       Record activity
    */

    char activity[100];

    sprintf(activity,
            "Candidate %s registered",
            newCandidate.id);

    addActivity(cq, activity);

    printf("\nRegistration completed successfully!\n");
}


/* =========================================================
   SCHEDULE INTERVIEW
   ========================================================= */

void scheduleInterview(
        Queue *q,
        CircularQueue *cq) {

    if (isQueueEmpty(q)) {

        printf("\nNo candidates waiting for interview.\n");
        return;
    }

    char id[20];

    strcpy(id, q->ids[q->front]);

    dequeue(q);

    Candidate *candidate =
        findCandidate(id);

    if (candidate != NULL) {

        strcpy(candidate->status,
               "Round 1");

        /*
           Update corresponding hash record
        */

        Candidate *hashCandidate =
            hashSearch(id);

        if (hashCandidate != NULL)
            strcpy(hashCandidate->status,
                   "Round 1");
    }

    char activity[100];

    sprintf(activity,
            "Interview scheduled for %s",
            id);

    addActivity(cq, activity);
}


/* =========================================================
   SEARCH ROUND
   ========================================================= */

void searchInterviewRound(BSTNode *root) {

    int round;

    printf("\nEnter Interview Round Number: ");
    scanf("%d", &round);

    BSTNode *result =
        searchRound(root, round);

    if (result == NULL) {

        printf("\nInterview round not found.\n");

    } else {

        printf("\nInterview Round Found\n");
        printf("Round Number : %d\n",
               result->round);

        printf("Round Name   : %s\n",
               result->roundName);
    }
}


/* =========================================================
   DISPLAY BST
   ========================================================= */

void displayInterviewHierarchy(BSTNode *root) {

    if (root == NULL) {

        printf("\nInterview hierarchy is empty.\n");
        return;
    }

    printf("\n========== INTERVIEW ROUND HIERARCHY ==========\n");

    printf("\nInorder Traversal:\n");

    inorderRounds(root);
}


/* =========================================================
   MAIN FUNCTION
   ========================================================= */

int main() {

    Queue interviewQueue;

    CircularQueue activityQueue;

    BSTNode *roundRoot = NULL;

    int choice;

    /*
       Initialize structures
    */

    initQueue(&interviewQueue);

    initCircularQueue(&activityQueue);

    initHashTable();


    /*
       Create Interview Round Hierarchy
    */

    roundRoot =
        insertRound(roundRoot,
                    3,
                    "Final Interview");

    roundRoot =
        insertRound(roundRoot,
                    1,
                    "Technical Round");

    roundRoot =
        insertRound(roundRoot,
                    2,
                    "HR Round");

    roundRoot =
        insertRound(roundRoot,
                    4,
                    "Management Round");


    /*
       MAIN MENU
    */

    do {

        printf("\n\n");
        printf("============================================\n");
        printf(" AI INTERVIEW SCHEDULER & RECRUITMENT\n");
        printf(" MANAGEMENT SYSTEM\n");
        printf("============================================\n");

        printf("\n1. Register Candidate");
        printf("\n2. Display Candidates");
        printf("\n3. Delete Candidate");
        printf("\n4. Update Candidate Status");
        printf("\n5. Search Candidate by ID");
        printf("\n6. Display Interview Queue");
        printf("\n7. Schedule Next Interview");
        printf("\n8. Display Recent Activities");
        printf("\n9. Display Interview Round Hierarchy");
        printf("\n10. Search Interview Round");
        printf("\n11. Add New Interview Round");
        printf("\n12. Display BST Traversals");
        printf("\n0. Exit");

        printf("\n\nEnter your choice: ");
        scanf("%d", &choice);


        switch (choice) {


            /* -------------------------------------
               REGISTER CANDIDATE
               ------------------------------------- */

            case 1:

                registerNewCandidate(
                    &interviewQueue,
                    &activityQueue);

                break;


            /* -------------------------------------
               DISPLAY CANDIDATES
               ------------------------------------- */

            case 2:

                displayAllCandidates();

                break;


            /* -------------------------------------
               DELETE CANDIDATE
               ------------------------------------- */

            case 3: {

                char id[20];

                printf("\nEnter Candidate ID to delete: ");
                scanf("%19s", id);

                deleteCandidate(id);

                break;
            }


            /* -------------------------------------
               UPDATE STATUS
               ------------------------------------- */

            case 4: {

                char id[20];
                char status[30];

                printf("\nEnter Candidate ID: ");
                scanf("%19s", id);

                printf("Enter New Status: ");
                scanf(" %[^\n]", status);

                updateStatus(id, status);

                /*
                   Also update hash table record
                */

                Candidate *hashCandidate =
                    hashSearch(id);

                if (hashCandidate != NULL)
                    strcpy(hashCandidate->status,
                           status);

                break;
            }


            /* -------------------------------------
               HASH TABLE SEARCH
               ------------------------------------- */

            case 5:

                searchCandidateByHash();

                break;


            /* -------------------------------------
               DISPLAY QUEUE
               ------------------------------------- */

            case 6:

                displayQueue(&interviewQueue);

                break;


            /* -------------------------------------
               SCHEDULE INTERVIEW
               ------------------------------------- */

            case 7:

                scheduleInterview(
                    &interviewQueue,
                    &activityQueue);

                break;


            /* -------------------------------------
               RECENT ACTIVITIES
               ------------------------------------- */

            case 8:

                displayActivities(&activityQueue);

                break;


            /* -------------------------------------
               DISPLAY BST
               ------------------------------------- */

            case 9:

                displayInterviewHierarchy(roundRoot);

                break;


            /* -------------------------------------
               SEARCH INTERVIEW ROUND
               ------------------------------------- */

            case 10:

                searchInterviewRound(roundRoot);

                break;


            /* -------------------------------------
               ADD INTERVIEW ROUND
               ------------------------------------- */

            case 11: {

                int round;
                char name[50];

                printf("\nEnter Round Number: ");
                scanf("%d", &round);

                printf("Enter Round Name: ");
                scanf(" %[^\n]", name);

                if (searchRound(roundRoot, round)
                    != NULL) {

                    printf("\nRound already exists!\n");

                } else {

                    roundRoot =
                        insertRound(
                            roundRoot,
                            round,
                            name);

                    printf("\nInterview round added successfully!\n");
                }

                break;
            }


            /* -------------------------------------
               BST TRAVERSALS
               ------------------------------------- */

            case 12:

                printf("\n========== BST TRAVERSALS ==========\n");

                printf("\nInorder:\n");
                inorderRounds(roundRoot);

                printf("\nPreorder:\n");
                preorderRounds(roundRoot);

                printf("\nPostorder:\n");
                postorderRounds(roundRoot);

                break;


            case 0:

                printf("\nExiting system...\n");
                printf("Thank you!\n");

                break;


            default:

                printf("\nInvalid choice! Try again.\n");
        }

    } while (choice != 0);


    return 0;
}
