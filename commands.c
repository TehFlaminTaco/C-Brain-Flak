#include <stdio.h>

enum _command {
    ONE,    // ()
    HEIGHT, // []
    POP,    // {}
    TOGGLE, // <>
    META    // CODE STUFFS
};
typedef enum _command command;

typedef struct commandlett commandlett;

struct _command_list_sub {
    struct _command_list_sub* next;
    struct _command_list_sub* prev;
    commandlett* value;
};

typedef struct command_list{
    struct _command_list_sub* head;
    int length;
} command_list;

command_list* command_list_new(){
    command_list* l_newlist = (command_list*) malloc(sizeof(command_list));
    l_newlist -> length = 0;
    return l_newlist;
}

#define command_list_contains(list, index) (!(index < 0 || index >= list -> length))

struct _command_list_sub* _command_list_get(command_list* list, int index){
    if (index < 0){
        printf("Tried to index (%i) of a command list. (A negative value)", index);
        exit(1);
    }
    if(index >= list -> length){
        if(list -> length == 0){
            printf("Tried to index out of range value (%i) in empty command list.", index, list -> length);
        }
        printf("Tried to index out of range value (%i) in command list of length (%i).", index, list -> length);
    }
    struct _command_list_sub* tar = list -> head;
    while(index > 0){
        index--;
        tar = tar -> next;
    }
    return tar;
}

#define command_list_get(list, index) ((commandlett*)(_command_list_get(list, index)->value))

// Contains explicit behavoir if index = length.
int command_list_set(command_list* list, int index, commandlett* value){
    if (index < 0){
        printf("Tried to index (%i) of a command list. (A negative value)", index);
        exit(1);
    }
    int list_length = list -> length;
    if(index > list_length){
        printf("Tried to index out of range value (%i) in command list of length (%i).", index, list -> length);
        exit(1);
    }
    if(index == list_length){
        struct _command_list_sub* new_sublet = (struct _command_list_sub*) malloc(sizeof(struct _command_list_sub));
        new_sublet -> value = value;
        if(index > 0){
            new_sublet -> prev = _command_list_get(list, index-1);
            new_sublet -> prev -> next = new_sublet;
        }else{
            list -> head = new_sublet;
        }
        (list -> length)++;
        return index;
    }
    _command_list_get(list, index) -> value = value;
    return index;
}

#define command_list_push(list, value) command_list_set(list, list -> length, value)

commandlett* command_list_remove(command_list* list, int index){
    if (index < 0){
        printf("Tried to index (%i) of a command list. (A negative value)", index);
        exit(1);
    }
    if(index >= list -> length){
        if(list -> length == 0){
            printf("Tried to index out of range value (%i) in empty command list.", index, list -> length);
        }
        printf("Tried to index out of range value (%i) in command list of length (%i).", index, list -> length);
    }
    if(index == 0){
        struct _command_list_sub* old_head = list -> head;
        commandlett* old_value = old_head -> value;
        list -> head = old_head -> next;
        free(old_head);
        list -> length--;
        return old_value;
    }
    struct _command_list_sub* cur_sub = _command_list_get(list, index);
    commandlett* old_value = cur_sub -> value;
    cur_sub -> prev -> next = cur_sub -> next;
    list -> length--;
    if(index < list -> length){
        cur_sub -> next -> prev = cur_sub -> prev;
    }
    free(cur_sub);
    return old_value;
}
#define command_list_pop(list) command_list_remove(list, list -> length - 1)

struct commandlett{
    command action;
    command_list* arguments;
};

commandlett* commandlett_new(command action){
    commandlett* new_command = (commandlett*)malloc(sizeof(commandlett));
    new_command -> arguments = command_list_new();
    new_command -> action = action;
    return new_command;
}