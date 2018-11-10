#ifndef STRING_ARRAYS_H
#define STRING_ARRAYS_H
#include <stdio.h>
#include <stdlib.h>
#include <sds.h>

typedef struct {
	int count;
	sds *strings;
} StringList;

typedef struct {
	int count;
	StringList **lists;
} MDStringList; // multi-dimensional string list

static StringList* stringlist_init()
{
	StringList *list = malloc( sizeof(StringList) );
	list->strings = malloc( sizeof(sds) );
	list->strings[0] = sdsempty();

	if ( list->strings )
		list->count = 1;

	return list;
}

static void stringlist_append(StringList *list, sds str)
{
	sds *buffer = realloc(list->strings, (list->count+1)*sizeof(sds));
	if (buffer) {
		list->strings = buffer;
		list->count++;
	}
	list->strings[ list->count-1 ] = str;
}

static void stringlist_alter(StringList *list, int index, sds str)
{
	if (index+1 > list->count) {
		stringlist_append(list, sdsempty());
		stringlist_alter(list, index, str);
		return;
	}
	sdsfree( list->strings[index] );
  list->strings[index] = str;
}

static void stringlist_free(StringList *list)
{
	for (int i = 0; i < list->count; i++) {
		if (sdslen(list->strings[i]) > 0)
			sdsfree( list->strings[i] );
	}
	free( list->strings );
	free(list);
}

// Multi-dimensional

static MDStringList* mdstringlist_init()
{
	MDStringList *list = malloc( sizeof(MDStringList) );
	list->lists = malloc( sizeof(MDStringList*) );

	list->lists[0] = stringlist_init();

	if ( list->lists )
		list->count = 1;
	else
		list->count = 0;

	return list;
}

static void mdstringlist_addrow(MDStringList *list)
{
	list->lists = realloc(list->lists,  (list->count+1) * sizeof(MDStringList*));
	list->lists[ list->count ] = stringlist_init();
	list->count++;
}

static void mdstringlist_alter(MDStringList *list, int row, int col, sds str)
{
	if (row+1 > list->count) {
		mdstringlist_addrow(list);
		mdstringlist_alter(list, row, col, str);
		return;
	}
	StringList *selRow = list->lists[row];
	stringlist_alter(selRow, col, str);
}

static void mdstringlist_free(MDStringList *list)
{
	for (int i = 0; i < list->count; i++) {
		stringlist_free( list->lists[i] );
	}
	free( list->lists );
	free(list);
}

#endif
