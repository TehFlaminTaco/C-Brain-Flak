#define define_stack(type, name, def)																						\
struct _stacklett_##name {																									\
	type value;																												\
	struct _stacklett_##name* next;																							\
};																															\
typedef struct{																												\
	struct _stacklett_##name* head;																							\
	int length;																												\
} name;																														\
name* name##_new(){																											\
	name* l_##name = (name*) malloc(sizeof(name));																			\
	l_##name -> length = 0;																									\
	return l_##name;																										\
}																									                        \
struct _stacklett_##name* _stacklett_##name##_new(type p_value, struct _stacklett_##name* p_next){                          \
	struct _stacklett_##name* l_stacklett_##name = (struct _stacklett_##name*)malloc(sizeof(struct _stacklett_##name));     \
	l_stacklett_##name -> value = p_value;																					\
	l_stacklett_##name -> next = p_next;																					\
	return l_stacklett_##name;																								\
}																															\
type name##_push(name* p_stack, type p_value){																				\
	p_stack -> head = _stacklett_##name##_new(p_value, p_stack -> head);													\
    ++(p_stack -> length);                                                                                                  \
	return p_value;                     																					\
}																															\
type name##_peek(name* p_stack){																							\
	if(p_stack -> length == 0){																								\
	return def;																												\
	}																														\
	return p_stack -> head -> value;																						\
}																															\
type name##_pop(name* p_stack){																								\
	if(p_stack -> length == 0){																								\
	return def;																												\
	}																														\
	struct _stacklett_##name* l_old_head = p_stack -> head;																	\
	p_stack -> length--;																									\
	p_stack -> head = l_old_head -> next;																					\
	type l_old_value = l_old_head -> value;																					\
	free(l_old_head);																										\
	return l_old_value;																										\
}																															\
int name##_empty(name* p_stack){																							\
	return !(p_stack -> length);																							\
}
