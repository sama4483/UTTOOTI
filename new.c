#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define ERROR -1
#define LOGOUT -2
#define LOGIN 1 
#define SUCCEESS 0
typedef struct post_details post;
typedef struct account_info account;
struct post_details {
    char * username;
    int post_id;
    int like;
    char * post;
    post * next;
};
struct account_info {
    char* username;
    char* password;
    post posts_of_user;
    account * next;
    int logStatus;
};
char * dstr_read() /*function to read long inputs*/ {
    char * str;
    char buf[100]; /*buffer*/
    /*printf("%s", msg);*/
    scanf("%s", buf); /*read string in buffer*/
    str = (char * ) malloc(strlen(buf) + 1);
    if (str == NULL) {
        printf("Error: out of memory …\n");
        exit(1);
    }
    strcpy(str, buf);
    return str;
}
void printListOfAccounts(account * tempStd) {
    while (tempStd != NULL) {
        printf("user name: %s,pass:%s\n", tempStd -> username, tempStd -> password);
        tempStd = tempStd -> next;
    }
}

char * read_ignor() /*function to read long inputs ignoring spaces*/ {
    char * str;
    char buf[100]; /*buffer*/
    /*printf("%s", msg);*/
    scanf("%[^\n]s", buf); /*read string in buffer*/
    str = (char * ) malloc(strlen(buf) + 1);
    if (str == NULL) {
        printf("Error: out of memory …\n");
        exit(1);
    }
    strcpy(str, buf);
    return str;
}
/*function to add accounts*/
int insert_at_end(account * head, char * name, char * password) {
    int userLength=strlen(name);
    int passLength=strlen(password);
    account * newNode = (account * ) malloc(sizeof(account)); /*Creat newnode and fill info*/
    if(!newNode)
    printf(" MEMORY ALLOCATION FAILIURE!");
    newNode->username=(char*)realloc(newNode->username,userLength*sizeof(char));/*change size of node info*/
    newNode->password=(char*)realloc(newNode->password,passLength*sizeof(char));
    strcpy(newNode -> username, name);
    strcpy(newNode -> password, password);
    newNode -> next = NULL;
    while (head -> next != NULL)/*traverse list until last node*/
        head = head -> next;
    head -> next = newNode;
    return 0;
}


account* searchUser (char* name,account* head)/*function to search account by name*/
{
    account*current = (account*)malloc(sizeof(account));
	current = head;
	while (current->next != NULL)
	{
		current = current->next;
		if (strcmp(current->username, name) ==0)
			return current;
	}
	current = current->next;
	free(current);
	return NULL;
}
account* findLoggedInUser(account*head)
{
    account* current=(account*)malloc(sizeof(account));
    current=head;
    while (current->next != NULL)
	{
		current = current->next;
		if (current->logStatus == LOGIN)
			return current;
	}
	current = current->next;
	free(current);
	return NULL;
}
post*find_posts_by_name_id(char*name,int post_id,post*headOfPost,account*headOfAcc)
{   post*current=(post*)malloc (sizeof(post));
    current=headOfPost;
  while(current->next!=NULL)
    {
        current = current->next;
        if ((strcmp(searchUser(name,headOfAcc)->username,name)==0)&&current->post_id == post_id)/*check to find post with desired identifications*/
        return current;
	}
	current = current->next;
	free(current);
	return NULL;
} 


/*function used to find all of the posts belonged to a person*/
post*find_posts_by_name(char*name,post*headOfPost,account*headOfAcc)
{     
   post*current=(post*)malloc (sizeof(post));
    current=headOfPost;
  while(current->next!=NULL)
    {
        current = current->next;
        if ((strcmp(searchUser(name,headOfAcc)->username,name)==0))
        return current;
	}
	current = current->next;
	free(current);
	return NULL;
}



int signup(account*head,int*status)
{
char*name=dstr_read();
char*password=dstr_read();
account* signedupUser = searchUser(name, head);
	 if (*status != LOGOUT)                 /*check status*/
	{
		printf("%s\n", "\t\t Error: logout please");
		return ERROR;
	}
	if (signedupUser != NULL)
	{
		printf("%s\n", "\t\t Error: this user exists!");
		return ERROR;
	}
	insert_at_end(head,name,password);
	return SUCCEESS;
}
int login(account*head,int*status)
{   
    char*name=dstr_read();
    char*password=dstr_read();
    account* loggedInUser = searchUser(name, head);
    if (*status == LOGIN)
	{
		printf("%s\n", "\t\t Error: logout please");
		return ERROR;
	}
    if(loggedInUser==NULL)
    {printf("%s\n", "\t\tplease signup and then try again!");
    return ERROR;
    }
    
    if (strcmp(name, loggedInUser->username) == 0 &&
		strcmp(password, loggedInUser->password) == 0)
	{
		printf("%s\n", "\t\t successfully logged in");
		loggedInUser->logStatus = LOGIN;
		*status =LOGIN;
		return SUCCEESS;
	}
	 if (strcmp(name, loggedInUser->username)!= 0||   
		strcmp(password, loggedInUser->password) != 0)
    printf("%s\n", "\t\t failure enter information correctly");
    
}

  
int logout(account*head,int*status)
{
    if (*status == LOGOUT)
	{
		printf("%s\n", "\t\t Error: please login");
		return ERROR;
	}
	 account* loggedInUser = findLoggedInUser(head);
	loggedInUser->logStatus =LOGOUT;
	printf("%s\n", "\t\t successfully logged out");
	*status = LOGOUT;
	return SUCCEESS;
}
int add_post(account*headOfAcc ,post * head,int* status,int counter) {
    if (*status == LOGOUT)
	{
		printf("%s\n", "\t\t Error: please login");
		return ERROR;
	}
	account* loggedInUser = findLoggedInUser(headOfAcc);
    char*content= read_ignor();
    int postLength=strlen(content);
    post * newPost = (post * ) malloc(sizeof(post)); /*Creat newnode and fill info*/
    if(!newPost)
    printf(" MEMORY ALLOCATION FAILIURE!");
    newPost->post=(char*)realloc(newPost->post,postLength*sizeof(char));
    strcpy(newPost ->post, content);
    newPost->username=loggedInUser->username;
    newPost -> post_id=0;
    newPost -> next = NULL;
    while (head -> next != NULL)
        head = head -> next;
        head -> next = newPost;
        (newPost -> post_id)=counter;
        
    return SUCCEESS;
}

int like(post*headOfPost,account*headOfAcc,int*status)
{   char*name=dstr_read();
    int post_num;
    scanf("%d",&post_num);
     if (*status == LOGOUT)
	{
		printf("%s\n", "\t\t Error: please login");
		return ERROR;
	}
    post*liked_post=find_posts_by_name_id(name, post_num,headOfPost,headOfAcc);
    if(liked_post==NULL)
    {
        printf("not found!!");
        return ERROR;
    }
    else
    liked_post->like++;
}
void display(post* node){

    //as linked list will end when Node is Null
    while(node!=NULL)
    {
        printf("%s \n %s\n %d \n %d \n",node->username,node->post,node->post_id,node->like);
        node = node->next;
    }
    printf("\n");
}
int finduser(post*headOfPost,account*headOfAcc,int*status)
{   
    char* name=dstr_read();
     if (*status == LOGOUT)
	{
		printf("%s\n", "\t\t Error: please login");
		return ERROR;
	}
    post*target_post=find_posts_by_name(name,headOfPost,headOfAcc);
    if(target_post==NULL)
    {
        printf("not found!!");
        return ERROR;
    }
    else
    display(target_post);

}
int info(post*headOfPost,account*headOfAcc,int*status)/*function to show data of an account*/
{
    if (*status == LOGOUT)
	{
		printf("%s\n", "\t\t Error: please login");
		return ERROR;
	}
	account*logged_in=findLoggedInUser(headOfAcc);/*find the current user*/
	 printf("user name: %s,pass:%s\n",logged_in -> username,logged_in -> password);
	post*target_post=find_posts_by_name(logged_in->username,headOfPost,headOfAcc);
	if(target_post==NULL)
    {
        printf("not found!!");
        return ERROR;
    }
    else
    display(target_post);
	
}
int delete_post (account*headOfAcc,post*headOfPost,int*status)
{   int post_id;
    scanf("%d",&post_id);
    account*logged_in=findLoggedInUser(headOfAcc);
    char*name=logged_in->username;
    if (*status == LOGOUT)
	{
		printf("%s\n", "\t\t Error: please login");
		return ERROR;
	}
    post*prev=find_posts_by_name_id(name, post_id-1,headOfPost,headOfAcc);
    post* ptr= prev->next;
   if(prev==NULL)
   free(ptr);
  if( ptr ==NULL )
    {   
        printf("fail to delete does not exist");
        return ERROR;
        
    }
 prev->next = ptr->next;
  free(ptr);
  return SUCCEESS;
  

   
}


int main() 
 {  int stat =-2;
    int*status=& stat;/*variable to check status*/
    int op=1;
    int counter=0;;
    account* head_account=NULL;
    head_account=(account*)malloc(sizeof(account));//allocating a dummy node for head of accounts
    if(!head_account)
   { printf(" MEMORY ALLOCATION FAILIURE!");
     return 1;
   }
  post* head_post=NULL;
    head_post=(post*)malloc(sizeof(post));//allocating a dummy node for head of posts
    if(!head_post)
   { printf(" MEMORY ALLOCATION FAILIURE!");
     return 1;
   }
   char input[10];
  
   while (op)
   {     if(*status==LOGOUT)
        counter=0;
        printf("Enter an order to execute\n");/*code for operation */
        scanf("%s",&input);
        if(strcmp(input,"signup")==0)
      {
       signup(head_account,status);
       
      }
        if(strcmp(input,"login")==0)
       login(head_account,status);
        if(strcmp(input,"logout")==0)
       logout(head_account,status);
        if(strcmp(input,"post")==0)
      {
        add_post(head_account,head_post,status,counter);
        counter++;
        
      }
      if(strcmp(input,"like")==0)
     like( head_post,head_account,status);
      if(strcmp(input,"finduser")==0)
      finduser( head_post,head_account,status);
     if(strcmp(input,"info")==0)
     info( head_post,head_account,status);
     if(strcmp(input,"delete")==0)
     delete_post (head_account,head_post,status);
     if(strcmp(input,"end")==0)
     op=0;
    }
       
 }
 







