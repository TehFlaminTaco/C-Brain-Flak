#include "commands.c"
#ifndef define_stack
#include "stack.c"
#endif

define_stack(commandlett*, command_stack, (commandlett*)(long)printf("Tried to pop empty stack (%x)!", p_stack));

commandlett* parse_commands(FILE* file){
    command_stack* prog_stack = command_stack_new();
    commandlett* META_COMMANDLETT = commandlett_new(META);
    command_stack_push(prog_stack, META_COMMANDLETT);
    int next_char;
    int character = 0;
    while((next_char = fgetc(file))>-1){
        #define COMMAND_OPEN(token, value) case token: \
            top_command = command_stack_peek(prog_stack); \
            command_list_push(top_command -> arguments, command_stack_push(prog_stack, commandlett_new(value))); \
            break;
        
        #define COMMAND_CLOSE(token, value) case token: \
            top_command = command_stack_pop(prog_stack); \
            if(top_command -> action != value){ \
                printf("Unexpected character %c at position %i.", next_char, character); \
                exit(1); \
            } \
            break;
        commandlett* top_command;
        switch(next_char){
            COMMAND_OPEN('(', ONE)
            COMMAND_OPEN('[', HEIGHT)
            COMMAND_OPEN('{', POP)
            COMMAND_OPEN('<', TOGGLE)
            COMMAND_CLOSE(')', ONE)
            COMMAND_CLOSE(']', HEIGHT)
            COMMAND_CLOSE('}', POP)
            COMMAND_CLOSE('>', TOGGLE)
            default:
                printf("Unknown character %x found at position %i", next_char, character);
                exit(1);
        }

        #undef COMMAND_OPEN
        #undef COMMAND_CLOSE
        character++;
    }
    while(prog_stack -> length > 1){
        command_stack_pop(prog_stack);
    }
    META_COMMANDLETT -> action = TOGGLE;
    return META_COMMANDLETT;
}