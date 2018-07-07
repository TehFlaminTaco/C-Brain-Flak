#include <stdio.h>
#include <stdlib.h>
#include "stack.c"
#include "command_parser.c"

define_stack(int, iStack, 0)

const char* names[] = {
    "ONE", "HEIGHT", "POP", "TOGGLE"
};
void debug_command_list(const char* name, command_list* list, int depth){
    for(int d=0; d < depth; d++)
        printf("\t");
    printf("%s (%i)\n", name, list -> length);
    for(int i=0; i < list -> length; i++){
        commandlett* sub_command = command_list_get(list, i);
        debug_command_list(names[sub_command -> action], sub_command -> arguments, depth+1);
    }
}

iStack* stacks[2];
int cur_stack = 0;
// COMMAND FUNCTIONS
// Making this a seperate file was #EFFORT
int evaluate_commandlett(commandlett* action);

int nilad_ONE(){
    return 1;
}

int nilad_HEIGHT(){
    return stacks[cur_stack] -> length;
}

int nilad_POP(){
    return iStack_pop(stacks[cur_stack]);
}

int nilad_TOGGLE(){
    cur_stack = 1 - cur_stack;
    return 0;
}

//

int manyad_ONE(commandlett* action){
    int sum = 0;
    for(int i=0; i < action -> arguments -> length; i++){
        sum += evaluate_commandlett(command_list_get(action -> arguments, i));
    }
    iStack_push(stacks[cur_stack], sum);
    return sum;
}

int manyad_HEIGHT(commandlett* action){
    int sum = 0;
    for(int i=0; i < action -> arguments -> length; i++){
        sum += evaluate_commandlett(command_list_get(action -> arguments, i));
    }
    return -sum;
}

int manyad_POP(commandlett* action){
    int sum = 0;
    while(iStack_peek(stacks[cur_stack])){
        for(int i=0; i < action -> arguments -> length; i++){
            sum += evaluate_commandlett(command_list_get(action -> arguments, i));
        }
    }
    iStack_pop(stacks[cur_stack]);
    return -sum;
}

int manyad_TOGGLE(commandlett* action){
    for(int i=0; i < action -> arguments -> length; i++){
        evaluate_commandlett(command_list_get(action -> arguments, i));
    }
    return 0;
}

//

int evaluate_commandlett(commandlett* action){
    #define MAKE_ACTION(name) case name: return action -> arguments -> length ? manyad_##name(action) : nilad_##name();

    switch(action -> action){
        MAKE_ACTION(ONE)
        MAKE_ACTION(HEIGHT)
        MAKE_ACTION(POP)
        MAKE_ACTION(TOGGLE)
    }
}

int main(int argc, char** argv){
    if(argc <= 1){
        printf("Expected format: %s <file>", argv[0]);
        return 1;
    }

    stacks[0] = iStack_new();
    stacks[1] = iStack_new();

    FILE* code_file = fopen(argv[1], "rb");

	commandlett* parsed = parse_commands(code_file);
    //debug_command_list("PROGRAM", parsed -> arguments, 0);
    evaluate_commandlett(parsed);

    int out;
    while(out = iStack_pop(stacks[cur_stack])){
        printf("%i\n", out);
    }

    fclose(code_file);
	return 0;
}