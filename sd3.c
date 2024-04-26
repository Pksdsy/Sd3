#include <stdio.h>
#include <stdlib.h>

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int min;
	enum weekday {
		monday, tuesday, wendnesday, thursday, friday, saturday, sunday
	};
	char description[500];
	char place[100];
	int importance;
} event;


typedef struct  {
	struct Tree* Left;
	struct Tree* Right;
	event* Event;
	int height;
} tree;

void printTree(tree* Tree) {
	if (Tree != NULL) {
		printTree((tree*)(Tree->Left));
		printf("%s\n", Tree->Event->description);
		printTree((tree*)(Tree->Right));
	}
}

int Height(tree* Tree) {
	if (Tree) {
		return Tree->height;
	}
	else return 0;
}

