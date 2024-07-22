#include "minishell.h"

// define directions set

// is the grammar checker asking questions like
// is this a command? Is there any meta data to
// help with the parsing?

// a linked list makes sense here as then only one
// pass through the tokens is required the size
// the number of commands not being known beforehand

// every time commands is called it appends a new
// cmd arg struct

void	sorting()
{
// 	check 1. & 2.

// 	if in set of direction symbols
//		directions()

//	else
//		commands()

//	while not end of args	
//		arguments()
}

// cases where the first string in a
// grouping of n strings is not a command
//
// 1. when the pipeline begins < filein
// 2. when the string contains an =
//
// so if 1.
// 		create cmd arg struct
// 		send next token (filepath) to args
// 		or that is to say increment tokens 
// 		and send to args
// if 2.
// 		send to user_var create function

// commands()
// 		appends a new struct or creates one if
// 		none exists
// 		adds the token to the struct->cmd member
// 		reads forward in tokens until it finds
// 		the end of arguments
// 		creats space for n + 1 args
//
// arguments()
// 		adds argument to current struct->arg[i]
//
// directions()
// 		appends to list of direction symbols (and if last
// 		redirection, filepath sub to stdout) 
// 		list passed to chain_pipes()
//
//
