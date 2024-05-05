#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum{
	monday, tuesday, wednesday, thursday, friday, saturday, sunday
} weekday;

typedef struct {
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

//23.12.2005 11:21
typedef struct {
	struct tree* Left;
	struct tree* Right;
	struct tree* parent;
	event* Event;
} tree;


void print_day_of_week(weekday day) {
	switch (day) {
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

	int week_day = (day + 13 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;

	return week_day;
}


void fill_event(event* new_event) {/*Here we filling new event*/
	printf("\nPlease enter the event details:\n");
	int year, month, day, hour, minute, importance;

	while (1) {
		printf("Year: ");
		scanf("%d", &year);
		if (year > 0) {
			new_event->year = year;
			break;
		}
		else {
			puts("Incorrect data, please try again!");
		}
	}

	while (1) {
		printf("Month: ");
		scanf("%d", &month);
		if (month > 0 && month <= 12) {
			new_event->month = month;
			break;
		}
		else {
			puts("Incorrect data, please try again!");
		}
	}

	while (1) {
		printf("Day: ");
		scanf("%d", &day);
		if (day > 0 && day <= 31) {
			new_event->day = day;
			break;
		}
		else {
			puts("Incorrect data, please try again!");
		}
	}

	int week_day = zeller(year, month, day);/*Zeller's algoritm for finding the day of the week*/
	switch (week_day) {
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

	while (1) {
		printf("Hour: ");
		scanf("%d", &hour);
		if (hour >= 0 && hour < 24) {
			new_event->hour = hour;
			break;
		}
		else {
			puts("Incorrect data, please try again!");
		}
	}

	while (1) {
		printf("Minute: ");
		scanf("%d", &minute);
		getchar();
		if (minute >= 0 && minute < 60) {
			new_event->min = minute;
			break;
		}
		else {
			puts("Incorrect data, please try again!");
		}
	}


	printf("Description: ");
	fgets(new_event->description, 500, stdin);
	new_event->description[strcspn(new_event->description, "\n")] = '\0';

	printf("Place: ");
	fgets(new_event->place, 100, stdin);
	new_event->place[strcspn(new_event->place, "\n")] = '\0';

	while (1) {
		printf("Importance(1-10): ");
		scanf("%d", &importance);
		if (importance >= 0 && importance <= 10) {
			new_event->importance = importance;
			break;
		}
		else {
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
	}
	else {
		long int time_event = new_event->year * 364 + new_event->month * 30 + new_event->day + new_event->hour * 60 + new_event->min;
		tree* root2 = *Tree, * root3 = NULL;
		tree* new_tree = (tree*)malloc(sizeof(tree));
		long int time_tree;
		while (root2 != NULL) {
			root3 = root2;
			time_tree = root2->Event->year * 364 + root2->Event->month * 30 + root2->Event->day + root2->Event->hour * 60 + root2->Event->min;
			if (time_event < time_tree) {
				root2 = root2->Left;
			}
			else {
				root2 = root2->Right;
			}
		}
		new_tree->Event = new_event;
		new_tree->Left = NULL;
		new_tree->Right = NULL;
		new_tree->parent = root3;
		time_tree = root3->Event->year * 364 + root3->Event->month * 30 + root3->Event->day + root3->Event->hour * 60 + root3->Event->min;
		if (time_event < time_tree) {
			root3->Left = new_tree;
		}
		else {
			root3->Right = new_tree;
		}
	}
}

void toRoot(tree** Tree, event* Event) {
	while ((*Tree)->parent != NULL) { //���� ����������� ����������. ����������, ����� �������� ��������� root-�, ��� ��� ��������� ���������� � ����������.
		tree* root1 = (*Tree)->parent, * root2 = *Tree;
		if (root1->Left == root2 && root1->Right != NULL && root2->Right!=NULL && root2->Left!=NULL) {//����� �������
			root2->parent = root1->parent;
			tree* parent = root1->parent;
			if (parent->Left == root1) {
				parent->Left = root2;
			}
			else {
				parent->Right = root2;
			}
			tree* root3 = root2->Right;
			root3->parent = root1;
			root1->Left = root3;
			root2->Right = root1;
			root1->parent = root2;
			

			(*Tree) = root1; //�� �����?
		}
		else if (root1->Right == root2 && root1->Left != NULL && root2->Right != NULL && root2->Left != NULL){
			root2->parent = root1->parent;
			tree* root3 = root2->Left;
			root3->parent = root1;
			root1->Right = root3;
			root2->Left = root1;
			root1->parent = root2;

			(*Tree) = root1; //���������� �� �����?
		}
		else if (root1->Right==root2 && root2->Left==NULL && root2->Right==NULL&&root1->Right==NULL) {
			root2->parent = root1->parent;
			root2->Right = root1;
			tree* parent = root1->parent;
			if (parent->Left == root1) {
				parent->Left = root2;
			}
			/*else if (parent->){
				
			}*/
		}

	}
}

void treevst(tree** Tree, event* Event ,int mode) { //�������� ��� ����� ����� ��� event**
	if (mode == 1) {
		if ((*Tree)->Event == NULL) {
			*Tree = (tree*)malloc(sizeof(tree));
			(*Tree)->Event = Event;
			(*Tree)->Left = NULL;
			(*Tree)->Right = NULL;
			(*Tree)->parent = NULL;
		} else if ((*Tree)->Right == NULL && ((*Tree)->Event->importance <= Event->importance)) {
			tree* node;
			node = (tree*)malloc(sizeof(tree));
			node->Event = Event;
			node->Left = NULL;
			node->Right = NULL;
			node->parent = (*Tree);
			(*Tree)->Right = node;
			//toRoot(Tree, Event);
			return;
		}
		else if ((*Tree)->Left == NULL && ((*Tree)->Event->importance > Event->importance)) {
			tree* node;
			node = (tree*)malloc(sizeof(tree));
			node->Event = Event;
			node->Left = NULL;
			node->Right = NULL;
			node->parent = (*Tree);
			(*Tree)->Left = node;
			//toRoot(Tree, Event);
			return;
		}
		else if ((*Tree)->Event->importance <= Event->importance) {
			treevst(&((*Tree)->Right), Event, 1);
		}
		else {
			treevst(&((*Tree)->Left), Event, 1);
		}
	}
	else { //mode 2
		if ((*Tree)->Event != NULL) {
			(*Tree)->Event->place[0]=tolower((*Tree)->Event->place[0]);
			Event->place[0]=tolower(Event->place[0]);
		}
		if ((*Tree)->Event == NULL) {
			*Tree = (tree*)malloc(sizeof(tree));
			(*Tree)->Event = Event;
			(*Tree)->Left = NULL;
			(*Tree)->Right = NULL;
			(*Tree)->parent = NULL;
		}
		else if ((*Tree)->Right == NULL && (strcmp((*Tree)->Event->place, Event->place) <= 0)) {
			tree* node;
			node = (tree*)malloc(sizeof(tree));
			node->Event = Event;
			node->Left = NULL;
			node->Right = NULL;
			node->parent = (*Tree);
			(*Tree)->Right = node;
			//toRoot(Tree, Event);
			return;
		}
		else if ((*Tree)->Left == NULL && (strcmp((*Tree)->Event->place, Event->place) > 0)) {
			tree* node;
			node = (tree*)malloc(sizeof(tree));
			node->Event = Event;
			node->Left = NULL;
			node->Right = NULL;
			node->parent = (*Tree);
			(*Tree)->Left = node;
			//toRoot(Tree, Event);
			return;
		}
		else if (strcmp((*Tree)->Event->place, Event->place) <= 0) {
			treevst(&((*Tree)->Right), Event, 2);
		}
		else {
			treevst(&((*Tree)->Left), Event, 2);
		}
	}

	//������, ������� �� ��������. ������ ����� ������� ��� ������ � ������� ��������.
}

void treeobh(tree** Tree, int mode, tree** Treeout) { //1 - imp, 2 - place
	if ((*Tree) == NULL) {
		return;
	}
	treeobh(&((*Tree)->Left), mode, Treeout);
	treevst(Treeout, (*Tree)->Event, mode);
	treeobh(&((*Tree)->Right), mode, Treeout);
}

void printTreeImp(tree** Tree) {
	//������� ������, � ��� ������� �������� �������� ������� � ����� ������ � ����� ������, � ����� �������

	tree* Tree1;
	Tree1 = (tree*)malloc(sizeof(tree));
	event* Event = (event*)malloc(sizeof(event));
	Event = NULL;

	Tree1->Left=NULL;
	Tree1->Right=NULL;
	Tree1->parent=NULL;
	Tree1->Event = Event;

	treeobh(Tree, 1, &Tree1);

	printTree(Tree1); //�������� ��� ���� ����� ������� � �������� �������
}

void printTreePlace(tree* Tree) {
	//������� ������, � ��� ������� �������� �������� ������� � ����� ������ � ����� ������, � ����� �������
	tree* Tree1;
	Tree1 = (tree*)malloc(sizeof(tree));
	event* Event = (event*)malloc(sizeof(event));
	Event = NULL;

	Tree1->Left = NULL;
	Tree1->Right = NULL;
	Tree1->parent = NULL;
	Tree1->Event = Event;
	treeobh(Tree, 2, &Tree1);

	printTree(Tree1); //�������� ��� ���� ����� ������� � �������� �������
}

tree* search_tree(tree* Tree, int time) {
	long int time_tree = Tree->Event->year * 364 + Tree->Event->month * 30 + Tree->Event->day + Tree->Event->hour * 60 + Tree->Event->min;

	if ((Tree == NULL) || (time == time_tree)) {
		return Tree;
	}
	if (time < time_tree) {
		return search_tree(Tree->Left, time);
	}
	else {
		return search_tree(Tree->Right, time);
	}
}


tree* minTree(tree* Tree) {
	tree* current = Tree;
	while (current->Left != NULL) {
		current = current->Left;
	}
	return current;
}


tree* succ(tree* Tree) {
	tree* root1 = Tree, * root2 = NULL;
	if (root1->Right != NULL) {
		return minTree(root1->Right);
	}
	root2 = root1->parent;
	while ((root2 != NULL) && (root1 == root2->Right)) {
		root1 = root2;
		root2 = root2->parent;
	}
	return root2;
}



void delete_event(tree** Tree, long int time) {
	if (*Tree == NULL) {
		puts("Error!!! Empty tree.");
		return;
	}
	tree* root1 = (*Tree), * root2 = NULL, * root3 = NULL;
	root2 = search_tree(*Tree, time);
	if (root2->parent == NULL) {
		if (root2->Right != NULL && root2->Left != NULL) {
			//root1 = root2->Right;
			//root3 = root2->Left;
			//int timeroot1, timeroot2;
			//timeroot1 = root1->Event->year * 365 + root1->Event->month * 30 + root1->Event->day + root1->Event->hour * 60 + root1->Event->min;
			//timeroot2 = root3->Event->year * 365 + root3->Event->month * 30 + root3->Event->day + root3->Event->hour * 60 + root3->Event->min;
			if (1) {
				root3 = succ(root2);
				free(root2->Event);//�������� ������� �� ��������� ��������?
				root2->Event = root3->Event;
				tree* parent_root3 = root3->parent;
				if (root3->Right == NULL) {

					parent_root3->Left = NULL;
				}
				else {
					parent_root3->Left = root3->Right;
				}
				free(root3->Event);
				free(root3);
			}
			//else {
			//	//���������
			//}
		}
		else if (root2->Right == NULL && root2->Left != NULL) {
			root3 = root2->Left;
			root3->parent = NULL;
			free(root2->Event);
			free(root2);
		}
		else if (root2->Right != NULL && root2->Left == NULL) {
			root3 = root2->Right;
			root3->parent = NULL;
			(*Tree) = root3;//copy everywhere
			free(root2->Event);
			free(root2);
		}
		else {
			free(root2->Event);
			free(root2);
			printf("\nThe Note is empty now\n");
		}
	}
	else if ((root2->Left == NULL) && (root2->Right == NULL)) {
		root3 = root2->parent; //�� �����
		/*if (root3 == NULL) {
			printf("\nThe notebook is empty\n");
			return;
		}*/
		if (root2 == root3->Right) {
			root3->Right = NULL;
		}
		else {
			root3->Left = NULL;
		}
		free(root2->Event);
		free(root2);
	}

	else if ((root2->Left == NULL) && (root2->Right != NULL)) {
		root3 = root2->parent;
		if (root2 == root3->Right) {
			root3->Right = root2->Right;
		}
		else {
			root3->Left = root2->Right;
		}
		free(root2->Event);
		free(root2);
	}

	else if ((root2->Left != NULL) && (root2->Right == NULL)) {
		root3 = root2->parent;
		if (root2 == root3->Right) {
			root3->Right = root2->Left;
		}
		else {
			root3->Left = root2->Left;
		}
		free(root2->Event);
		free(root2);
	} //16.10.1111 07:12

	else if ((root2->Left != NULL) && (root2->Right != NULL)) { //����� �������������� ����� �������
		root3 = succ(root2);
		free(root2->Event);
		root2->Event = root3->Event;
		tree* parent_root3 = root3->parent;
		if (root3->Right == NULL) {

			parent_root3->Left = NULL;
		}
		else {
			parent_root3->Left = root3->Right;
		}
		free(root3->Event);
		free(root3);
	}
}

void input_date(int* year, int* month, int* day, int* hour, int* min) {
	printf("Input date (format dd.mm.yyyy hh:mm): ");
	scanf("%d.%d.%d %d:%d", day, month, year, hour, min);
}

void import_events(FILE * import, tree** Tree) {
	char c;
	c = 'o';
	int counter = 0;
	while ( c != EOF) {
		//char month[2], day[2], hour[2], min[2], description[500], place[100], importance[2], weekday[10];
		int year, month, day, hour, min;
		char description[500], place[100];
		int importance;
		counter++;
		fscanf(import, "%d", &year);
		fscanf(import, "%d", &month);
		fscanf(import, "%d", &day);
		fscanf(import, "%d", &hour);
		fscanf(import, "%d", &min);
		fgetc(import);
		fgets(description, 500, import);
		fgets(place, 100, import);
		fscanf(import, "%d", &importance);
		if (month > 12 || month < 1) {
			printf("\nincorrect month in Event %d\n", counter);
			break;
		}
		if (day > 31 || day < 1) {
			printf("\nincorrect day in Event %d\n", counter);
			break;
		}
		if (year > 9999 || year < 1) {
			printf("\nincorrect year in Event %d\n", counter);
			break;
		}
		if (hour > 23 || hour < 0) {
			printf("\nincorrect hour in Event %d\n", counter);
			break;
		}
		if (min > 59 || min < 0) {
			printf("\nincorrect minutes in Event %d\n", counter);
			break;
		}if (importance > 10 || importance < 0) {
			printf("\nincorrect importance in Event %d\n", counter);
			break;
		}
		for (int i = 0; i < 500; i++) {
			if (description[i] == '\n') {
				description[i] = '\0';
				break;
			}
		}
		for (int i = 0; i < 100; i++) {
			if (place[i] == '\n') {
				place[i] = '\0';
				break;
			}
		}
		event* new_event = (event*)malloc(sizeof(event));
		new_event->year = year;
		new_event->month = month;
		new_event->day = day;
		new_event->hour = hour;
		new_event->min = min;
		int flag = 0;
		int week_day = zeller(year, month, day);/*Zeller's algoritm for finding the day of the week*/
		switch (week_day) {
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
			puts("Incorrect weekday in event %d", counter);
			flag = 1;
			break;
		}
		if (flag == 1) {
			break;
		}
		strcpy(new_event->description, description);
		strcpy(new_event->place, place);
		new_event->importance = importance;

		if (*Tree == NULL) {
			*Tree = (tree*)malloc(sizeof(tree));
			(*Tree)->Event = new_event;
			(*Tree)->Left = NULL;
			(*Tree)->Right = NULL;
			(*Tree)->parent = NULL;
		}
		else {
			long int time_event = new_event->year * 364 + new_event->month * 30 + new_event->day + new_event->hour * 60 + new_event->min;
			tree* root2 = *Tree, * root3 = NULL;
			tree* new_tree = (tree*)malloc(sizeof(tree));
			long int time_tree;
			while (root2 != NULL) {
				root3 = root2;
				time_tree = root2->Event->year * 364 + root2->Event->month * 30 + root2->Event->day + root2->Event->hour * 60 + root2->Event->min;
				if (time_event < time_tree) {
					root2 = root2->Left;
				}
				else {
					root2 = root2->Right;
				}
			}
			new_tree->Event = new_event;
			new_tree->Left = NULL;
			new_tree->Right = NULL;
			new_tree->parent = root3;
			time_tree = root3->Event->year * 364 + root3->Event->month * 30 + root3->Event->day + root3->Event->hour * 60 + root3->Event->min;
			if (time_event < time_tree) {
				root3->Left = new_tree;
			}
			else {
				root3->Right = new_tree;
			}
		}
		c = fgetc(import);
		printf("\nEvent %d imported Succesfully!\n", counter);
	}

}

void output_events(FILE* input, tree* Tree, int flag) {
	if (Tree == NULL) {
		return;
	}
	output_events(input, Tree->Left, 0);
	fprintf(input,"%d\n", Tree->Event->year);
	fprintf(input, "%d\n", Tree->Event->month);
	fprintf(input, "%d\n", Tree->Event->day);
	fprintf(input, "%d\n", Tree->Event->hour);
	fprintf(input, "%d\n", Tree->Event->min);
	fputs(Tree->Event->description, input);
	fputc('\n', input);
	fputs(Tree->Event->place, input);
	fputc('\n', input);
	fprintf(input, "%d", Tree->Event->importance);
	if (Tree->Right != NULL) flag = 0; //���������� ����������� ��������� � flag
	if (flag != 1) {
		fprintf(input, "\n\n");
	}
	output_events(input, Tree->Right, 1);
}

int main() {
	int choice;
	tree* tree_for_day = NULL;
	long int time;
	int year, month, day, hour, min;
	char date[20];
	int sorttype;

	while (1) {
		puts("1)Add new event\n2)Delete event\n3)Print events\n4)Import events from file\n5)Exit\n6)Save events to file");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			add_event(&tree_for_day);
			printf("\nAdded succesfully!\n");
			break;
		case 2:
			input_date(&year, &month, &day, &hour, &min);
			printf("%d %d %d %d %d\n", year, month, day, hour, min);
			time = year * 364 + month * 30 + day + hour * 60 + min;
			delete_event(&tree_for_day, time);
			printf("\nDeleted succesfully!\n");
			break;
		case 3:
			printf("How do you want to sort it?\n1)By date\n2)By importance\n3)By place\n");
			scanf("%d", &sorttype);
			switch (sorttype) {
			case 1:
				printf("\nSorted by date:\n\n"); 
				printTree(tree_for_day);
				break;
			case 2:
				printf("\nSorted by importance:\n\n");
				printTreeImp(&tree_for_day);
				break;
			case 3:
				printf("\nSorted by place:\n\n");
				printTreePlace(&tree_for_day);
				break;
			}
			break;
		case 4:
			printf("Write the name of file: ");
			FILE* inputfile;
			char in[100];
			scanf("%s", &in);
			inputfile = fopen(in, "r");
			if (inputfile == NULL) {
				printf("Error. Restart file\n");
				break;
			}
			import_events(inputfile, &tree_for_day);
			fclose(inputfile);
			
			break;
		case 5:
			puts("Goodbye!");
			exit(0);
			break;
		case 6:
			printf("Write the name of file: ");
			FILE* innputfile;
			char inn[100];
			scanf("%s", &inn);
			innputfile = fopen(inn, "w+");
			if (innputfile == NULL) {
				printf("Error. Restart file\n");
				break;
			}
			output_events(innputfile, tree_for_day, 1);
			fclose(innputfile);
			printf("\nSaved succesfully!\n");
			break;
		default:
			puts("Incorrect data, please try again!");
		}
		printf("\n");
	}

	return 0;
}