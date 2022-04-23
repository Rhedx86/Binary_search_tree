#include <stdio.h>
#include <stdlib.h>
#define SIZE 50

struct node
{
	int data;
	struct node* right;
	struct node* left;
	struct node* parent;
};

struct node* root = NULL;
int treeIterator = 0;

void insertion(int newData);
struct node* binarySearch(int);
struct node** breadthFirstSearch(); // is not really searching but go through every level of the tree
void delete (struct node* deleted, int value);
void swapData(int*, int*);
int findHeight();
void reinsertion(struct node*); // was made because at some point when debbuging 558239 was at the right of 702583
							    // actually I could just swap the values but I decided to stay with that 
void checkForReinsertion(struct node* deletedParent, struct node* deleted);
void swapChildren(struct node* ptr);
void invert();
void depthFirstSearch(struct node*);

int main()
{
	int arr[] = { 446584, 1452, 52, 45, 741222, 1363, 2, 54632, 885563, 123, 145, 558239, 702583 };
	int i;
	for (i = 0; i < sizeof(arr) / 4; i++) { insertion(arr[i]); }
	findHeight();
	binarySearch(54632);
	depthFirstSearch(NULL);


	for (i = 0; i < sizeof(arr) / 4; i++) { insertion(arr[i]); }
	return 0;
}

void insertion(int newData)
{
	if(treeIterator < SIZE)
	{
		struct node* newLeaf = (struct node*)malloc(sizeof(struct node));
		(*newLeaf).data = newData;
		(*newLeaf).right = NULL;
		(*newLeaf).left = NULL;
		(*newLeaf).parent = NULL;
		struct node* leafPtr = NULL;

		if (root == NULL) {
			root = newLeaf;
			treeIterator++;
		}
		else
		{
			leafPtr = root;
			while (leafPtr != NULL)//this code will search for free space
			{
				if ((*newLeaf).data < (*leafPtr).data)
				{
					if ((*leafPtr).left == NULL) { // checks for free place at the left 
						(*newLeaf).parent = leafPtr;
						(*leafPtr).left = newLeaf;
						break;
					}
					leafPtr = (*leafPtr).left;
				}
				else
				{
					if ((*leafPtr).right == NULL) { // checks for free place at the right
						(*newLeaf).parent = leafPtr;
						(*leafPtr).right = newLeaf;
						break;
					}
					leafPtr = (*leafPtr).right;
				}
			}
			treeIterator++;

		}
	}
	else
		printf("no, you can't add\n");
}

void reinsertion(struct node* reinserted) // this won't just swap the parent value with the reinserted value 
{										  // reinserted might have a new parent 
	(*reinserted).right = NULL;
	(*reinserted).left = NULL; // the reinserted node is becoming a leaf 
	struct node* parent = (*reinserted).parent;

	if((*parent).right == reinserted)
		(*parent).right = NULL;
	else if ((*parent).left == reinserted)
		(*parent).left = NULL;

	struct node* ptr = root;
	while (ptr != NULL)//this code will search for free space
	{
		if ((*reinserted).data < (*ptr).data)
		{
			if ((*ptr).left == NULL) { // checks for free place at the left 
				(*reinserted).parent = ptr;
				(*ptr).left = reinserted;
				break;
			}
			ptr = (*ptr).left;
		}
		else if ((*ptr).data == (*reinserted).data)
			return;
		else
		{
			if ((*ptr).right == NULL) { // checks for free place at the right
				(*reinserted).parent = ptr;
				(*ptr).right = reinserted;
				break;
			}
			ptr = (*ptr).right;
		}
	}
}

struct node* binarySearch(int key)
{
	struct node* ptr = root;
	if (key > -1)
	{
		if (key == (*ptr).data)
			return ptr;
		else if (key < (*ptr).data)
		{
			ptr = (*ptr).left;
			return binarySearch(key, ptr);
		}
		else
		{
			ptr = (*ptr).right;
			return binarySearch(key, ptr);
		}
	}
	return NULL;
}

void swapData(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

int findHeight()
{
	if (root == NULL)
		return 0;
	struct node** result = breadthFirstSearch();
	struct node* ptr = *(result + treeIterator - 1); // take the last element added in the arr because it's the lowest node in the tree  
	int height = 1;

	while (ptr != root) {// counts how many times there's a parent
		ptr = (*ptr).parent;
		height++;
	}//
	free(result);
	return height;
}

void depthFirstSearch(struct node* ptr)
{
	if(ptr == NULL)
		ptr = root;
	if((*ptr).left != NULL)
		depthFirstSearch((*ptr).left);

	if((*ptr).right != NULL)
		depthFirstSearch((*ptr).right);
}

struct node** breadthFirstSearch()
{
	if (root == NULL)
		return NULL;
	struct node** arrHeap = (struct node**)malloc(SIZE);
	struct node* arr[SIZE] = { 0x00 };
	arr[0] = root;
	int i = 0;
	int j = 1;
	for (i; i < treeIterator; i++) // put every node from the tree in the arr left to right level per level
	{
		if ((*arr[i]).left != NULL || (*arr[i]).right != NULL)
		{
			if ((*arr[i]).left != NULL) {
				arr[j] = (*arr[i]).left;
				j++;
			}
			if ((*arr[i]).right != NULL) {
				arr[j] = (*arr[i]).right;
				j++;
			}
		}
	}//
	arrHeap = arr;
	return arrHeap;
}

void swapChildren(struct node* ptr)
{
	struct node* temp = (*ptr).left;
	(*ptr).left = (*ptr).right;
	(*ptr).right = temp;
}

void invert()
{
	struct node** ptrToArr = breadthFirstSearch();
	int i = 0;
	struct node* ptr;
	for(i; i < treeIterator; i++){
		ptr = *(ptrToArr + i);
		swapChildren(ptr);
	}
	free (ptrToArr);
}

void checkForReinsertion(struct node* deletedParent, struct node* deleted)
{
	if(deletedParent == NULL)
		return;
	if ((*deletedParent).data < (*deleted).data)
			if ((*deletedParent).left == deleted)
				reinsertion(deleted);
		if ((*deletedParent).data > (*deleted).data)
			if ((*deletedParent).right == deleted)
				reinsertion(deleted);
}
void delete (struct node* deleted, int value)
{
	if(root != NULL)
	{
		if (value >= 0 && deleted == NULL)
		{
			deleted = binarySearch(value);
			if (deleted == NULL)
				return;
		}

		struct node* deletedParent = (*deleted).parent;
		if ((*deleted).right == NULL && (*deleted).left == NULL) // this part delete nodes without children
		{
			if (deletedParent == NULL) { root = NULL; }
			else
			{
				if ((*deletedParent).left == deleted)
					(*deletedParent).left = NULL;
				else
					(*deletedParent).right = NULL;
			}
			free(deleted);
			return;
		}//
		else if ((*deleted).right == NULL) // this part delete nodes with one child at the left
		{
			struct node* ptr = (*deleted).left;
			(*deleted).data = (*ptr).data;
			delete(ptr, -1);
			deletedParent = (*deleted).parent;
		}//
		else if ((*deleted).left == NULL) // this part delete nodes with one child at the right 
		{
			struct node* ptr = (*deleted).right;
			(*deleted).data = (*ptr).data;
			delete(ptr, -1);
			deletedParent = (*deleted).parent;
		}//
		else if ((*deleted).right != NULL && (*deleted).left != NULL) // this part delete node with two children 
		{
			struct node* ptr = (*deleted).right;
			struct node* ptrParent;

			if ((*ptr).left == NULL) {
				swapData(&(*ptr).data, &(*deleted).data);
				delete(ptr, -1);//
				deletedParent = (*deleted).parent;
			}
			else
			{
				// this loop take the value of deleted and brings it down the tree 
				//then deletes the last node containing this value  
				while ((*ptr).right != NULL || (*ptr).left != NULL)
				{
					ptrParent = (*ptr).parent;
					swapData(&(*ptrParent).data, &(*ptr).data);
					if ((*ptr).left == NULL && (*ptr).right != NULL)
						ptr = (*ptr).right;
					else if ((*ptr).left != NULL && (*ptr).right == NULL || (*ptr).left != NULL && (*ptr).right != NULL)
						ptr = (*ptr).left;
				}
				ptrParent = (*ptr).parent;
				swapData(&(*ptrParent).data, &(*ptr).data);
				delete(ptr, -1);//
				// this part take the value down to the tree and brings it up
				do {
					ptr = ptrParent;
					ptrParent = (*ptrParent).parent;
					swapData(&(*ptrParent).data, &(*ptr).data);
				} while (ptrParent != deleted);//
			}
		} //
		checkForReinsertion(deletedParent, deleted);
	}
	else 
		printf("no, you can't delete\n");
}