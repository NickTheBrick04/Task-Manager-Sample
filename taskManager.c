#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct task {
    char name[100];
    struct task* next;
} Task;
typedef struct {
    Task* front;
    Task* rear;
} TaskQueue;
typedef struct {
    Task* top;
} TaskStack;
//Queue operations
void enqueue(TaskQueue* q, const char* taskName) {
    Task* newTask = malloc(sizeof(Task));
    if (newTask == NULL) {
        printf("Memory allocation for new task failed.\n");
        return;
    }
    strncpy(newTask->name, taskName, sizeof(newTask->name) - 1);
    newTask->name[sizeof(newTask->name) - 1] = '\0';
    if (q->front == NULL) {
        printf("Queue is empty, adding to front.\n");
        q->front = newTask;
        q->rear = newTask;
        newTask->next = newTask;
    } else {
        newTask->next = q->front;
        q->rear->next = newTask;
        q->rear = newTask;
    }
}
char* dequeue(TaskQueue* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return NULL;
    }
    char* taskName = malloc(strlen(q->front->name) + 1);
    if (taskName == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    strcpy(taskName, q->front->name);
    if (q->front == q->rear) {
        free(q->front);
        q->front = NULL;
        q->rear = NULL;
    } else {
        Task* temp = q->front;
        q->front = q->front->next;
        q->rear->next = q->front;
        free(temp);
    }
    return taskName;
}
void displayQueue(const TaskQueue* q) {
    if (q->front == NULL) {
        printf("The queue is empty\n");
        return;
    }
    Task* temp = q->front;
    do {
        printf("%s -> ", temp->name);
        temp = temp->next;
    } while (temp != q->front);  // Stop when we complete the loop
    printf("...\n");
}
//Stack operations
void push(TaskStack* s, const char* taskName) {
    Task* newTask = malloc(sizeof(Task));
    if (newTask == NULL) {
        printf("Memory allocation for new task failed.\n");
        return;
    }
    strncpy(newTask->name, taskName, sizeof(newTask->name) - 1);
    newTask->name[sizeof(newTask->name) - 1] = '\0';
    newTask->next=s->top;
    s->top = newTask;
}
char* pop(TaskStack* s) {
    if (s->top == NULL) {
        printf("The stack is empty\n");
        return NULL;
    }
    char* taskName = strdup(s->top->name);
    if (taskName == NULL) {
        printf("Memory allocation failed in pop.\n");
        return NULL;
    }
    Task* temp = s->top;
    s->top = s->top->next;
    free(temp);
    return taskName;
}
void displayStack(const TaskStack* s) {
    Task* temp = s->top;
    if (temp == NULL) {
        printf("The stack is empty\n");
        return;
    }
    while (temp != NULL) {
        printf("%s -> ", temp->name);
        temp = temp->next;
    }
    printf("NULL\n");
}
void unenqueue(TaskQueue* q) {
    if (q->front == NULL) return;
    if (q->front == q->rear) {
        free(q->front);
        q->front = q->rear = NULL;
        return;
    }
    Task* cur = q->front;
    while (cur->next != q->rear) {
        cur = cur->next;
    }
    free(q->rear);
    q->rear = cur;
    q->rear->next = q->front;
}
void undequeue(TaskQueue* q, const char* taskName) {
    enqueue(q, taskName);
}
void addTask(TaskQueue *q, TaskStack *undoStack, const char *taskName) {
    enqueue(q, taskName);
    size_t len = strlen("Add:") + strlen(taskName) + 1;
    char* temp = malloc(len);
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    strcpy(temp, "Add:");
    strcat(temp, taskName);
    push(undoStack, temp);
    printf("Added task: %s\n", taskName);
}
void processTask(TaskQueue* q, TaskStack* undoStack) {
    char* taskName = dequeue(q);
    if (taskName == NULL) {
        printf("No task to process.\n");
        return;
    }
    size_t len = strlen("Add:") + strlen(taskName) + 1;
    char* temp = malloc(len);
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    strcpy(temp, "Process:");
    strcat(temp, taskName);
    push(undoStack, temp);
    printf("Processed task: %s\n", taskName);
}
void undo(TaskQueue* q, TaskStack* undoStack, TaskStack* redoStack) {
    char* action = pop(undoStack);
    if (action == NULL) {
        printf("Nothing to undo.\n");
        return;
    }
    if (strncmp(action, "Add:", 4) == 0) {
        unenqueue(q);
        printf("Undo: Removed task added: %s\n", action + 4);
    } else if (strncmp(action, "Process:", 8) == 0) {
        undequeue(q, action + 8);
        printf("Undo: Unprocessed task: %s\n", action + 8);
    } else {
        printf("Undo: Unknown action: %s\n", action);
    }
    push(redoStack, action);
    free(action);
}
void redo(TaskQueue* q, TaskStack* undoStack, TaskStack* redoStack) {
    char* action = pop(redoStack);
    if (action == NULL) {
        printf("Nothing to redo.\n");
        return;
    }
    if (strncmp(action, "Add:", 4) == 0) {
        enqueue(q, action + 4);
        printf("Redo: Re-added task: %s\n", action + 4);
    }
    else if (strncmp(action, "Process:", 8) == 0) {
        const char* taskToRemove = action + 8;
        Task* prev = q->rear;
        Task* curr = q->front;
        int found = 0;
        do {
            if (strcmp(curr->name, taskToRemove) == 0) {
                found = 1;
                if (curr == q->front && curr == q->rear) {
                    q->front = q->rear = NULL;
                } else if (curr == q->front) {
                    q->front = curr->next;
                    q->rear->next = q->front;
                } else if (curr == q->rear) {
                    q->rear = prev;
                    q->rear->next = q->front;
                } else {
                    prev->next = curr->next;
                }
                free(curr);
                printf("Redo: Re-processed task: %s\n", taskToRemove);
                break;
            }
            prev = curr;
            curr = curr->next;
        } while (curr != q->front);
        if (!found) {
            printf("Redo: Task '%s' not found in queue.\n", taskToRemove);
        }
    }
    else {
        printf("Redo: Unknown action: %s\n", action);
    }
    push(undoStack, action);
    free(action);
}
int main() {
    TaskQueue queue = {NULL, NULL};
    TaskStack undoStack = {NULL};
    TaskStack redoStack = {NULL};
    addTask(&queue, &undoStack, "Email Client");
    addTask(&queue, &undoStack, "Compile Report");
    displayQueue(&queue);
    displayStack(&undoStack);
    displayStack(&redoStack);
    processTask(&queue, &undoStack);
    displayQueue(&queue);
    displayStack(&undoStack);
    displayStack(&redoStack);
    undo(&queue, &undoStack, &redoStack);
    displayQueue(&queue);
    displayStack(&undoStack);
    displayStack(&redoStack);
    redo(&queue, &undoStack, &redoStack);
    displayQueue(&queue);
    displayStack(&undoStack);
    displayStack(&redoStack);
    return 0;
}