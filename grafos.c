#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct AdjListNode {
    int dest;
    struct AdjListNode* next;
};

struct AdjList {
    struct AdjListNode *head;
};

struct Grafo {
    int V;
    struct AdjList* array;
};

struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* array;
};

struct AdjListNode* newAdjListNode(int dest) {
    struct AdjListNode* newNode = (struct AdjListNode*) malloc(sizeof(struct AdjListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

struct Grafo* createGraph(int V) {
    struct Grafo* graph = (struct Grafo*) malloc(sizeof(struct Grafo));
    graph->V = V;

    graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(struct Grafo* graph, int src, int dest) {
    struct AdjListNode* newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;

    newNode = newAdjListNode(src);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

void DFSUtil(int v, int visited[], struct Grafo* graph, int preOrder[], int postOrder[], int *preIndex, int *postIndex) {
    visited[v] = 1;
    preOrder[(*preIndex)++] = v;

    struct AdjListNode* temp = graph->array[v].head;
    while (temp) {
        int adjVertex = temp->dest;
        if (!visited[adjVertex])
            DFSUtil(adjVertex, visited, graph, preOrder, postOrder, preIndex, postIndex);
        temp = temp->next;
    }

    postOrder[(*postIndex)++] = v;
}

void DFS(struct Grafo* graph, int startVertex) {
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++)
        visited[i] = 0;

    int preOrder[graph->V], postOrder[graph->V];
    int preIndex = 0, postIndex = 0;

    DFSUtil(startVertex, visited, graph, preOrder, postOrder, &preIndex, &postIndex);

    printf("DFS Pré-ordem:\n");
    for (int i = 0; i < preIndex; i++)
        printf("%d ", preOrder[i]);
    printf("\n");

    printf("DFS Pós-ordem:\n");
    for (int i = 0; i < postIndex; i++)
        printf("%d ", postOrder[i]);
    printf("\n");
}

struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

int isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

void enqueue(struct Queue* queue, int item) {
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

int dequeue(struct Queue* queue) {
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

void BFS(struct Grafo* graph, int startVertex) {
    int visited[graph->V];
    for (int i = 0; i < graph->V; i++)
        visited[i] = 0;

    struct Queue* queue = createQueue(graph->V);

    visited[startVertex] = 1;
    enqueue(queue, startVertex);

    while (!isEmpty(queue)) {
        int currentVertex = dequeue(queue);
        printf("%d ", currentVertex);

        struct AdjListNode* temp = graph->array[currentVertex].head;
        while (temp) {
            int adjVertex = temp->dest;

            if (!visited[adjVertex]) {
                visited[adjVertex] = 1;
                enqueue(queue, adjVertex);
            }
            temp = temp->next;
        }
    }
}

int main() {
    int V, E;
    printf("Digite o número de vértices: ");
    scanf("%d", &V);
    struct Grafo* graph = createGraph(V);

    printf("Digite o número de arestas: ");
    scanf("%d", &E);
    printf("Digite as arestas no formato 'u v', onde u e v são os vértices conectados:\n");
    for (int i = 0; i < E; i++) {
        int u, v;
        scanf("%d %d", &u, &v);
        addEdge(graph, u, v);
    }

    int startVertex, choice;
    printf("Digite 1 para BFS ou 2 para DFS: ");
    scanf("%d", &choice);
    printf("Digite o vértice de início: ");
    scanf("%d", &startVertex);

    if (choice == 1) {
        printf("BFS a partir do vértice %d:\n", startVertex);
        BFS(graph, startVertex);
    } else if (choice == 2) {
        printf("DFS a partir do vértice %d:\n", startVertex);
        DFS(graph, startVertex);
    } else {
        printf("Escolha inválida.\n");
    }

    return 0;
}