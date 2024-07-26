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

char	**d_set;

// should be initialised at start up
int	init_router(t_data *data)
{
	// should d_set be a fixed size array or
	// a linked list
	// a linked list makes sense since the
	// data will be read sequentially and so
	// there are no expensive look ups
	// cannot use the cmd args struct
	// 		require linked list of strings
	data->d_set = malloc();
}


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
//  (we are checking every token anyway against d_set)
// 2. when the string contains an =
//  (and i suppose every time if contains = )
//  (then we write manually the command createvar or so
//  	and pass it as 2nd argument first is the command)
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
// 		adds token also to the arg[0] position
// 			(we expand to absolute path later)
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
//
// redirection can be anywhere in a sequence which is fine 
// as when encountered it will be added along with the relevant
// filepath to the directoin list and will be at the correct position
// how should the filepath be stored
// this only applies to the first or last sequence? since redirection
// nullifies the function of the pipe
//
