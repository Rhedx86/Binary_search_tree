> With this project I now understand the pain of having lots of bugs in the code.
I've done my best to remove the most bugs I could find I don't know if there stil are, tell me if you ever find some bugs and let me take care of it.

It doesn't handle duplicates for the moment


### Delete function 
```c
void delete (struct node* deleted, int value)

{

 	if(root != NULL)

 	{	

 		if (value >= 0 && deleted == NULL)

 		{

 			deleted = binarySearch(value, root);

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
```

- The delete function wastes the stack, when deleting a node with no children it's pushing the whole function in the stack just for this piece of code 
```c
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
```

- Deleting a node with one child is the worst because of the previous reason plus it's recursive, it means that if we have a node with a bunch of nodes with one child, when deleting the function is going to call itself on the stack again and again. We have GB of RAM now so it's not a real problem actually but on embedded systems this would be a bad idea
- I should probably separate the function 
```c
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
```


### Tree visualizer
![[bst_1650564337014.png]]
