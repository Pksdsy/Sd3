#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	monday, tuesday, wednesday, thursday, friday, saturday, sunday
} weekday;

typedef struct{
	int year;
	int month;
	int day;
	int hour;
	int min;
	weekday day_of_week;
	char description[500];
	char place[100];
	int importance;
} event;


typedef struct {
	struct Tree* Left;
	struct Tree* Right;
	struct Tree* parent;
	event* Event;
} tree;


void print_day_of_week(weekday day){
	switch(day){
	case monday:
		printf("Monday\t");
		break;
	case tuesday:
		printf("Tuesday\t");
		break;
	case wednesday:
		printf("Wednesday\t");
		break;
	case thursday:
		printf("Thursday\t");
		break;
	case friday:
		printf("Friday\t");
		break;
	case saturday:
		printf("Saturday\t");
		break;
	case sunday:
		printf("Sunday\t");
		break;
	default:
		printf("Invalid day\t");
		break;
	}
}


void printTree(tree* Tree) {
    if (Tree == NULL) {
        return;
    }
    printTree(Tree->Left);
	printf("Date: %d.%d.%d\t", Tree->Event->day, Tree->Event->month, Tree->Event->year, Tree->Event->min, Tree->Event->hour);
	print_day_of_week(Tree->Event->day_of_week);
	printf("time: %d:%d\n", Tree->Event->hour, Tree->Event->min);
	printf("Description: %s\n", Tree->Event->description);
	printf("Place: %s\n", Tree->Event->place);
	printf("Importance: %d\n\n", Tree->Event->importance);
	printTree(Tree->Right);
}



int zeller(int year, int month, int day) {/*Zeller's algoritm for finding the day of the week*/
    if (month < 3) {
        month += 12;
        year--;
    }

    int week_day = (day + 13*(month + 1)/5 + year + year/4 - year/100 + year/400) % 7;

    return week_day;
}


void fill_event(event *new_event){/*Here we filling new event*/
    printf("\nPlease enter the event details:\n");
	int year, month, day, hour, minute, importance;

	while (1){
		printf("Year: ");
		scanf("%d", &year);
		if (year > 0){
			new_event->year = year;
			break;
		}else{
			puts("Incorrect data, please try again!");
		}
	}
    
	while (1){
		printf("Month: ");
		scanf("%d", &month);
		if (month > 0 && month <= 12){
			new_event->month = month;
			break;
		}else{
			puts("Incorrect data, please try again!");
		}
	}

    while (1){
		printf("Day: ");
		scanf("%d", &day);
		if (day > 0 && day <= 31){
			new_event->day = day;
			break;
		}else{
			puts("Incorrect data, please try again!");
		}
	}
	
	int week_day = zeller(year, month, day);/*Zeller's algoritm for finding the day of the week*/
	switch(week_day){
	case 0:
		new_event->day_of_week = saturday;
		break;
	case 1:
		new_event->day_of_week = sunday;
		break;
	case 2:
		new_event->day_of_week = monday;
		break;
	case 3:
		new_event->day_of_week = tuesday;
		break;
	case 4:
		new_event->day_of_week = wednesday;
		break;
	case 5:
		new_event->day_of_week = thursday;
		break;
	case 6:
		new_event->day_of_week = friday;
		break;
	default:
		puts("Happened some mistakes!");
	}

	while (1){
		printf("Hour: ");
		scanf("%d", &hour);
		if (hour >= 0 && hour < 24){
			new_event->hour = hour;
			break;
		}else{
			puts("Incorrect data, please try again!");
		}
	}

    while (1){
		printf("Minute: ");
		scanf("%d", &minute);
		getchar();
		if (minute >= 0 && minute < 60){
			new_event->min = minute;
			break;
		}else{
			puts("Incorrect data, please try again!");
		}
	}


    printf("Description: ");
	fgets(new_event->description, 500, stdin);
	new_event->description[strcspn(new_event->description, "\n")] = '\0';

    printf("Place: ");
	fgets(new_event->place, 100, stdin);
	new_event->place[strcspn(new_event->place, "\n")] = '\0';

    while (1){
		printf("Importance(1-10): ");
		scanf("%d", &importance);
		if (importance >= 0 && importance <= 10){
			new_event->importance = importance;
			break;
		}else{
			puts("Incorrect data, please try again!");
		}
	}
}



void add_event(tree** Tree) {
	event* new_event = (event*)malloc(sizeof(event));
    fill_event(new_event);
    if (*Tree == NULL) {
        *Tree = (tree*)malloc(sizeof(tree));
        (*Tree)->Event = new_event;
        (*Tree)->Left = NULL;
        (*Tree)->Right = NULL;
        (*Tree)->parent = NULL;
    } else {
		long int time_event = new_event->year*364 + new_event->month*30 + new_event->day + new_event->hour*60 + new_event->min;
        tree* root2 = *Tree, *root3 = NULL;
		tree* new_tree = (tree*)malloc(sizeof(tree));
		long int time_tree;
        while (root2 != NULL) {
            root3 = root2;
			time_tree = root2->Event->year*364 + root2->Event->month*30 + root2->Event->day + root2->Event->hour*60 + root2->Event->min;
			if (time_event < time_tree){
				root2 = root2->Left;
			}else {
				root2 = root2->Right;
			}
        }
        new_tree->Event = new_event;
        new_tree->Left = NULL;
        new_tree->Right = NULL;
        new_tree->parent = root3;
		time_tree = root3->Event->year*364 + root3->Event->month*30 + root3->Event->day + root3->Event->hour*60 + root3->Event->min;
		if (time_event < time_tree){
			root3->Left = new_tree;
		} else{
			root3->Right = new_tree;
		}
    }
}


tree *search_tree(tree *Tree, int time){
	long int time_tree = Tree->Event->year*364 + Tree->Event->month*30 + Tree->Event->day + Tree->Event->hour*60 + Tree->Event->min;

	if ((Tree == NULL) || (time == time_tree)){
		return Tree;
	}
	if (time < time_tree){
		return search_tree(Tree->Left, time);
	}else {
		return search_tree(Tree->Right, time);
	}
}


tree *min(tree *Tree){
	tree *current = Tree;
	while (current->Left != NULL){
		current = current ->Left;
	}
	return current;
}


tree *succ(tree *Tree){
	tree *root1 = Tree, *root2 = NULL;
	if (root1->Right != NULL){
		return min(root1->Right);
	}
	root2 = root1->parent;
	while ((root2 != NULL) && (root1 == root2->Right)){
		root1 = root2;
		root2 = root2->parent;
	}
	return root2;
}



void delete_event(tree** Tree, long int time){
	if (*Tree == NULL){
		puts("Error!!! Empty tree.");
		return;
	}
	tree *root1 = (*Tree), *root2 = NULL, *root3 = NULL;
	root2 = search_tree(*Tree, time);
	if ((root2->Left == NULL) && (root2->Right == NULL)){
		root3 = root2->parent;
		if (root2 == root3->Right){
			root3->Right = NULL;
		}else {
			root3->Left = NULL;
		}
		free(root2->Event);
		free(root2);
	}

	if ((root2->Left == NULL) && (root2->Right != NULL)){
		root3 = root2->parent;
		if (root2 == root3->Right){
			root3->Right = root2->Right;
		}else{
			root3->Left = root2->Right;
		}
		free(root2->Event);
		free(root2);
	}

	if ((root2->Left != NULL) && (root2->Right == NULL)){
		root3 = root2->parent;
		if (root2 == root3->Right){
			root3->Right = root2->Left;
		}else{
			root3->Left = root2->Left;
		}
		free(root2->Event);
		free(root2);
	}

	if ((root2->Left != NULL) && (root2->Right != NULL)){
		root3 = succ(root2);
		free(root2->Event);
		root2->Event = root3->Event;
		tree *parent_root3 = root3->parent;
		if (root3->Right == NULL){
			
			parent_root3->Left = NULL;
		} else{
			parent_root3->Left = root3->Right;
		}
		free(root3->Event);
		free(root3);
	}
}

void input_date(int *year, int *month, int *day, int *hour, int *min){
    printf("Input date (format dd.mm.yyyy hh:mm): ");
    scanf("%d.%d.%d %d:%d", day, month, year, hour, min);
}

int main(){
	int choice;
	tree* tree_for_day = NULL;
	long int time; 
	int year, month, day, hour, min;
	char date[20];

	while (1){
		puts("1)Add new event\n2)Delete event\n3)Print events\n4)Import events from file\n5)Exit");
		scanf("%d", &choice);
		switch(choice){
		case 1: 
			add_event(&tree_for_day);
			break;
		case 2:
			input_date(&year, &month, &day, &hour, &min);
			printf("%d %d %d %d %d\n", year, month, day, hour, min);
			time = year*364 + month*30 + day + hour*60 + min;
			delete_event(&tree_for_day, time);
			break;
		case 3:
			printTree(tree_for_day);
			break;
		case 4:
			puts("Import");
			break;
		case 5:
			puts("Goodbye!");
			exit(0);
		default:
			puts("Incorrect data, please try again!");
		}
		printf("\n");
	}

	return 0;
}